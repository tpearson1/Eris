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

Renderer *Renderer::nodes, *Renderer::canvas;

void Renderer::Register(std::function<void()> func, Renderable *node, const RenderRequirements &r) {
  for (auto &shader : renderOrder) {
    if (shader.same == r.shader) {
      // Same shader
      for (auto &texture : shader.items) {
        if (texture.same == r.texture) {
          // Same texture too. Insert node into this list
          texture.items.push_front({func, node});
          return;
        }
      }
      // Texture wasn't found
      RenderItem2 t;
      t.same = r.texture;
      t.items.push_front({func, node});
      shader.items.push_front(t);
      return;
    }
  }
  // Shader wasn't found
  RenderItem s;
  s.same = r.shader;

  RenderItem2 st;
  st.same = r.texture;
  st.items.push_front({func, node});
  s.items.push_front(st);

  renderOrder.push_front(s);
}

static std::function<void()> *RemoveItem(std::forward_list<RenderTuple> &items, Renderable *value) {
  auto it = items.before_begin(), nit = std::next(it);
  while (nit != items.end()) {
    if (std::get<1>(*nit) == value) {
      items.erase_after(it);
      return &std::get<0>(*nit);
    }
    it = nit++;
  }

  return nullptr;
}

void Renderer::Unregister(Renderable *node, const RenderRequirements &r) {
  for (auto &shader : renderOrder) {
    if (shader.same == r.shader) {
      // Same shader
      for (auto &texture : shader.items) {
        if (texture.same == r.texture) {
          // Same texture
          RemoveItem(texture.items, node);
        }
      }
    }
  }
}

void Renderer::UpdateRequirements(Renderable *node, const RenderRequirements &cur, const RenderRequirements &old) {
  if (cur == old)
    return;

  std::function<void()> *oldFunc;
  for (auto &shader : renderOrder) {
    if (shader.same == old.shader) {
      // Same shader
      for (auto &texture : shader.items) {
        if (texture.same == old.texture) {
          // Same texture
          oldFunc = RemoveItem(texture.items, node);
        }
      }

      if (cur.shader == old.shader) {
        // Just the texture
        for (auto &texture : shader.items) {
          if (cur.texture == texture.same) {
            texture.items.push_front({*oldFunc, node});
            return;
          }
        }
        // Wasn't found
        RenderItem2 st;
        st.same = cur.texture;
        st.items.push_front({*oldFunc, node});
        shader.items.push_front(st);
        return;
      }
    }
  }
  // Shader wasn't found
  RenderItem s;
  s.same = cur.shader;

  RenderItem2 st;
  st.same = cur.texture;
  st.items.push_front({*oldFunc, node});

  s.items.push_front(st);
  renderOrder.push_front(s);
}

void Renderer::Render() {
  for (auto shaderGroup : renderOrder) {
    if (shaderGroup.same)
      shaderGroup.same->Use();
    for (auto textureGroup : shaderGroup.items) {
      if (textureGroup.same) {
        GLint textureSampler = shaderGroup.same->GetUniform("textureSampler");
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
