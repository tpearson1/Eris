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
#include <core/ref.h>
#include <scene/node.h>

struct Light : public NNode {
  virtual void SetUniformData(const std::string &prefix) const = 0;
};

struct PointLight : public Light {
  virtual void SetUniformData(const std::string &prefix) const override;

  Vec3 ambient = Vec3::one * 0.1f, diffuse = Vec3::one * 1.0f, specular = Vec3::one * 0.6f;
  float constant = 1.0f, linear, quadratic;
};

class DirectionalLight : public Light {
public:
  virtual void SetUniformData(const std::string &prefix) const override;

  Vec3 ambient = Vec3::one * 0.1f, diffuse = Vec3::one * 1.0f, specular = Vec3::one * 0.6f;
};

class LightManager {
  std::list<PointLight *> pointLights;
  std::list<DirectionalLight *> directionalLights;

public:
  auto RegisterPointLight(PointLight *light)
    { pointLights.push_front(light); return pointLights.front(); }

  auto RegisterDirectionalLight(DirectionalLight *light)
    { directionalLights.push_front(light); return directionalLights.front(); }

  void Unregister(std::list<PointLight *>::iterator it)
    { pointLights.erase(it); }
  void Unregister(std::list<DirectionalLight *>::iterator it)
    { directionalLights.erase(it); }

  using PointSizeType = std::list<PointLight *>::size_type;
  using DirSizeType = std::list<DirectionalLight *>::size_type;
  void SetUniformsForClosestLights(Vec3 location, DirSizeType maxDirCount, PointSizeType maxPointCount);
};

#endif // _SCENE__LIGHT_MANAGER_H

