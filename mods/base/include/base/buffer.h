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

#ifndef _BASE__BUFFER_H
#define _BASE__BUFFER_H

#include <vector>
#include <base/gl.h>

template <typename T>
class Buffer {
  GLuint ID = 0;
  std::vector<T> data;

public:
  Buffer() {}
  Buffer(const std::vector<T> &bufferData) : data(bufferData) {}
  ~Buffer()
    { if (ID) glDeleteBuffers(1, &ID); }

  void Data(const std::vector<T> &value) { data = value; }

  void Use() const { glBindBuffer(GL_ARRAY_BUFFER, ID); }

  static void ClearUse() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  void Generate() {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
  }
};

class ElementBuffer {
  GLuint ID = 0;
  std::vector<GLuint> data;

public:
  ElementBuffer() {}
  ElementBuffer(const std::vector<GLuint> &bufferData) : data(bufferData) {}
  ~ElementBuffer()
    { if (ID) glDeleteBuffers(1, &ID); }

  void Data(const std::vector<GLuint> &value) { data = value; }

  void Use() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }

  static void ClearUse() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

  void Generate() {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), GL_STATIC_DRAW);
  }

  void Draw() const
    { glDrawElements(GL_TRIANGLES, data.size(), GL_UNSIGNED_INT, 0); }

  template <size_t NumAttrs>
  friend class InstancedMesh;
};

#endif // _BASE__BUFFER_H
