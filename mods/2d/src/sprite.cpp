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

#include <sprite.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <base/image.h>
#include <core/file.h>

std::vector<GLfloat> Sprite::TexCoords() const {
  float widthRatio = 1.0f / sheet->Tex().Width();
  float heightRatio = 1.0f / sheet->Tex().Height();
  float x1 = region.x1 * widthRatio;
  float y1 = region.y1 * heightRatio;
  float x2 = region.x2 * widthRatio;
  float y2 = region.y2 * heightRatio;
  return std::vector<GLfloat>{
    x1, y1,
    x2, y1,
    x1, y2,
    x2, y2
  };
}

#define FAIL_IF(expr, failMessage) {\
if (expr) {\
  std::cerr << "JSON: Sprite sheet at '" << path << "' has error: " << (failMessage) << '\n';\
  return false;\
}}

#define CHECK(expr, failMessage) FAIL_IF(!(expr), failMessage)

bool SpriteSheet::Load(const std::string &imagePath) {
  std::string path(imagePath);
  // Only files ending with '.json' should be used. Files should use the following format:
  // foo.png.json, specifying metadata for the foo.png sprite.
  // Therefore we can do the following:
  if (!texture.Load(imagePath))
    return false;

  path.append(".json");
  std::string jsonText;
  File::Read(path, jsonText);

  rapidjson::Document document;
  document.Parse<rapidjson::kParseCommentsFlag
                | rapidjson::kParseTrailingCommasFlag
                | rapidjson::kParseNanAndInfFlag>(jsonText.c_str());

  CHECK(document.IsObject(), "There is no root object")

  CHECK(document.HasMember("sprites"), "'sprites' member must be present")
  CHECK(document["sprites"].IsArray(), "'sprites' member must be of type array")

  const auto &json = document["sprites"];
  FAIL_IF(json.Size() == 0, "no sprites are listed inside member 'sprites'")

  for (rapidjson::SizeType i = 0; i < json.Size(); i++) {
    const auto &sprite = json[i];
    CHECK(sprite.IsObject(), "All elements of 'sprites' array must be of type object")

    CHECK(sprite.HasMember("name"), "All 'sprites' array elements must have 'name' element")
    CHECK(sprite["name"].IsString(), "All 'sprites' array 'name' elements must be of type string")

    CHECK(sprite.HasMember("rect"), "All 'sprites' array elements must have 'rect' element")
    const auto &rect = sprite["rect"];
    CHECK(rect.IsArray() && rect.Size() == 4, "All 'sprites' array 'rect' elements must be of type array with size 4")

    for (rapidjson::SizeType j = 0; j < 4; j++)
      CHECK(rect[j].IsUint(), "All 'sprites' array 'rect' element arrays must have all children of type uint")

    Rect b = Rect(rect[0].GetUint(),
             rect[1].GetUint(),
             rect[2].GetUint(),
             rect[3].GetUint());
    AddSprite(sprite["name"].GetString(), b);
  }

  return true;
}
