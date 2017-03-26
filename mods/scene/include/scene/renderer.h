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

#include <tuple>
#include <list>
#include <functional>
#include <core/ref.h>
#include <scene/renderrequirements.h>

class Renderable;

using RenderTuple = std::tuple<std::function<void()>, Renderable *>;

struct RenderItem2 {
  Ref<Texture> same;
  std::list<RenderTuple> items;
};

struct RenderItem {
  Ref<Shader> same;
  std::list<RenderItem2> items;
};

struct RenderRegistration {
  RenderRegistration() {}
  friend class Renderer;

private:
  RenderRegistration(std::list<RenderItem>::iterator s, std::list<RenderItem2>::iterator t, std::list<RenderTuple>::iterator r)
    : shaderGroup(s), textureGroup(t), renderTuple(r) {}

  std::list<RenderItem>::iterator shaderGroup;
  std::list<RenderItem2>::iterator textureGroup;
  std::list<RenderTuple>::iterator renderTuple;
};

class Renderer {
  std::list<RenderItem> renderOrder;

public:
  static Ref<Renderer> active;

  // A single class instance SHOULD NOT register two functions with the same requirements!
  RenderRegistration Register(std::function<void()> func, Renderable *renderable, const RenderRequirements &r);

  void Unregister(const RenderRegistration &registration);

  RenderRegistration UpdateRequirements(const RenderRegistration &registration, const RenderRequirements &updated);

  void Render();
};

#endif // _SCENE__RENDERER_H
