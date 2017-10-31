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

#ifndef _SCENE__LIGHT_MANAGER_H
#define _SCENE__LIGHT_MANAGER_H

#include <list>
#include <memory>

#include <base/registration.h>
#include <scene/node.h>

class NPointLight;
class NDirectionalLight;

struct LightingConfig;

class LightManager {
  using PointLightList = std::list<NPointLight *>;
  using DirectionalLightList = std::list<NDirectionalLight *>;

  PointLightList pointLights;
  DirectionalLightList directionalLights;

  void SetDirectionalLights(const LightingConfig &config);
  void SetPointLights(Vec3 location, const LightingConfig &config);

  static std::unique_ptr<LightManager> active;

public:
  using PointLightRegistration = Registration<PointLightList>;
  using DirectionalLightRegistration = Registration<DirectionalLightList>;

  static LightManager *Active() { return active.get(); }
  static void SetActive(std::unique_ptr<LightManager> m) {
    active = std::move(m);
  }

  PointLightRegistration Register(NPointLight *light) {
    return {light, pointLights};
  }

  DirectionalLightRegistration Register(NDirectionalLight *light) {
    return {light, directionalLights};
  }

  using PointSizeType = std::list<NPointLight *>::size_type;
  using DirSizeType = std::list<NDirectionalLight *>::size_type;
  void SetUniformsForClosestLights(Vec3 location,
                                   const struct LightingConfig &lightingConfig);
};

struct NLight : public NNode {
  virtual void SetUniformData(const std::string &prefix) const = 0;
};

class NPointLight : public NLight {
  LightManager::PointLightRegistration registration;

public:
  virtual void SetUniformData(const std::string &prefix) const override;

  Vec3 ambient = Vec3::one * 0.1f, diffuse, specular;
  float constant = 1.0f, linear, quadratic;

  void Register(LightManager &manager) {
    registration = manager.Register(this);
  }
};

template <>
struct JSONImpl<NPointLight> {
  static void Read(NPointLight &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(const NPointLight &value, JSON::Writer &writer);
};

class NDirectionalLight : public NLight {
  LightManager::DirectionalLightRegistration registration;

public:
  virtual void SetUniformData(const std::string &prefix) const override;

  Vec3 ambient = Vec3::one * 0.1f, diffuse, specular;

  void Register(LightManager &manager) {
    registration = manager.Register(this);
  }
};

template <>
struct JSONImpl<NDirectionalLight> {
  static void Read(NDirectionalLight &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(const NDirectionalLight &value, JSON::Writer &writer);
};

struct LightingConfig {
  LightManager::DirSizeType maxDirectionalLights;
  LightManager::PointSizeType maxPointLights;
};

#endif // _SCENE__LIGHT_MANAGER_H
