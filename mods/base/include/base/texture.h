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

#ifndef _BASE__TEXTURE_H
#define _BASE__TEXTURE_H

#include <base/gl.h>
#include <base/image.h>
#include <base/texturesettings.h>
#include <base/window.h>

class Texture {
  GLuint id = 0;
  int width = 0, height = 0;

public:
  bool Load(const TextureSettings &settings);
  void Load(const RawImage &raw, const TextureSettings &settings);

  void CreateForFramebuffer(int _width = Window::inst->width, int _height = Window::inst->height);

  ~Texture()
    { if (id) glDeleteTextures(1, &id); }

  GLuint ID() const { return id; }
  int Width() const { return width; }
  int Height() const { return height; }
};

#endif // _BASE__TEXTURE_H
