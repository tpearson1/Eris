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

#ifndef _BASE__TEXTURE_SETTINGS_H
#define _BASE__TEXTURE_SETTINGS_H

#include <string>
#include <core/readwrite.h>
#include <base/gl.h>

enum class TextureType : GLenum {
  TEX_1D = GL_TEXTURE_1D,
  TEX_2D = GL_TEXTURE_2D,
  TEX_3D = GL_TEXTURE_3D,
  RECTANGLE = GL_TEXTURE_RECTANGLE,
  BUFFER = GL_TEXTURE_BUFFER,
  CUBEMAP = GL_TEXTURE_CUBE_MAP,
  TEX_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
  TEX_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  CUBEMAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
  TEX_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
  TEX_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

enum class TextureShrinkType : GLint {
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR
};

enum class TextureEnlargeType : GLint {
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR
};

struct TextureSettings {
  TextureSettings() {}
  TextureSettings(TextureType tt, TextureShrinkType tst, TextureEnlargeType tet)
                 : type(tt), shrinkFilter(tst), enlargeFilter(tet) {}

  std::string path;

  TextureType type = TextureType::TEX_2D;

  TextureShrinkType shrinkFilter = TextureShrinkType::LINEAR;
  TextureEnlargeType enlargeFilter = TextureEnlargeType::LINEAR;

  static TextureSettings nearest, linear;
};

template <>
struct JSONImpl<TextureSettings> {
  static void Read(TextureSettings &out, const JSON::Value &value, const JSON::ReadData &data);
  static void Write(const TextureSettings &value, JSON::Writer &writer);
};

#endif // _BASE__TEXTURE_SETTINGS_H
