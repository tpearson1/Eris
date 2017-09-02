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

#include <renderer.h>
#include <iostream>
#include <base/shader.h>
#include <base/texture.h>
#include <renderable.h>

Renderer *Renderer::active = nullptr;

Renderer::Registration Renderer::Register(std::function<void()> func, Renderable *renderable, const std::shared_ptr<const Shader> &s) {
  auto &group = renderItems[s];
  group.emplace_front(func, renderable);
  return {s, group.begin()};
}

void Renderer::Unregister(const Renderer::Registration &registration) {
  auto groupIt = renderItems.find(registration.shader);
  if (groupIt == std::end(renderItems)) {
    // Must have already been unregistered because Registration object can
    // only be created by Renderer
    std::cerr << "Renderable object already unregistered\n";
    return;
  }

  auto &group = groupIt->second;
  group.erase(registration.element);

  if (group.empty()) {
    renderItems.erase(groupIt);
    return;
  }
}

Renderer::Registration
Renderer::UpdateRequirements(const Renderer::Registration &registration,
                             const std::shared_ptr<const Shader> &updated) {
  const auto &renderTuple = *registration.element;
  auto reg = Register(renderTuple.first, renderTuple.second, updated);
  Unregister(registration);
  return reg;
}

void Renderer::Render() {
  for (auto group : renderItems) {
    const auto &shader = group.first;
    const auto &list = group.second;
    // Shader should be valid because it was when registered
    shader->Use();
    for (auto pair : list) {
      auto renderable = pair.second;
      auto renderFunc = pair.first;
      if (renderable->visible)
        renderFunc();
    }
  }
}

