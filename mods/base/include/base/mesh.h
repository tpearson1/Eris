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

#ifndef _BASE__MESH_H
#define _BASE__MESH_H

#include <iostream>
#include <vector>
#include <array>
#include <base/vertexattribute.h>
#include <math/vec.h>

class VertexArray {
  GLuint ID = 0;

public:
  VertexArray() {}
  ~VertexArray()
    { if (ID) glDeleteVertexArrays(1, &ID); }

  void Generate()
    { glGenVertexArrays(1, &ID); }
  void Use() const
    { glBindVertexArray(ID); }
  static void ClearUse()
    { glBindVertexArray(0); }
};

class Mesh {
  VertexAttribute<> vertices;
  std::vector<VertexAttribute<>> attrs;

  VertexArray vao;
  ElementBuffer indices;

public:
  Mesh() {}

  Mesh(const std::vector<GLfloat> &verts, const std::vector<GLuint> &indexData, const std::vector<VertexAttribute<>> &&atts)
    { Setup(verts, indexData, std::forward<const std::vector<VertexAttribute<>>>(atts)); }

  void Setup(const std::vector<GLfloat> &verts, const std::vector<GLuint> &indexData, const std::vector<VertexAttribute<>> &&atts);

  void Draw() const;

  template <size_t NumAttrs>
  friend class InstancedMesh;
};

class MeshTemplates {
  MeshTemplates();

  static Mesh quad;

public:
  static const Mesh &Quad() { return quad; }

  static void SetupStatics();
};


#endif // _BASE__MESH_H
