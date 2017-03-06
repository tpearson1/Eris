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

TextureSettings TextureSettings::nearest{
  TextureType::TEX_2D,
  TextureShrinkType::NEAREST,
  TextureEnlargeType::NEAREST
};

TextureSettings TextureSettings::linear{
  TextureType::TEX_2D,
  TextureShrinkType::LINEAR,
  TextureEnlargeType::LINEAR
};

#define CHECK(expr, failMessage) {\
if (!(expr)) {\
  std::cerr << "> JSON: Texture '" << path << "' error: " << (failMessage) << '\n';\
  return false;\
}}

#define ERROR(message)\
{ std::cerr << "> JSON: Texture '" << path << "' error: " << (message) << '\n'; return false; }

bool TextureSettings::FromJSON(const std::string &path) {
  std::string text;
  File::Read(path, text);
  rapidjson::Document document;
  document.Parse<rapidjson::kParseCommentsFlag
                | rapidjson::kParseTrailingCommasFlag
                | rapidjson::kParseNanAndInfFlag>(text.c_str());

  // Error checking
  CHECK(document.IsObject(), "There is no root object")

  CHECK(document.HasMember("texture"), "'texture' member must be present")
  CHECK(document["texture"].IsObject(), "'texture' member must be of type object")

  const rapidjson::Value &json = document["texture"];

  if (json.HasMember("use-preset")) {
    CHECK(json["use-preset"].IsString(), "'use-preset' member must be of type string")

    std::string preset = json["use-preset"].GetString();
    if (preset == "nearest") *this = nearest;
    else if (preset == "linear") *this = linear;
    else ERROR("'use-preset' member is not a valid preset")
  }
  else {
    CHECK(json.HasMember("type"), "'type' member must be included if 'use-preset' member isn't present")
    CHECK(json.HasMember("shrink-filter"), "'shrink-filter' member must be included if 'use-preset' member isn't present")
    CHECK(json.HasMember("enlarge-filter"), "'enlarge-filter' member must be included if 'use-preset' member isn't present")
  }

  if (json.HasMember("type")) {
    CHECK(json["type"].IsString(), "'type' member must be of type string")
    std::string typeString = json["type"].GetString();
    if (typeString == "tex-1d") type = TextureType::TEX_1D;
    else if (typeString == "tex-2d") type = TextureType::TEX_2D;
    else if (typeString == "tex-3d") type = TextureType::TEX_3D;
    else if (typeString == "rectangle") type = TextureType::RECTANGLE;
    else if (typeString == "buffer") type = TextureType::BUFFER;
    else if (typeString == "cubemap") type = TextureType::CUBEMAP;
    else if (typeString == "tex-1d-array") type = TextureType::TEX_1D_ARRAY;
    else if (typeString == "tex-2d-array") type = TextureType::TEX_2D_ARRAY;
    else if (typeString == "cubemap-array") type = TextureType::CUBEMAP_ARRAY;
    else if (typeString == "tex-2d-multisample") type = TextureType::TEX_2D_MULTISAMPLE;
    else if (typeString == "tex-2d-multisample-array") type = TextureType::TEX_2D_MULTISAMPLE_ARRAY;
    else ERROR("Member 'type' has invalid value")
  }

  if (json.HasMember("shrink-filter")) {
    CHECK(json["shrink-filter"].IsString(), "'shrink-filter' member must be of type string")
    std::string shrinkString = json["shrink-filter"].GetString();
    if (shrinkString == "nearest-mipmap-nearest") shrinkFilter = TextureShrinkType::NEAREST_MIPMAP_NEAREST;
    else if (shrinkString == "linear-mipmap-nearest") shrinkFilter = TextureShrinkType::LINEAR_MIPMAP_NEAREST;
    else if (shrinkString == "nearest-mipmap-linear") shrinkFilter = TextureShrinkType::NEAREST_MIPMAP_LINEAR;
    else if (shrinkString == "linear-mipmap-linear") shrinkFilter = TextureShrinkType::LINEAR_MIPMAP_LINEAR;
    else if (shrinkString == "nearest") shrinkFilter = TextureShrinkType::NEAREST;
    else if (shrinkString == "linear") shrinkFilter = TextureShrinkType::LINEAR;
    else ERROR("Member 'shrink-filter' has invalid value")
  }

  if (json.HasMember("enlarge-filter")) {
    CHECK(json["enlarge-filter"].IsString(), "'enlarge-filter' member must be of type string")
    std::string enlargeString = json["enlarge-filter"].GetString();
    if (enlargeString == "nearest") enlargeFilter = TextureEnlargeType::NEAREST;
    else if (enlargeString == "linear") enlargeFilter = TextureEnlargeType::LINEAR;
    else ERROR("Member 'enlarge-filter' has invalid value")
  }

  return true;
}
