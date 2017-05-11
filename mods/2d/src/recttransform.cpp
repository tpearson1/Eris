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

#include <recttransform.h>
#include <cmath>
#include <base/window.h>
#include <canvasitem.h>

Mat4 RectTransform::Matrix() const {
  Vec3 loc;
  float rot = rotation;

  const RectTransform *curr = this;
  while (curr->Parent()) {
    const RectTransform *parent = &curr->Parent()->transform;
    loc += curr->location + parent->size * curr->anchors * 0.5f; // - curr->size * 0.5f;

    loc -= (curr->pivot + 0.5f) * curr->size;

    Vec2 piv = (curr->pivot - Vec2(0.5f, 0.5f)) * curr->size;
    loc += Vec3(
      piv.x * cosf(rotation) - piv.y * sinf(rotation),
      piv.x * sinf(rotation) - piv.y * cosf(rotation),
      0.0f
    );

    rot += parent->rotation;

    curr = parent;
  }

  // FIXME: Should be a 3x3 matrix as that is all that is needed for 2D
  // Efficient way of creating a 4x4 rotation matrix fr
  float cosRot = cosf(-rot);
  float sinRot = sinf(-rot);
  Mat4 rotMat{
    cosRot, -sinRot, 0.0f, 0.0f,
    sinRot, cosRot, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };

  return Mat4::Scale({2.0f / Window::inst->width, 2.0f / Window::inst->height, 1.0f})
         * rotMat
         * Mat4::Translate((loc * 2.0f) - size * 0.5f - Vec2(Window::inst->width, Window::inst->height) / 2.0f)
         * Mat4::Scale({size.x, size.y, 1.0f});
}

void RectTransform::WriteToJSON(JSON::Writer &writer) {
  writer.StartObject();
    writer.String("location", strlen("location"));
    location.WriteToJSON(writer);
    writer.String("pivot", strlen("pivot"));
    pivot.WriteToJSON(writer);
    writer.String("anchors", strlen("anchors"));
    anchors.WriteToJSON(writer);
    writer.String("size", strlen("size"));
    size.WriteToJSON(writer);
    writer.String("rotation-rads", strlen("rotation-rads"));
    writer.Double(static_cast<double>(rotation));

    writer.String("children", strlen("children"));
    writer.StartArray();
      for (auto &elem : children)
        elem->WriteToJSON(writer);
    writer.EndArray();
  writer.EndObject();
}

bool RectTransform::ReadFromJSON(const rapidjson::Value &data) {
  JSON::ParseAssert(data.IsObject(), "Type 'RectTransform' must be an object")

  auto object = data.GetObject();
  JSON::ParseAssert(object.HasMember("location"), "'RectTransform' object must have member 'location'")
  JSON::ParseAssert(object.HasMember("size"), "'RectTransform' object must have member 'size'")
  JSON::ParseAssert(object.HasMember("pivot"), "'RectTransform' object must have member 'pivot'")
  JSON::ParseAssert(object.HasMember("anchors"), "'RectTransform' object must have member 'anchors'")

  auto &loc = object["location"], &siz = object["size"],
       &piv = object["pivot"], &anchs = object["anchors"];

  if (object.HasMember("rotation-rads")) {
    auto &rot = object["rotation-rads"];
    JSON::ParseAssert(rot.IsFloat(), "Member 'rotation-rads' of 'RectTransform' must be of type float")
    rotation = rot.GetFloat();
  }
  else if (object.HasMember("rotation-degs")) {
    auto &rot = object["rotation-degs"];
    JSON::ParseAssert(rot.IsFloat(), "Member 'rotation-degs' of 'RectTransform' must be of type float")
    rotation = Math::Radians(rot.GetFloat());
  }
  else
    PARSE_ERROR("'RectTransform' object must have member 'rotation-rads' or 'rotation-degs'")

  return location.ReadFromJSON(loc)
         && pivot.ReadFromJSON(piv)
         && size.ReadFromJSON(siz)
         && anchors.ReadFromJSON(anchs);
}
