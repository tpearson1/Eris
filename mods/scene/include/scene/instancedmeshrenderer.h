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

#ifndef _SCENE__INSTANCED_MESH_RENDERER_H
#define _SCENE__INSTANCED_MESH_RENDERER_H

#include <memory>
#include <base/shader.h>
#include <base/instancedmesh.h>
#include <scene/component.h>

template <size_t attrCount>
class CInstancedMeshRenderer : public CComponent {
  std::shared_ptr<const InstancedMesh<attrCount>> mesh;
  bool visible = true;

protected:
  void _Draw(const Mat4 &global) {
    if (!visible)
      return;

    GLint shaderMVP = Shader::Current().GetUniform("MVP");
    Shader::SetUniformMatrix4(shaderMVP, 1, GL_FALSE, global);
    mesh->Draw();
  }

public:
  CInstancedMeshRenderer() {}
  CInstancedMeshRenderer(const std::shared_ptr<const InstancedMesh<attrCount>> &m)
    { Set(m); }

  std::shared_ptr<const InstancedMesh<attrCount>> Get() { return mesh; }
  const std::shared_ptr<const InstancedMesh<attrCount>> Get() const { return mesh; }
  void Set(const std::shared_ptr<const InstancedMesh<attrCount>> &m) { mesh = m; }

  bool Visible() const { return visible; }
  void Visible(bool value) { visible = value; }
};

#endif // _SCENE__INSTANCED_MESH_RENDERER_H
