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

#include <transform.h>
#include <node.h>

Mat4 Transform::Matrix() const {
  if (dirty) {
    dirty = false;
    matrix = Mat4::Translate(location) * rotation.Matrix() * Mat4::Scale(scale);
  }
  return matrix;
}

Transform Transform::operator*(const Transform &other) {
  Transform t;
  // Right hand side is assumed to be parent
  t.location = other.rotation * (location * other.scale) + other.location;
  t.rotation = rotation * other.rotation;
  t.scale = scale * other.scale;
  t.dirty = true;
  return t;
}

Transform &Transform::operator*=(const Transform &other) {
  // Right hand side is assumed to be parent
  location = other.rotation * (location * other.scale) + other.location;
  rotation = rotation * other.rotation;
  scale = scale * other.scale;
  dirty = true;
  return *this;
}

Vec3 Transform::GlobalLocation() const {
  Vec3 pos;
  const Transform *p = this;
  const NNode *par = Parent();
  while (par) {
    pos += par->transform.rotation * (p->location * par->transform.scale);
    p = &par->transform;
    par = par->transform.Parent();
  }
  return pos + p->location;
}

Quat Transform::GlobalRotation() const {
  Quat rot;
  const Transform *p = this;
  const NNode *par;
  do {
    rot *= p->rotation;
    par = p->Parent();
    if (par)
      p = &par->transform;
  } while (par);
  return rot;
}

Vec3 Transform::GlobalScale() const {
  Vec3 scale{1.0f, 1.0f, 1.0f};
  const Transform *p = this;
  const NNode *par;
  do {
    scale *= p->scale;
    par = p->Parent();
    if (par)
      p = &par->transform;
  } while (par);
  return scale;
}

// This function is equivalent to:
// return Transform(
//   GlobalLocation(),
//   GlobalRotation(),
//   GlobalScale()
// );
Transform Transform::GlobalTransform() const {
  Transform t;
  const Transform *p = this;
  const NNode *par;
  do {
    t *= *p;
    par = p->Parent();
    if (par)
      p = &par->transform;
  } while (par);
  return t;
}

void Transform::WriteToJSON(JSON::Writer &writer) const {
  writer.StartObject();
    writer.String("location", strlen("location"));
    location.WriteToJSON(writer);
    writer.String("rotation", strlen("rotation"));
    rotation.WriteToJSON(writer);
    writer.String("scale", strlen("scale"));
    scale.WriteToJSON(writer);

    writer.String("children", strlen("children"));
    writer.StartArray();
      for (auto &elem : children)
        elem->WriteToJSON(writer);
    writer.EndArray();
  writer.EndObject();
}

bool Transform::ReadFromJSON(const rapidjson::Value &data, JSON::TypeManager &/* manager */) {
  PARSE_CHECK(data.IsObject(), "Type 'Transform' must be an object")

  auto object = data.GetObject();
  PARSE_CHECK(object.HasMember("location"), "'Transform' object must have member 'location'")
  PARSE_CHECK(object.HasMember("rotation"), "'Transform' object must have member 'rotation'")
  
  bool res = true;
  if (object.HasMember("scale"))
    res = scale.ReadFromJSON(object["scale"]);
  else
    scale = Vec3::one;

  auto &loc = object["location"], &rot = object["rotation"];
  return location.ReadFromJSON(loc)
         && rotation.ReadFromJSON(rot)
         && res;
}
