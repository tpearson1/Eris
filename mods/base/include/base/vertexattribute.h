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

#ifndef _BASE__VERTEX_ATTRIBUTE_H
#define _BASE__VERTEX_ATTRIBUTE_H

#include <type_traits>
#include <vector>
#include <base/gl.h>
#include <base/buffer.h>

template <typename T = GLfloat>
class VertexAttribute {
  static_assert(std::is_same<T, GLfloat>::value ||
                std::is_same<T, GLuint>::value ||
                std::is_same<T, GLint>::value,
                "Template Class 'VertexAttribute' must have template type GLfloat, GLuint or GLint");

  unsigned index, columns;
  std::vector<T> data;
  Buffer<T> buf;

public:
  VertexAttribute() {}
  VertexAttribute(unsigned attrIndex, unsigned valuesPerRow, const std::vector<T> &_data) {
    index = attrIndex;
    columns = valuesPerRow;
    data = _data;
  }

  void Setup() {
    buf.Data(data);
    buf.Generate();
  }

  void Enable() const {
    glEnableVertexAttribArray(index);
    buf.Use();

    GLenum type;
    if (std::is_same<T, GLfloat>::value)
      type = GL_FLOAT;
    else if (std::is_same<T, GLuint>::value)
      type = GL_UNSIGNED_INT;
    else if (std::is_same<T, GLint>::value)
      type = GL_INT;
    glVertexAttribPointer(index, columns, type, GL_FALSE, 0, (GLvoid *)0);
  }

  void Disable() const
    { glDisableVertexAttribArray(index); }

  template <size_t NumAttrs>
  friend class InstancedMesh;
};

#endif // _BASE__VERTEX_ATTRIBUTE_H
