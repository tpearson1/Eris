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

#include <mesh.h>

RawMesh::RawMesh(const std::vector<GLfloat> &verts,
           const std::vector<GLuint> &indexData) {
  // Vertices, Element buffer and passing attribute to vertex shader
  vao.Generate();
  vao.Use();

  vertices = VertexAttribute<>(0, 3, verts);
  vertices.Setup();

  indices.Data(indexData);
  indices.Generate();

  // We do not clear the vertex array so that deriving classes can add to it
}

void RawMesh::Draw() const {
  vao.Use();

  vertices.Enable();
  EnableAttributes();

  indices.Use();
  indices.Draw();

  vertices.Disable();
  DisableAttributes();

  Buffer<GLfloat>::ClearUse();
  ElementBuffer::ClearUse();
  VertexArray::ClearUse();
}

