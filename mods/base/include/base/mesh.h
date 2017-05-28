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

class RawMesh {
  VertexAttribute<> vertices;
  VertexArray vao;
  ElementBuffer indices;

public:
  RawMesh(const std::vector<GLfloat> &verts,
          const std::vector<GLuint> &indexData);

  void Draw() const;

  virtual void EnableAttributes() const {}
  virtual void DisableAttributes() const {}

  virtual ~RawMesh() {}
};

class UVMesh : public RawMesh {
  VertexAttribute<> uvs;

public:
  UVMesh(const std::vector<GLfloat> &verts,
         const std::vector<GLuint> &indexData,
         const std::vector<GLfloat> &uvData)
      : RawMesh(verts, indexData) {
    uvs = VertexAttribute<>{1, 2, uvData};
    uvs.Setup();
  }

  virtual void EnableAttributes() const override { uvs.Enable(); }
  virtual void DisableAttributes() const override { uvs.Disable(); }
};

class NormalMesh : public RawMesh {
  VertexAttribute<> normals;

public:
  NormalMesh(const std::vector<GLfloat> &verts,
             const std::vector<GLuint> &indexData,
             const std::vector<GLfloat> &normalData)
      : RawMesh(verts, indexData) {
    normals = VertexAttribute<>{2, 3, normalData};
    normals.Setup();
  }

  virtual void EnableAttributes() const override { normals.Enable(); }
  virtual void DisableAttributes() const override { normals.Disable(); }
};

class StandardMesh : public UVMesh {
  VertexAttribute<> normals;

public:
  StandardMesh(const std::vector<GLfloat> &verts,
               const std::vector<GLuint> &indexData,
               const std::vector<GLfloat> &uvData,
               const std::vector<GLfloat> &normalData)
      : UVMesh(verts, indexData, uvData) {
    normals = VertexAttribute<>{2, 3, normalData};
    normals.Setup();
  }

  virtual void EnableAttributes() const override {
    UVMesh::EnableAttributes();
    normals.Enable();
  }

  virtual void DisableAttributes() const override {
    UVMesh::DisableAttributes();
    normals.Disable();
  }
};

#endif // _BASE__MESH_H
