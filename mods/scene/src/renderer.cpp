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

RenderRegistration Renderer::Register(std::function<void()> func, Renderable *renderable, const RenderRequirements &r) {
  for (auto shaderIt = renderOrder.begin(); shaderIt != renderOrder.end(); shaderIt++) {
    if (shaderIt->same == r.shader) {
      // Same shader
      auto &shaderItems = shaderIt->items;
      for (auto textureIt = shaderItems.begin(); textureIt != shaderItems.end(); textureIt++) {
        if (textureIt->same == r.texture) {
          // Same texture too. Insert node into this list
          textureIt->items.push_front({func, renderable});
          return {shaderIt, textureIt, textureIt->items.begin()};
        }
      }
      // Texture wasn't found
      RenderItem2 t;
      t.same = r.texture;
      t.items.push_front({func, renderable});
      auto renderTuple = t.items.begin();
      shaderIt->items.push_front(t);
      return {shaderIt, shaderIt->items.begin(), renderTuple};
    }
  }
  // Shader wasn't found
  RenderItem s;
  s.same = r.shader;

  RenderItem2 st;
  st.same = r.texture;
  st.items.push_front({func, renderable});
  auto renderTuple = st.items.begin();

  s.items.push_front(st);
  auto textureIt = s.items.begin();

  renderOrder.push_front(s);
  return {renderOrder.begin(), textureIt, renderTuple};
}

void Renderer::Unregister(const RenderRegistration &registration) {
  registration.textureGroup->items.erase(registration.renderTuple);
  if (registration.textureGroup->items.empty()) {
    registration.shaderGroup->items.erase(registration.textureGroup);
    if (registration.shaderGroup->items.empty())
      renderOrder.erase(registration.shaderGroup);
  }
}

RenderRegistration Renderer::UpdateRequirements(const RenderRegistration &registration, const RenderRequirements &updated) {
  auto &renderTuple = *registration.renderTuple;
  auto reg = Register(std::get<0>(renderTuple), std::get<1>(renderTuple), updated);
  Unregister(registration);
  return reg;
}

void Renderer::Render() {
  for (auto shaderGroup : renderOrder) {
    if (shaderGroup.same)
      shaderGroup.same->Use();
    for (auto textureGroup : shaderGroup.items) {
      if (textureGroup.same) {
        auto textureSampler = shaderGroup.same->GetUniform("material.diffuse");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureGroup.same->ID());
        Shader::SetUniform(textureSampler, 0);
      }

      for (auto elem : textureGroup.items) {
        auto node = std::get<1>(elem);
        if (node->visible)
          std::get<0>(elem)();
      }
    }
  }
}
