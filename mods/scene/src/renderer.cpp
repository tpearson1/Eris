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

#include <base/shader.h>
#include <base/texture.h>
#include <iostream>
#include <renderdata.h>
#include <renderer.h>

Renderer *Renderer::active = nullptr;

void Renderer::Registration::UnregisterUnchecked() {
// A renderable object should not unregister itself during the render process
#ifndef NDEBUG
  assert(!renderer->currentlyRendering);
#endif
  auto shaderGroup = renderer->renderItems.find(shader.get());

  auto &renderPairs = shaderGroup->second;
  renderPairs.erase(*it);

  if (renderPairs.empty()) {
    // Need to remove the shader group because nothing is registered under this
    // shader anymore
    renderer->renderItems.erase(shaderGroup);
  }

  // Indicate that the registration is no longer registered
  it = nullptr;
}

void Renderer::Registration::CreateFrom(const Registration &other,
                                        RenderFunction newRenderFunc) {
  if (this == &other) return;
  if (Registered()) UnregisterUnchecked();

  other.renderer->Register(*this, other.shader);
  SetRenderData(other.GetRenderData());
  SetDrawFunction(newRenderFunc);
}

void Renderer::Registration::CreateFrom(Registration &&other,
                                        RenderFunction newRenderFunc) {
  if (this == &other) return;
  if (Registered()) UnregisterUnchecked();

  renderer = other.renderer;
  it = std::move(other.it);
  shader = std::move(other.shader);

  SetDrawFunction(newRenderFunc);
}

void Renderer::Registration::ChangeShader(
    const std::shared_ptr<Shader> &changed) {
  assert(it);
  if (changed == shader) return;

  // Add to new location
  auto &changedShaderGroup = renderer->renderItems[changed.get()];
  changedShaderGroup.emplace_front(GetDrawFunction(), GetRenderData());

  // Unregister the old data.
  // Checking unnecessary since 'it' was asserted to be non-null
  UnregisterUnchecked();
  // Change the iterator and the shader to represent the changes
  *it = changedShaderGroup.begin();
  shader = changed;
}

void Renderer::Register(Renderer::Registration &registration,
                        const std::shared_ptr<Shader> &s) {
  registration.SetRenderer(this);
  registration.shader = s;

  auto &shaderGroup = renderItems[s.get()];
  shaderGroup.emplace_front(nullptr, RenderData{});
  registration.SetRenderPairIterator(shaderGroup.begin());
}

void Renderer::Render() {
#ifndef NDEBUG
  currentlyRendering = true;
#endif
  for (auto &group : renderItems) {
    auto shader = group.first;
    const auto &list = group.second;
    assert(shader);
    shader->Use();
    for (auto pair : list) {
      auto renderData = pair.second;
      auto renderFunc = pair.first;

      assert(renderFunc);
      if (renderData.visible) renderFunc();
    }
  }
#ifndef NDEBUG
  currentlyRendering = false;
#endif
}
