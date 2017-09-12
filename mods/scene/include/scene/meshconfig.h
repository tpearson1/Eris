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

#ifndef _SCENE__MESH_CONFIG_H
#define _SCENE__MESH_CONFIG_H

#include <base/resources.h>
#include <base/texture.h>
#include <scene/lightmanager.h>
#include <scene/mesh.h>
#include <test/macros.h>

namespace MeshRenderConfigs {
struct Standard;

using Generator = std::shared_ptr<Standard> (*)(const JSON::Value &,
                                                const JSON::ReadData &,
                                                const NMesh &);

extern std::unordered_map<std::string, Generator> configurationGenerators;

IS_VALID_EXPR(HasReadStaticMemberFunction, Type::Read)

template <typename ConfigType>
Generator MakeGenerator() {
  return [](const auto &value, const auto &data,
            const auto &mesh) -> std::shared_ptr<Standard> {
    auto c = std::make_shared<ConfigType>();
    static_assert(HasReadStaticMemberFunction<ConfigType>::value,
                  "Must have static member 'Read' to use 'MakeGenerator'");
    ConfigType::Read(*c, value, data, mesh);
    return c;
  };
}

// TODO: Refactor for if constexpr when updating to C++17
template <typename T>
inline std::enable_if_t<HasReadStaticMemberFunction<T>::value>
TryCallRead(T &in, const JSON::Value &value, const JSON::ReadData &data,
            const NMesh &mesh) {
  T::Read(in, value, data, mesh);
}

template <typename T>
inline std::enable_if_t<!HasReadStaticMemberFunction<T>::value>
TryCallRead(T &, const JSON::Value &, const JSON::ReadData &, const NMesh &) {}

struct NamedTexturePair {
  std::string uniform;
  std::shared_ptr<Texture> texture;
};

template <typename ConfigBase>
struct AddTextures : public ConfigBase {
  std::vector<NamedTexturePair> textures;

  template <typename... Args>
  AddTextures(Args &&... args) : ConfigBase(std::forward<Args>(args)...) {}

  virtual void PreRender() const override {
    int i = 0;
    for (auto &pair : textures) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, pair.texture->ID());
      auto sampler = Shader::Current()->GetUniform(pair.uniform);
      Shader::SetUniform(sampler, 0);
      i++;
    }
  }

  static void Read(AddTextures &in, const JSON::Value &value,
                   const JSON::ReadData &data, const NMesh &mesh) {
    auto t =
        Trace::Pusher{data.trace, "MeshRenderConfigs::AddTextures<T>::Read"};
    const auto &object = JSON::GetObject(value, data);

    TryCallRead<ConfigBase>(in, value, data, mesh);
    JSON::GetMember(in.textures, "textures", object, data);
  }
};

template <typename ConfigBase>
struct MakeLit : public ConfigBase {
  std::shared_ptr<LightingConfig> lightingConfig;
  Vec3 specular = Vec3::one * 0.5f;
  float shininess = 32.0f;
  const Transform *transform = nullptr;

  template <typename... Args>
  MakeLit(Args &&... args) : ConfigBase(std::forward<Args>(args)...) {}

  virtual void PreRender() const override {
    ConfigBase::PreRender();

    if (!transform) {
      std::cerr << "Member 'transform' must be specified for "
                   "MeshRenderConfigs::SingleTextureLit\n";
      return;
    }

    LightManager::active->SetUniformsForClosestLights(transform->Location(),
                                                      *lightingConfig);

    const auto *cur = Shader::Current();
    cur->SetUniform(cur->GetUniform("material.specular"), specular);
    cur->SetUniform(cur->GetUniform("material.shininess"), shininess);
    cur->SetUniformMatrix4(cur->GetUniform("model"), 1, false,
                           transform->Matrix());
  }

  static void Read(MakeLit &in, const JSON::Value &value,
                   const JSON::ReadData &data, const NMesh &mesh) {
    auto t = Trace::Pusher{data.trace, "MeshRenderConfigs::MakeLit<T>::Read"};
    const auto &object = JSON::GetObject(value, data);

    TryCallRead<ConfigBase>(in, value, data, mesh);

    in.transform = &mesh.transform;

    JSON::GetMember(in.specular, "specular", object, data);
    JSON::GetMember(in.shininess, "shininess", object, data);
    in.lightingConfig = std::make_shared<LightingConfig>();
    JSON::GetMember(in.lightingConfig->maxDirectionalLights, "max-dir-lights",
                    object, data);
    JSON::GetMember(in.lightingConfig->maxPointLights, "max-point-lights",
                    object, data);
  }
};
}

template <>
struct JSONImpl<MeshRenderConfigs::NamedTexturePair> {
  static void Read(MeshRenderConfigs::NamedTexturePair &out,
                   const JSON::Value &value, const JSON::ReadData &data);
};

#endif // _SCENE__MESH_CONFIG_H
