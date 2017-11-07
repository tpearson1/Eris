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

#include <meshconfig.h>
#include <test/macros.h>

std::unordered_map<std::string, MeshRenderConfigs::Generator>
    MeshRenderConfigs::configurationGenerators;

void JSONImpl<MeshRenderConfigs::NamedTexturePair>::Read(
    MeshRenderConfigs::NamedTexturePair &out, const JSON::Value &value,
    const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "MeshRenderConfigs::NamedTexturePair"};
  const auto &object = JSON::GetObject(value, data);
  JSON::GetMember(out.uniform, "uniform", object, data);
  auto textureStr = JSON::GetMember<std::string>("texture", object, data);
  out.texture = Resources::active->textures.Get(textureStr);
}
