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

#include <scene/camera.h>
#include <scene/lightmanager.h>
#include <scene/meshrenderer.h>

#include <base/resources.h>
#include <base/texture.h>
#include <test/macros.h>

namespace MeshRenderConfigs {
IS_VALID_EXPR(ImplementsSetCompose, &Type::SetCompose)
IS_VALID_EXPR(ImplementsGetUniforms, &Type::GetUniforms)
IS_VALID_EXPR(ImplementsPreRender, &Type::PreRender)
IS_VALID_EXPR(ImplementsSetup, &Type::Setup)

template <typename... Configs>
class Compose : public MeshRenderer, public Configs... {
  template <typename C>
  void TryCallSetCompose() {
    if constexpr (ImplementsSetCompose<C>::value)
      static_cast<C *>(this)->SetCompose();
  }

  template <typename C>
  void TryCallGetUniforms(Shader &s) {
    if constexpr (ImplementsGetUniforms<C>::value)
      static_cast<C *>(this)->GetUniforms(s);
  }

  template <typename C>
  void TryCallPreRender() {
    if constexpr (ImplementsPreRender<C>::value)
      static_cast<C *>(this)->PreRender();
  }

  template <typename C>
  void TryCallSetup() {
    if constexpr (ImplementsSetup<C>::value)
      static_cast<C *>(this)->Setup(vertexAttributes);
  }

public:
  template <typename C>
  auto &Get() {
    return *static_cast<C *>(this);
  }

  template <typename C>
  auto &Get() const {
    return *static_cast<C *>(this);
  }

  IS_VALID_EXPR(CanGetImpl,
                std::declval<Compose<Configs...>>().template Get<Type>())

  template <typename Type>
  static inline constexpr bool canGet = CanGetImpl<Type>::value;

  template <typename Func>
  void ForEach(Func func) {
    (..., func(Get<Configs>()));
  }

  Compose() { (..., TryCallSetCompose<Configs>()); }

  virtual void GetUniforms(Shader &s) override {
    (..., TryCallGetUniforms<Configs>(s));
  }

  virtual void PreRender() override { (..., TryCallPreRender<Configs>()); }

  virtual void Setup() override { (..., TryCallSetup<Configs>()); }
};
} // namespace MeshRenderConfigs

template <typename... Configs>
struct JSONImpl<MeshRenderConfigs::Compose<Configs...>> {
  static void Read(MeshRenderConfigs::Compose<Configs...> &out,
                   const JSON::Value &value, const JSON::ReadData &data) {
    auto t =
        Trace::Pusher{data.trace, "MeshRenderConfigs::Compose<Configs...>"};

    out.ForEach([&](auto &config) {
      using ConfigType = std::remove_reference_t<decltype(config)>;
      if constexpr (JSON::implementsRead<ConfigType>)
        JSONImpl<ConfigType>::Read(config, value, data);
    });
  }
};

namespace MeshRenderConfigs {
struct UV {
  std::vector<GLfloat> uvs;
  void Setup(std::vector<VertexAttribute> &attributes);
};

struct Normal {
  std::vector<GLfloat> normals;
  void Setup(std::vector<VertexAttribute> &attributes);
};

struct Standard {
  std::vector<GLfloat> uvs, normals;
  void Setup(std::vector<VertexAttribute> &attributes);
};

struct Single {
  void GetUniforms(Shader &s) { mvpUniform = s.GetUniform("MVP"); }

  void PreRender();

  const Transform &GetGlobalTransform() const { return globalTransform; }
  void SetGlobalTransform(const Transform &t) { globalTransform = t; }

  template <typename... Configs>
  static Single &Get(Compose<Configs...> &compose) {
    static_assert(Compose<Configs...>::template canGet<Single>,
                  "Cannot get class 'MeshRenderConfigs::Single'");
    return compose.template Get<Single>();
  }

private:
  Transform globalTransform;

  Shader::Uniform mvpUniform;
};

struct GeneratorReturn {
  std::shared_ptr<MeshRenderer> meshRenderer;
  Single &single;
  Standard &standard;
};

using Generator = GeneratorReturn (*)(const JSON::Value &,
                                      const JSON::ReadData &);

extern std::unordered_map<std::string, Generator> configurationGenerators;

template <typename ConfigType>
Generator MakeGenerator() {
  static_assert(JSON::implementsRead<ConfigType>,
                "Must implement 'JSONImpl<T>::Read' for type to use "
                "'MakeGenerator'");
  return [](const auto &value, const auto &data) -> GeneratorReturn {
    auto c = std::make_shared<ConfigType>();
    JSONImpl<ConfigType>::Read(*c, value, data);
    return {c, c->template Get<Single>(), c->template Get<Standard>()};
  };
}

struct NamedTexturePair {
  std::string uniform;
  std::shared_ptr<Texture> texture;
};

struct Textures {
  std::vector<NamedTexturePair> textures;
  std::vector<Shader::Uniform> textureUniforms;

  void GetUniforms(Shader &s);

  void PreRender();
};

struct Lit {
  std::shared_ptr<LightingConfig> lightingConfig;
  Vec3 specular = Vec3::one * 0.5f;
  float shininess = 32.0f;

  Single *single = nullptr;

  template <typename Composed>
  void SetCompose(Composed &c) {
    if constexpr (c.template canGet<Single>) single = &c.template Get<Single>();
  }

  void GetUniforms(Shader &s);

  void PreRender();

private:
  Shader::Uniform specularUniform, shininessUniform, modelUniform;
};
} // namespace MeshRenderConfigs

template <>
struct JSONImpl<MeshRenderConfigs::NamedTexturePair> {
  static void Read(MeshRenderConfigs::NamedTexturePair &out,
                   const JSON::Value &value, const JSON::ReadData &data);
};

template <>
struct JSONImpl<MeshRenderConfigs::Textures> {
  static void Read(MeshRenderConfigs::Textures &out, const JSON::Value &value,
                   const JSON::ReadData &data);
};

template <>
struct JSONImpl<MeshRenderConfigs::Lit> {
  static void Read(MeshRenderConfigs::Lit &out, const JSON::Value &value,
                   const JSON::ReadData &data);
};

#endif // _SCENE__MESH_CONFIG_H
