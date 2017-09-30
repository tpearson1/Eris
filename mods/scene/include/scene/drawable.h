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

#ifndef _SCENE__DRAWABLE_H
#define _SCENE__DRAWABLE_H

#include <scene/renderdata.h>
#include <scene/renderer.h>

class Shader;

class Drawable {
  Renderer::Registration registration;

public:
  Drawable() = default;
  Drawable(const std::shared_ptr<const Shader> &s) { Register(s); }

  void Register(const std::shared_ptr<const Shader> &s);

  Drawable(const Drawable &other);
  Drawable &operator=(const Drawable &other);

  Drawable(Drawable &&other);
  Drawable &operator=(Drawable &&other);

  bool GetVisible() const {
    return registration.GetRenderData().visible;
  }

  void SetVisible(bool visible) { registration.SetRenderData({visible}); }

  std::shared_ptr<const Shader> GetShader() const {
    return registration.GetShader();
  };

  void SetShader(const std::shared_ptr<const Shader> &s) {
    registration.ChangeShader(s);
  }

  virtual void Draw() const = 0;
};

#endif // _SCENE__DRAWABLE_H
