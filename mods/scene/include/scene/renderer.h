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

#ifndef _SCENE__RENDERER_H
#define _SCENE__RENDERER_H

#include <base/shader.h>
#include <functional>
#include <list>
#include <memory>
#include <scene/renderdata.h>
#include <utility>

class Renderer {
  using RenderFunction = std::function<void()>;
  using RenderPair = std::pair<RenderFunction, RenderData>;

  std::unordered_map<const Shader *, std::list<RenderPair>> renderItems;

public:
  class Registration {
    std::shared_ptr<const Shader> shader;
    using It = typename std::list<RenderPair>::iterator;
    std::unique_ptr<It> it;
    Renderer *renderer;

    void SetRenderer(Renderer *r) { renderer = r; }
    void SetRenderPairIterator(It _it) { it = std::make_unique<It>(_it); }

    void UnregisterUnchecked();

  public:
    Registration() = default;

    void CreateFrom(const Registration &other, RenderFunction newRenderFunc);
    void CreateFrom(Registration &&other, RenderFunction newRenderFunc);

    Registration(const Registration &) = delete;
    Registration &operator=(const Registration &) = delete;

    std::function<void()> GetDrawFunction() const {
      assert(Registered());
      return (*it)->first;
    }

    void SetDrawFunction(std::function<void()> func) {
      assert(Registered());
      (*it)->first = std::move(func);
    }

    const RenderData &GetRenderData() const {
      assert(Registered());
      return (*it)->second;
    }

    void SetRenderData(const RenderData &changed) {
      assert(Registered());
      (*it)->second = changed;
    }

    std::shared_ptr<const Shader> GetShader() const { return shader; }
    void ChangeShader(const std::shared_ptr<const Shader> &changed);

    bool Registered() const { return static_cast<bool>(it); }

    void Unregister() {
      assert(Registered());
      UnregisterUnchecked();
    }

    ~Registration() {
      if (Registered()) UnregisterUnchecked();
    }

    friend class Renderer;
  };

  static Renderer *active;

  // A single class instance SHOULD NOT register two functions with the same
  // RenderData object!
  void Register(Registration &registration,
                const std::shared_ptr<const Shader> &s);

  void Render();
};

#endif // _SCENE__RENDERER_H
