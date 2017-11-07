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

#include <pointlight.h>

#include <base/shader.h>

void NPointLight::SetUniformData(const std::string &prefix) const {
  auto s = Shader::Current();
  s->GetUniform(prefix + ".location").Set(transform.Location());
  s->GetUniform(prefix + ".ambient").Set(ambient);
  s->GetUniform(prefix + ".diffuse").Set(diffuse);
  s->GetUniform(prefix + ".specular").Set(specular);
  s->GetUniform(prefix + ".constant").Set(constant);
  s->GetUniform(prefix + ".linear").Set(linear);
  s->GetUniform(prefix + ".quadratic").Set(quadratic);
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

  auto lightManager = LightManager::Active();
  JSON::ParseAssert(lightManager, data, "Class 'LightManager' must have its static member 'active' set");
  out.Register(*lightManager);
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
