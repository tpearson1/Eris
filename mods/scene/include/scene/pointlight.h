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

#ifndef _SCENE__POINT_LIGHT_H
#define _SCENE__POINT_LIGHT_H

#include <scene/light.h>

#include <scene/lightmanager.h>

class NPointLight : public NLight {
  LightManager::PointLightRegistration registration;

public:
  void SetUniformData(const std::string &prefix) const;

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

#endif // _SCENE__POINT_LIGHT_H
