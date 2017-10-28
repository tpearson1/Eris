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
  Tex1D = GL_TEXTURE_1D,
  Tex2D = GL_TEXTURE_2D,
  Tex3D = GL_TEXTURE_3D,
  Rectangle = GL_TEXTURE_RECTANGLE,
  Buffer = GL_TEXTURE_BUFFER,
  Cubemap = GL_TEXTURE_CUBE_MAP,
  Tex1DArray = GL_TEXTURE_1D_ARRAY,
  Tex2DArray = GL_TEXTURE_2D_ARRAY,
  CubemapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
  Tex2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
  Tex2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

enum class TextureShrinkType : GLint {
  NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
  LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
  NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
  LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR
};

enum class TextureEnlargeType : GLint {
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR
};

struct TextureSettings {
  TextureSettings() {}
  TextureSettings(TextureType tt, TextureShrinkType tst, TextureEnlargeType tet)
                 : type(tt), shrinkFilter(tst), enlargeFilter(tet) {}

  std::string path;

  TextureType type = TextureType::Tex2D;

  TextureShrinkType shrinkFilter = TextureShrinkType::Linear;
  TextureEnlargeType enlargeFilter = TextureEnlargeType::Linear;

  static TextureSettings nearest, linear;
};

template <>
struct JSONImpl<TextureSettings> {
  static void Read(TextureSettings &out, const JSON::Value &value, const JSON::ReadData &data);
  static void Write(const TextureSettings &value, JSON::Writer &writer);
};

#endif // _BASE__TEXTURE_SETTINGS_H
