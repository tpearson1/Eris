/*
-------------------------------------------------------------------------------
This file is part of Eris Engine
-------------------------------------------------------------------------------
Copyright (c) 2017 Thomas Pearson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
-------------------------------------------------------------------------------
*/

#include <image.h>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <core/statics.h>
#include <png.h>

bool RawImage::Load(const std::string &path, bool flip) {
  // Read the vertex shader code from the file
  // We need to use c-style file reading as libpng accepts a c-style file pointer
  FILE *fp = fopen((::buildPath + path).c_str(), "rb");
  if (!fp) {
    std::cerr << "> Unable to open image file: " << path << '\n';
    return false;
  }

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    std::cerr << "> Unable to create png read struct\n";
    fclose(fp);
    return false;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    std::cerr << "> Unable to create png info struct\n";
    png_destroy_read_struct(&png, NULL, NULL);
    fclose(fp);
    return false;
  }

  if (setjmp(png_jmpbuf(png))) {
    std::cerr << "> Unable to set jump\n";
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return false;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  size.x = png_get_image_width(png, info);
  size.y = png_get_image_height(png, info);
  png_byte colorType = png_get_color_type(png, info);
  png_byte bitDepth = png_get_bit_depth(png, info);

  if (bitDepth == 16)
    png_set_strip_16(png);

  if (colorType == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16 bit depth
  if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color types don't have an alpha channel so we fill it with 0xff
  if (colorType == PNG_COLOR_TYPE_RGB ||
      colorType == PNG_COLOR_TYPE_GRAY ||
      colorType == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xff, PNG_FILLER_AFTER);

  if (colorType == PNG_COLOR_TYPE_GRAY ||
      colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  int rowBytes = png_get_rowbytes(png, info);
  png_bytep rowPointers[size.y];

  for (int y = 0; y < size.y; y++)
    rowPointers[y] = new png_byte[rowBytes];

  png_read_image(png, rowPointers);

  png_destroy_read_struct(&png, &info, NULL);
  fclose(fp);

  data.reserve(rowBytes * size.y);

  unsigned char *tmp = data.data();

  if (flip) {
    for (int i = 0; i < size.y; i++) {
      auto row = size.y - i - 1;
      memcpy(tmp + (rowBytes * row), rowPointers[i], rowBytes);
      delete[] rowPointers[i];
    }
  }
  else {
    for (int i = 0; i < size.y; i++) {
      memcpy(tmp + (rowBytes * i), rowPointers[i], rowBytes);
      delete[] rowPointers[i];
    }
  }

  return true;
}
