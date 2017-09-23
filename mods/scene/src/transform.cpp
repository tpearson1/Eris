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

#include <node.h>
#include <transform.h>

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

Transform &Transform::operator*=(const Transform&other) {
  // Right hand side is assumed to be parent
  location = other.rotation * (location * other.scale) + other.location;
  rotation = rotation * other.rotation;
  scale = scale * other.scale;
  dirty = true;
  return *this;
}

void JSONImpl<Transform>::Write(const Transform &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
  JSON::WritePair("location", value.Location(), writer);
  JSON::WritePair("rotation", value.Rotation(), writer);
  JSON::WritePair("scale", value.Scale(), writer);
}

void JSONImpl<Transform>::Read(Transform &out, const JSON::Value &value,
                               const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Vec3"};

  const auto &object = JSON::GetObject(value, data);
  JSON::GetMember(out.location, "location", object, data);
  JSON::GetMember(out.rotation, "rotation", object, data);
  JSON::TryGetMember(out.scale, "scale", object, Vec3::one, data);
}
