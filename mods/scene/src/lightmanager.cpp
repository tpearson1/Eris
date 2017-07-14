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

#include <lightmanager.h>
#include <base/shader.h>
#include <scene/camera.h>

std::unique_ptr<LightManager> LightManager::active;

void NPointLight::SetUniformData(const std::string &prefix) const {
  const auto *cur = Shader::Current();
  cur->SetUniform(cur->GetUniform(prefix + ".location"), transform.Location());
  cur->SetUniform(cur->GetUniform(prefix + ".ambient"), ambient);
  cur->SetUniform(cur->GetUniform(prefix + ".diffuse"), diffuse);
  cur->SetUniform(cur->GetUniform(prefix + ".specular"), specular);
  cur->SetUniform(cur->GetUniform(prefix + ".constant"), constant);
  cur->SetUniform(cur->GetUniform(prefix + ".linear"), linear);
  cur->SetUniform(cur->GetUniform(prefix + ".quadratic"), quadratic);
}

void JSONImpl<NPointLight>::Read(NPointLight &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "NPointLight"};
  const auto &object = JSON::GetObject(value, data);

  JSON::TryGetMember(out.ambient, "ambient", object, Vec3::one * 0.1f, data);
  JSON::GetMember(out.diffuse, "diffuse", object, data);
  JSON::GetMember(out.specular, "specular", object, data);
  JSON::TryGetMember(out.constant, "constant", object, 1.0f,  data);
  JSON::GetMember(out.linear, "linear", object, data);
  JSON::GetMember(out.quadratic, "quadratic", object, data);

  JSON::GetMember<NNode>(out, "NNode", object, data);

  JSON::ParseAssert(LightManager::active.get(), data, "Class 'LightManager' must have it's static member 'active' set");
  LightManager::active->RegisterPointLight(&out);
}

void JSONImpl<NPointLight>::Write(const NPointLight &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
  JSON::Write(static_cast<const NNode &>(value), writer);
  JSON::WritePair("ambient", value.ambient, writer);
  JSON::WritePair("diffuse", value.diffuse, writer);
  JSON::WritePair("specular", value.specular, writer);
  JSON::WritePair("constant", value.constant, writer);
  JSON::WritePair("linear", value.linear, writer);
  JSON::WritePair("quadratic", value.quadratic, writer);
}

void NDirectionalLight::SetUniformData(const std::string &prefix) const {
  const auto *cur = Shader::Current();
  cur->SetUniform(cur->GetUniform(prefix + ".direction"), transform.GlobalRotation() * Vec3::front);
  cur->SetUniform(cur->GetUniform(prefix + ".ambient"), ambient);
  cur->SetUniform(cur->GetUniform(prefix + ".diffuse"), diffuse);
  cur->SetUniform(cur->GetUniform(prefix + ".specular"), specular);
}

void JSONImpl<NDirectionalLight>::Read(NDirectionalLight &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "NDirectionalLight"};
  const auto &object = JSON::GetObject(value, data);

  JSON::TryGetMember(out.ambient, "ambient", object, Vec3::one * 0.1f, data);
  JSON::GetMember(out.diffuse, "diffuse", object, data);
  JSON::GetMember(out.specular, "specular", object, data);

  JSON::ParseAssert(LightManager::active.get(), data, "Class 'LightManager' must have it's static member 'active' set");
  LightManager::active->RegisterDirectionalLight(&out);
}

void JSONImpl<NDirectionalLight>::Write(const NDirectionalLight &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
  JSON::WritePair("ambient", value.ambient, writer);
  JSON::WritePair("diffuse", value.diffuse, writer);
  JSON::WritePair("specular", value.specular, writer);
}

void LightManager::SetUniformsForClosestLights(Vec3 location, LightManager::DirSizeType maxDirCount, LightManager::PointSizeType maxPointCount) {
  const auto *cur = Shader::Current();
  cur->SetUniform(cur->GetUniform("cameraLocation"), NCamera::active->transform.Location());
  cur->SetUniform(cur->GetUniform("numPointLights"), Math::Min<GLint>(maxPointCount, pointLights.size()));
  cur->SetUniform(cur->GetUniform("numDirectionalLights"), Math::Min<GLint>(maxDirCount, directionalLights.size()));

  auto dirIt = directionalLights.begin();
  auto less = Math::Min(maxDirCount, directionalLights.size());
  for (auto i = 0u; i < less; i++, dirIt++) {
    std::stringstream iss;
    iss << "directionalLights[" << i << ']';
    (*dirIt)->SetUniformData(iss.str());
  }

  // Order point lights by distance to objects
  pointLights.sort([&location](const auto &lhs, const auto &rhs) {
    return Vec3::Distance(lhs->transform.Location(), location) < Vec3::Distance(rhs->transform.Location(), location);
  });

  less = Math::Min(maxPointCount, pointLights.size());
  auto pointIt = pointLights.begin();
  for (auto i = 0u; i < less; i++, pointIt++) {
    std::stringstream iss;
    iss << "pointLights[" << i << ']';
    (*pointIt)->SetUniformData(iss.str());
  }
}

