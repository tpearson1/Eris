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
#include <math/mat.h>
#include <memory>
#include <type_traits>
#include <vector>

class VertexAttribute {
  struct Base {
    virtual void Setup(unsigned index, unsigned columns,
                       unsigned instanceDivisor) = 0;
    virtual ~Base() {}
  };

  template <typename T>
  struct Data : Base {
    static_assert(std::is_same<T, GLfloat>::value ||
                      std::is_same<T, GLuint>::value ||
                      std::is_same<T, GLint>::value ||
                      std::is_same<T, Mat4>::value,
                  "Template Class 'VertexAttribute' must have template type "
                  "GLfloat, GLuint, GLint or Mat4");

    Buffer<T> buf;
    std::vector<T> data;

    virtual void Setup(unsigned index, unsigned columns,
                       unsigned instanceDivisor) override {
      buf.Generate();
      data.clear();

      GLenum type;
      if (std::is_same<T, GLfloat>::value)
        type = GL_FLOAT;
      else if (std::is_same<T, GLuint>::value)
        type = GL_UNSIGNED_INT;
      else if (std::is_same<T, GLint>::value)
        type = GL_INT;
      else if (std::is_same<T, Mat4>::value) {
        auto rowSize = 4 * sizeof(float);
        for (auto i = 0; i < 4; i++) {
          glEnableVertexAttribArray(index + i);
          glVertexAttribPointer(index + i, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4),
                                (GLvoid *)(i * rowSize));
          
          if (instanceDivisor)
            glVertexAttribDivisor(index + i, instanceDivisor);
        }
        return;
      }

      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index, columns, type, GL_FALSE, 0, (GLvoid *)0);

      if (instanceDivisor)
        glVertexAttribDivisor(index, instanceDivisor);
    }
  };

  unsigned index, columns, instanceDivisor;
  std::unique_ptr<Base> data;

public:
  template <typename T>
  VertexAttribute(unsigned attrIndex, unsigned _columns, unsigned divisor,
                  const std::vector<T> &_data) {
    index = attrIndex;
    columns = _columns;
    instanceDivisor = divisor;
    auto d = std::make_unique<Data<T>>();
    d->buf.Data(_data);
    data = std::move(d);
  }

  void Setup() { data->Setup(index, columns, instanceDivisor); }

  unsigned GetIndex() const { return index; }
  unsigned GetColumns() const { return columns; }

  friend class InstancedMesh;
};

#endif // _BASE__VERTEX_ATTRIBUTE_H
