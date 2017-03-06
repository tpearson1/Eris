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

#include <vec.h>

const Vec2 Vec2::up{0.0f, 1.0f};
const Vec2 Vec2::down{0.0f, -1.0f};
const Vec2 Vec2::left{-1.0f, 0.0f};
const Vec2 Vec2::right{1.0f, 0.0f};
const Vec2 Vec2::one{1.0f, 1.0f};
const Vec2 Vec2::zero{0.0f, 0.0f};

const Vec3 Vec3::up{0.0f, 1.0f, 0.0f};
const Vec3 Vec3::down{0.0f, -1.0f, 0.0f};
const Vec3 Vec3::left{-1.0f, 0.0f, 0.0f};
const Vec3 Vec3::right{1.0f, 0.0f, 0.0f};
const Vec3 Vec3::front{0.0f, 0.0f, 1.0f};
const Vec3 Vec3::back{0.0f, 0.0f, -1.0f};
const Vec3 Vec3::one{1.0f, 1.0f, 1.0f};
const Vec3 Vec3::zero{0.0f, 0.0f, 0.0f};

Vec2 Vec2::MoveTowards(Vec2 a, Vec2 b, float maxDistance) {
  float t = SqrDistance(a, b);
  if (t == 0.0f) return a;
  t = maxDistance / sqrtf(t);
  if (t < 0.0f)
    return Lerp(b, a, Math::Clamp(-t, 0.0f, 1.0f));
  return Lerp(a, b, Math::Clamp(t, 0.0f, 1.0f));
}

void Vec2::SerializeToJSON(Save::Writer &writer) const {
  writer.StartArray();
    writer.Double(static_cast<double>(x));
    writer.Double(static_cast<double>(y));
  writer.EndArray();
}

bool Vec2::LoadFromJSON(const rapidjson::Value &data) {
  PARSE_CHECK(data.IsArray(), "Type 'Vec2' must be an array")
  auto arr = data.GetArray();
  PARSE_CHECK(arr.Size() == 2 && arr[0].IsNumber() && arr[1].IsNumber(), "Vec2's array must contain 2 float values")
  x = arr[0].GetFloat();
  y = arr[1].GetFloat();
  return true;
}

Vec3 Vec3::MoveTowards(const Vec3 &a, const Vec3 &b, float maxDistance) {
  float t = SqrDistance(a, b);
  if (t == 0.0f) return a;
  t = maxDistance / sqrtf(t);
  if (t < 0.0f)
    return Lerp(b, a, Math::Clamp(-t, 0.0f, 1.0f));
  return Lerp(a, b, Math::Clamp(t, 0.0f, 1.0f));
}

void Vec3::SerializeToJSON(Save::Writer &writer) const {
  writer.StartArray();
    writer.Double(static_cast<double>(x));
    writer.Double(static_cast<double>(y));
    writer.Double(static_cast<double>(z));
  writer.EndArray();
}

bool Vec3::LoadFromJSON(const rapidjson::Value &data) {
  PARSE_CHECK(data.IsArray(), "Type 'Vec3' must be an array")
  auto arr = data.GetArray();
  PARSE_CHECK(
    arr.Size() == 3 && arr[0].IsNumber() && arr[1].IsNumber() && arr[2].IsNumber(),
    "Vec3's array must contain 3 float values"
  )
  x = arr[0].GetFloat();
  y = arr[1].GetFloat();
  z = arr[2].GetFloat();
  return true;
}

void Vec4::SerializeToJSON(Save::Writer &writer) const {
  writer.StartArray();
    writer.Double(static_cast<double>(x));
    writer.Double(static_cast<double>(y));
    writer.Double(static_cast<double>(z));
    writer.Double(static_cast<double>(w));
  writer.EndArray();
}

bool Vec4::LoadFromJSON(const rapidjson::Value &data) {
  PARSE_CHECK(data.IsArray(), "Type 'Vec4' must be an array")
  auto arr = data.GetArray();
  PARSE_CHECK(
    arr.Size() == 4 && arr[0].IsNumber() && arr[1].IsNumber() && arr[2].IsNumber() && arr[3].IsNumber(),
    "Vec4's array must contain 4 float values"
  )
  x = arr[0].GetFloat();
  y = arr[1].GetFloat();
  z = arr[2].GetFloat();
  w = arr[3].GetFloat();
  return true;
}
