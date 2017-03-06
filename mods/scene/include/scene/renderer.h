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
#include <forward_list>
#include <functional>
#include <scene/renderrequirements.h>

class Renderable;

using RenderTuple = std::tuple<std::function<void()>, Renderable *>;

struct RenderItem2 {
  Ref<Texture> same;
  std::forward_list<RenderTuple> items;
};

struct RenderItem {
  Ref<Shader> same;
  std::forward_list<RenderItem2> items;
};

class Renderer {
  std::forward_list<RenderItem> renderOrder;

public:
  // A single class instance SHOULD NOT register two functions with the same requirements!
  void Register(std::function<void()> func, Renderable *node, const RenderRequirements &r);

  void Unregister(Renderable *node, const RenderRequirements &r);

  static Renderer *nodes, *canvas;

  void UpdateRequirements(Renderable *node, const RenderRequirements &cur, const RenderRequirements &old);

  void Render();
};

#endif // _SCENE__RENDERER_H
