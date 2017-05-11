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

void JSONImpl<Transform>::Write(const Transform &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
    JSON::WritePair("location", value.Location(), writer);
    JSON::WritePair("rotation", value.Rotation(), writer);
    JSON::WritePair("scale", value.Scale(), writer);

    JSON::Write<std::string>("children", writer);
    auto a = JSON::ArrayEncloser{writer};
      for (auto &elem : value)
        JSON::Write(*elem, writer);
}

void JSONImpl<Transform>::Read(Transform &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Vec3"};

  const auto &object = JSON::GetObject(value, data);
  JSON::GetMember(out.location, "location", object, data);
  JSON::GetMember(out.rotation, "rotation", object, data);
  JSON::TryGetMember(out.scale, "scale", object, Vec3::one, data);

  auto childrenIt = object.FindMember("children");
  if (childrenIt != object.MemberEnd()) {
    JSON::ParseAssert(childrenIt->value.IsArray(), data, "Member 'children' of 'Transform' must be of type array");

    const auto &childrenArr = childrenIt->value.GetArray();
    for (auto it = childrenArr.Begin(); it != childrenArr.End(); it++) {
      const auto type = JSON::Read<std::string>(*it, data);

      JSON::ParseAssert(++it != childrenArr.End(), data, "Array 'children' of 'Transform' must have data after each type string");

      auto func = data.typeManager->at(type);
      reinterpret_cast<NNode *>(func(*it, data))->transform.Parent(out.node);
    }
  }
}

