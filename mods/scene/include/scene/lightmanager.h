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

struct LightingConfig {
  LightManager::DirSizeType maxDirectionalLights;
  LightManager::PointSizeType maxPointLights;
};

#endif // _SCENE__LIGHT_MANAGER_H
