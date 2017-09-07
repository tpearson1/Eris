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

#include <base/buffer.h>
#include <base/gl.h>
#include <memory>
#include <type_traits>
#include <vector>

class VertexAttribute {
  struct Base {
    virtual void Setup(unsigned index, unsigned columns) = 0;
    virtual ~Base() {}
  };

  template <typename T>
  struct Data : Base {
    static_assert(std::is_same<T, GLfloat>::value ||
                      std::is_same<T, GLuint>::value ||
                      std::is_same<T, GLint>::value,
                  "Template Class 'VertexAttribute' must have template type "
                  "GLfloat, GLuint or GLint");

    Buffer<T> buf;
    std::vector<T> data;

    virtual void Setup(unsigned index, unsigned columns) override {
      buf.Generate();
      data.clear();

      glEnableVertexAttribArray(index);

      GLenum type;
      if (std::is_same<T, GLfloat>::value)
        type = GL_FLOAT;
      else if (std::is_same<T, GLuint>::value)
        type = GL_UNSIGNED_INT;
      else if (std::is_same<T, GLint>::value)
        type = GL_INT;
      glVertexAttribPointer(index, columns, type, GL_FALSE, 0, (GLvoid *)0);
    }
  };

  unsigned index, columns;
  std::unique_ptr<Base> data;

public:
  template <typename T>
  VertexAttribute(unsigned attrIndex, unsigned _columns,
                  const std::vector<T> &_data) {
    index = attrIndex;
    columns = _columns;
    auto d = std::make_unique<Data<T>>();
    d->buf.Data(_data);
    data = std::move(d);
  }

  void Setup() { data->Setup(index, columns); }

  template <size_t NumAttrs>
  friend class InstancedMesh;
};

#endif // _BASE__VERTEX_ATTRIBUTE_H
