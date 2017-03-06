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

#ifndef _BASE__RESOURCE_MANAGER_H
#define _BASE__RESOURCE_MANAGER_H

#include <unordered_map>
#include <base/ref.h>
#include <base/texture.h>
#include <base/shader.h>

template <typename T, typename Key = std::string, typename Hash = std::hash<Key>>
class ResourceManager {
  std::unordered_map<Key, Ref<T>, Hash> resources;

public:
  Ref<T> Get(const Key &key) const {
    auto val = resources.find(key);
    if (val == resources.end())
      return Ref<T>(); // nullptr
    else
      return val->second;
  }

  void Add(const Key &key, const Ref<T> &value)
    { resources[key] = value; }
};

struct HashShaderTuple {
  size_t operator()(const std::tuple<std::string, std::string> &t) const {
    size_t seed = std::hash<std::string>()(std::get<0>(t)) + 0x9e3779b9;
    return seed ^ (std::hash<std::string>()(std::get<1>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
  }
};

struct Resources {
  ResourceManager<Texture> textures;
  // Key is tuple of vertex shader and fragment shader paths
  ResourceManager<Shader, std::tuple<std::string, std::string>, HashShaderTuple> shaders;

  static Ref<Resources> active;
};

#endif // _BASE__RESOURCE_MANAGER_H
