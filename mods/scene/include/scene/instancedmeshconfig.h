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

#ifndef _SCENE__INSTANCED_MESH_CONFIG_H
#define _SCENE__INSTANCED_MESH_CONFIG_H

#include <algorithm>
#include <base/mesh.h>
#include <base/shader.h>
#include <core/readwrite.h>
#include <scene/camera.h>
#include <scene/meshconfig.h>
#include <scene/transform.h>

namespace MeshRenderConfigs {
namespace Instanced {
template <typename ConfigBase>
struct AddTransformation : public ConfigBase {
  template <typename... Args>
  AddTransformation(Args &&... args)
      : ConfigBase(std::forward<Args>(args)...) {}

  void SetupAddTransformation(const std::vector<Mat4> &transformationMatrices) {
    this->attrs.emplace_back(3, 16, 1, transformationMatrices);
  }

  void SetupAddTransformation(const std::vector<Transform> &transformations) {
    std::vector<Mat4> matrices;
    std::transform(std::begin(transformations), std::end(transformations),
                   std::back_inserter(matrices),
                   [](auto t) -> Mat4 { return t.Matrix(); });
    SetupAddTransformation(matrices);
  }

  virtual void PreRender() const override {
    auto camera = NCamera::active;
    auto VP = camera->ProjectionMatrix() * camera->ViewMatrix();
    Shader::SetUniformMatrix4(Shader::Current()->GetUniform("VP"), 1, false,
                              VP);
  }

  static void Read(AddTransformation &in, const JSON::Value &value,
                   const JSON::ReadData &data, const NMesh &mesh) {
    auto t = Trace::Pusher{data.trace,
                           "MeshRenderConfigs::AddTransformation<T>::Read"};
    const auto &object = JSON::GetObject(value, data);

    TryCallRead<ConfigBase>(in, value, data, mesh);

    std::vector<Transform> transformations;
    JSON::GetMember(transformations, "transformations", object, data);
    in.SetupAddTransformation(transformations);
  }
};
}
}

#endif // _SCENE__INSTANCED_MESH_CONFIG_H
