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

#include <renderrequirements.h>
#include <base/resources.h>

bool RenderRequirements::ReadFromJSON(const rapidjson::Value &data, JSON::TypeManager &/* manager */) {
  PARSE_CHECK(data.IsObject(), "RenderRequirements must be of type object")
  const auto &object = data.GetObject();

  PARSE_CHECK(object.HasMember("shader"), "RenderRequirements object must have member 'shader'")
  const auto &shaderObj = object["shader"];
  PARSE_CHECK(shaderObj.IsString(), "Member 'shader' of RenderRequirements must be of type string")
 
  PARSE_CHECK(object.HasMember("texture"), "RenderRequirements object must have member 'texture'")
  const auto &textureObj = object["texture"];
  PARSE_CHECK(textureObj.IsString(), "Member 'texture' of RenderRequirements must be of type string")

  shader = Resources::active->shaders.Get(shaderObj.GetString());
  texture = Resources::active->textures.Get(textureObj.GetString());

  return true;
}
