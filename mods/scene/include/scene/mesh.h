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

#ifndef _SCENE__MESH_H
#define _SCENE__MESH_H

#include <functional>
#include <unordered_map>
#include <base/mesh.h>
#include <scene/node.h>
#include <scene/camera.h>
#include <scene/renderer.h>

class NMesh : public NNode {
  std::shared_ptr<Mesh> mesh;

public:
  const Mesh *Get() { return mesh.get(); }
  void Set(const std::shared_ptr<Mesh> &value) { mesh = std::move(value); }

  const Shader *GetShader() const { return shader.get(); };

  void SetShader(const std::shared_ptr<const Shader> &s) {
    Renderer::active->UpdateRequirements(registration, shader = s);
  }

  NMesh(const std::shared_ptr<const Shader> &s) {
    registration = Renderer::active->Register([this] { this->Draw(); }, this, shader = s);
  }

  NMesh &operator=(const NMesh &mr);

  NMesh(const NMesh &mr) { *this = mr; }

  ~NMesh() {
    Renderer::active->Unregister(registration);
  }

private:
  void Draw() {
    mesh->Draw(NCamera::active->Matrix(transform.GlobalTransform().Matrix()));
  }

  std::shared_ptr<const Shader> shader;
  Renderer::Registration registration;
};

#endif // _SCENE__MESH_H

