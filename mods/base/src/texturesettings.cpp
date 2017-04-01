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

#include <texturesettings.h>
#include <iostream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <core/file.h>

void TextureSettings::WriteToJSON(JSON::Writer &writer) const {
  writer.StartObject();

  writer.String("path", strlen("path"));
  writer.String(path.c_str(), path.size());

  const std::unordered_map<TextureType, std::string> textureTypes = {
    {TextureType::TEX_1D, "tex-1d"},
    {TextureType::TEX_2D, "tex-2d"},
    {TextureType::TEX_3D, "tex-3d"},
    {TextureType::RECTANGLE, "rectangle"},
    {TextureType::BUFFER, "buffer"},
    {TextureType::CUBEMAP, "cubemap"},
    {TextureType::TEX_1D_ARRAY, "tex-1d-array"},
    {TextureType::TEX_2D_ARRAY, "tex-2d-array"},
    {TextureType::CUBEMAP_ARRAY, "cubemap-array"},
    {TextureType::TEX_2D_MULTISAMPLE, "tex-2d-multisample"},
    {TextureType::TEX_2D_MULTISAMPLE_ARRAY, "tex-2d-multisample-array"}
  };
  
  writer.String("type", strlen("type"));
  auto str = textureTypes.at(type);
  writer.String(str.c_str(), str.size());
  
  const std::unordered_map<TextureShrinkType, std::string> shrinkTypes = {
    {TextureShrinkType::NEAREST_MIPMAP_NEAREST, "nearest-mipmap-nearest"},
    {TextureShrinkType::LINEAR_MIPMAP_NEAREST, "linear-mipmap-nearest"},
    {TextureShrinkType::NEAREST_MIPMAP_LINEAR, "nearest-mipmap-linear"},
    {TextureShrinkType::LINEAR_MIPMAP_LINEAR, "linear-mipmap-linear"},
    {TextureShrinkType::NEAREST, "nearest"},
    {TextureShrinkType::LINEAR, "linear"}
  };

  writer.String("shrink-filter", strlen("shrink-filter"));
  str = shrinkTypes.at(shrinkFilter);
  writer.String(str.c_str(), str.size());
  
  const std::unordered_map<TextureEnlargeType, std::string> enlargeTypes = {
    {TextureEnlargeType::NEAREST, "nearest"},
    {TextureEnlargeType::LINEAR, "linear"}
  };

  writer.String("enlarge-filter", strlen("enlarge-filter"));
  str = enlargeTypes.at(enlargeFilter);
  writer.String(str.c_str(), str.size());

  writer.EndObject();
}

bool TextureSettings::ReadFromJSON(const rapidjson::Value &value, JSON::TypeManager &/* manager */) {
  CHECK(value.IsObject(), "TextureSettings must be of type object")

  CHECK(value.HasMember("path"), "Member 'path' of TextureSettings is not present")
  const auto &texPath = value["path"];
  CHECK(value["path"].IsString(), "Member 'path' of TextureSettings is not present")

  CHECK(value.HasMember("type"), "Member 'type' of TextureSettings is not present")
  const auto &texType = value["type"];
  CHECK(value["type"].IsString(), "Member 'type' of TextureSettings must be of type string")

  CHECK(value.HasMember("shrink-filter"), "Member 'shrink-filter' of TextureSettings is not present")
  const auto &shrink = value["shrink-filter"];
  CHECK(value["shrink-filter"].IsString(), "'Member 'shrink-filter' of TextureSettings must be of type string")

  CHECK(value.HasMember("enlarge-filter"), "Member 'enlarge-filter' of TextureSettings is not present")
  const auto &enlarge = value["enlarge-filter"];
  CHECK(value["enlarge-filter"].IsString(), "Member 'enlarge-filter' of TextureSettings must be of type string")

  path = texPath.GetString();

  const std::unordered_map<std::string, TextureType> textureTypes = {
    {"tex-1d", TextureType::TEX_1D},
    {"tex-2d", TextureType::TEX_2D},
    {"tex-3d", TextureType::TEX_3D},
    {"rectangle", TextureType::RECTANGLE},
    {"buffer", TextureType::BUFFER},
    {"cubemap", TextureType::CUBEMAP},
    {"tex-1d-array", TextureType::TEX_1D_ARRAY},
    {"tex-2d-array", TextureType::TEX_2D_ARRAY},
    {"cubemap-array", TextureType::CUBEMAP_ARRAY},
    {"tex-2d-multisample", TextureType::TEX_2D_MULTISAMPLE},
    {"tex-2d-multisample-array", TextureType::TEX_2D_MULTISAMPLE_ARRAY}
  };

  auto typesIt = textureTypes.find(texType.GetString()); 
  if (typesIt == textureTypes.end())
    ERROR("Member 'type' of TextureSettings has invalid value")
  type = typesIt->second;

  const std::unordered_map<std::string, TextureShrinkType> shrinkTypes = {
    {"nearest-mipmap-nearest", TextureShrinkType::NEAREST_MIPMAP_NEAREST},
    {"linear-mipmap-nearest", TextureShrinkType::LINEAR_MIPMAP_NEAREST},
    {"nearest-mipmap-linear", TextureShrinkType::NEAREST_MIPMAP_LINEAR},
    {"linear-mipmap-linear", TextureShrinkType::LINEAR_MIPMAP_LINEAR},
    {"nearest", TextureShrinkType::NEAREST},
    {"linear", TextureShrinkType::LINEAR}
  };

  auto shrinkIt = shrinkTypes.find(shrink.GetString());
  if (shrinkIt == shrinkTypes.end())
    ERROR("Member 'shrink-filter' of TextureSettings has invalid value")
  shrinkFilter = shrinkIt->second;

  const std::unordered_map<std::string, TextureEnlargeType> enlargeTypes = {
    {"nearest", TextureEnlargeType::NEAREST},
    {"linear", TextureEnlargeType::LINEAR}
  };
  
  auto enlargeIt = enlargeTypes.find(enlarge.GetString());
  if (enlargeIt == enlargeTypes.end())
    ERROR("Member 'enlarge-filter' of TextureSettings has invalid value")
  enlargeFilter = enlargeIt->second;

  return true;
}
