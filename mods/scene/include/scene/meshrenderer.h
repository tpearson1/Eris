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

#ifndef _SCENE__MESH_RENDERER_H
#define _SCENE__MESH_RENDERER_H

#include <memory>
#include <functional>
#include <core/mapping.h>
#include <base/mesh.h>
#include <scene/node.h>
#include <scene/camera.h>
#include <scene/renderer.h>

class MeshRenderer {
  std::shared_ptr<const Mesh> mesh;

public:
  void Draw(const Mat4 &global);

  MeshRenderer() {}
  MeshRenderer(const std::shared_ptr<const Mesh> &value) : mesh(value) {}

  std::shared_ptr<const Mesh> Get() { return mesh; }
  const std::shared_ptr<const Mesh> Get() const { return mesh; }
  void Set(const std::shared_ptr<const Mesh> &value) { mesh = value; }
};

class NMeshRenderer : public NNode {
public:
  using PreRenderFunctionType = std::function<void(NMeshRenderer *)>;

  static Mapping<std::string, PreRenderFunctionType> preRenderFunctions;
  PreRenderFunctionType preRenderFunction;

  MeshRenderer renderer;

  const RenderRequirements &GetRequirements() const { return requirements; };

  void SetRequirements(const RenderRequirements &r) {
    Renderer::active->UpdateRequirements(registration, requirements = r);
  }

  NMeshRenderer(const RenderRequirements &r) {
    registration = Renderer::active->Register([this] { this->Draw(); }, this, requirements = r);
  }

private:
  void Draw() {
    if (preRenderFunction)
      preRenderFunction(this);
    renderer.Draw(NCamera::active->Matrix(transform.GlobalTransform().Matrix()));
  }

  RenderRequirements requirements;
  RenderRegistration registration;
};

#endif // _SCENE__MESH_RENDERER_H
