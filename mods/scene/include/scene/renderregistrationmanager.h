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

#ifndef _SCENE__RENDER_REGISTRATION_MANAGER_H
#define _SCENE__RENDER_REGISTRATION_MANAGER_H

#include <scene/renderdata.h>
#include <scene/renderer.h>

class Shader;

class RenderRegistrationManager {
  std::shared_ptr<const Shader> shader;
  Renderer::Registration renderRegistration;
  RenderData renderData;

  void Register(const std::shared_ptr<const Shader> &s) {
    renderRegistration =
        Renderer::active->Register([this] { Draw(); }, &renderData, shader = s);
  }

public:
  RenderRegistrationManager(const std::shared_ptr<const Shader> &s) {
    assert(Renderer::active);
    Register(s);
  }

  RenderRegistrationManager(const RenderRegistrationManager &other);

  RenderRegistrationManager &operator=(const RenderRegistrationManager &other);

  ~RenderRegistrationManager() {
    Renderer::active->Unregister(renderRegistration);
  }

  void SetShader(const std::shared_ptr<const Shader> &s) {
    assert(Renderer::active);
    Renderer::active->UpdateRequirements(renderRegistration, shader = s);
  }

  const Shader *GetShader() const { return shader.get(); };

  virtual void Draw() const = 0;
};

#endif // _SCENE__RENDER_REGISTRATION_MANAGER_H
