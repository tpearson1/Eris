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

#ifndef _BASE__INSTANCED_MESH_H
#define _BASE__INSTANCED_MESH_H

#include <base/mesh.h>

struct InstancedVertexAttribute {
  size_t divisor;
  VertexAttribute<> attr;

  InstancedVertexAttribute() {}
  InstancedVertexAttribute(size_t di, unsigned attrIndex, unsigned cols, const std::vector<GLfloat> &data)
    { divisor = di; attr = VertexAttribute<>(attrIndex, cols, data); }
};

template <size_t AttrCount>
class InstancedMesh {
  const Ref<Mesh> mesh;

  size_t instanceCount;
  std::array<InstancedVertexAttribute, AttrCount> attrs;

public:
  InstancedMesh() {}

  void Draw() const {
    mesh->vao.Use();
    mesh->vertices.Enable();
    for (auto &elem : mesh->attrs)
      elem.Enable();

    mesh->indices.Use();

    for (auto &elem : attrs)
      elem.attr.Enable();

    glDrawElementsInstanced(GL_TRIANGLES, mesh->indices.data.size(), GL_UNSIGNED_INT, 0, instanceCount);

    mesh->vertices.Disable();
    for (auto &elem : mesh->attrs)
      elem.Disable();
    for (auto &elem : attrs)
      elem.attr.Disable();

    Buffer<GLfloat>::ClearUse();
    ElementBuffer::ClearUse();

    VertexArray::ClearUse();
  }

  template <typename... Attrs>
  InstancedMesh(const Ref<Mesh> &m, size_t count, Attrs... a)
    { Setup(m, count, a...); }

  template <typename... Attrs>
  void Setup(const Ref<Mesh> &m, size_t count, Attrs... a) {
    instanceCount = count;
    mesh = m;
    mesh->vao.Use();
    attrs = std::array<InstancedVertexAttribute, AttrCount>({a...});
    for (auto &elem : attrs) {
      elem.attr.Enable();
      glVertexAttribDivisor(elem.attr.index, elem.divisor);
      elem.attr.Disable();
    }
    VertexArray::ClearUse();
  }

  const Ref<Mesh> Get() { return mesh; }
  const Ref<const Mesh> Get() const { return mesh; }
};

#endif // _BASE__INSTANCED_MESH_H
