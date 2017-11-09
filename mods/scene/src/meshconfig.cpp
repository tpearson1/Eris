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

#include <meshconfig.h>
#include <test/macros.h>

std::unordered_map<std::string, MeshRenderConfigs::Generator>
    MeshRenderConfigs::configurationGenerators;

void MeshRenderConfigs::UV::Setup(std::vector<VertexAttribute> &attributes) {
  attributes.emplace_back(1, 2, 0, uvs);
  uvs.clear();
}

void MeshRenderConfigs::Normal::Setup(
    std::vector<VertexAttribute> &attributes) {
  attributes.emplace_back(0, 3, 0, normals);
  normals.clear();
}

void MeshRenderConfigs::Standard::Setup(
    std::vector<VertexAttribute> &attributes) {
  attributes.emplace_back(1, 2, 0, uvs);
  attributes.emplace_back(2, 3, 0, normals);
}

void MeshRenderConfigs::Single::PreRender() {
  auto mvp = NCamera::active->Matrix(globalTransform.Matrix());
  mvpUniform.SetMatrix4(1, GL_FALSE, mvp);
}

void MeshRenderConfigs::Textures::GetUniforms(Shader &s) {
  textureUniforms.reserve(textures.size());
  for (auto &pair : textures)
    textureUniforms.push_back(s.GetUniform(pair.uniform));
}

void MeshRenderConfigs::Textures::PreRender() {
  for (std::size_t i = 0; i < textureUniforms.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].texture->ID());
    textureUniforms[i].Set(static_cast<GLint>(i));
  }
}

void MeshRenderConfigs::Lit::GetUniforms(Shader &s) {
  specularUniform = s.GetUniform("material.specular");
  shininessUniform = s.GetUniform("material.shininess");
  modelUniform = s.GetUniform("model");
}

void MeshRenderConfigs::Lit::PreRender() {
  Transform t;
  // REVIEW: Heuristic for instancing?
  // If the Mesh is being instanced single will be nullptr, and the transform
  // to use for the closest lights is defaulted to (0,0,0)
  if (single) t = single->GetGlobalTransform();

  assert(lightingConfig);
  assert(LightManager::Active());
  LightManager::Active()->SetUniformsForClosestLights(t.Location(),
                                                      *lightingConfig);

  specularUniform.Set(specular);
  shininessUniform.Set(shininess);
  modelUniform.SetMatrix4(1, false, t.Matrix());
}

void JSONImpl<MeshRenderConfigs::NamedTexturePair>::Read(
    MeshRenderConfigs::NamedTexturePair &out, const JSON::Value &value,
    const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "MeshRenderConfigs::NamedTexturePair"};
  const auto &object = JSON::GetObject(value, data);
  JSON::GetMember(out.uniform, "uniform", object, data);
  auto textureStr = JSON::GetMember<std::string>("texture", object, data);
  out.texture = Resources::active->textures.Get(textureStr);
}

void JSONImpl<MeshRenderConfigs::Textures>::Read(
    MeshRenderConfigs::Textures &out, const JSON::Value &value,
    const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "MeshRenderConfigs::Textures"};
  const auto &object = JSON::GetObject(value, data);
  JSON::GetMember(out.textures, "textures", object, data);
}

void JSONImpl<MeshRenderConfigs::Lit>::Read(MeshRenderConfigs::Lit &out,
                                            const JSON::Value &value,
                                            const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "MeshRenderConfigs::Lit"};
  const auto &object = JSON::GetObject(value, data);

  JSON::GetMember(out.specular, "specular", object, data);
  JSON::GetMember(out.shininess, "shininess", object, data);
  out.lightingConfig = std::make_shared<LightingConfig>();
  JSON::GetMember(out.lightingConfig->maxDirectionalLights, "max-dir-lights",
                  object, data);
  JSON::GetMember(out.lightingConfig->maxPointLights, "max-point-lights",
                  object, data);
}
