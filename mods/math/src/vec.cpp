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

void JSONImpl<Vec2>::Write(const Vec2 &value, JSON::Writer &writer) {
  auto a = JSON::ArrayEncloser{writer};
  JSON::Write(value.x, writer);
  JSON::Write(value.y, writer);
}

void JSONImpl<Vec2>::Read(Vec2 &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Vec2"};
  auto array = JSON::Read<std::array<float, 2>>(value, data);
  out = Vec2{array[0], array[1]};
}

Vec3 Vec3::MoveTowards(const Vec3 &a, const Vec3 &b, float maxDistance) {
  float t = SqrDistance(a, b);
  if (t == 0.0f) return a;
  t = maxDistance / sqrtf(t);
  if (t < 0.0f)
    return Lerp(b, a, Math::Clamp(-t, 0.0f, 1.0f));
  return Lerp(a, b, Math::Clamp(t, 0.0f, 1.0f));
}

void JSONImpl<Vec3>::Write(const Vec3 &value, JSON::Writer &writer) {
  auto a = JSON::ArrayEncloser{writer};
  JSON::Write(value.x, writer);
  JSON::Write(value.y, writer);
  JSON::Write(value.z, writer);
}

void JSONImpl<Vec3>::Read(Vec3 &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Vec3"};
  auto array = JSON::Read<std::array<float, 3>>(value, data);
  out = Vec3{array[0], array[1], array[2]};
}

void JSONImpl<Vec4>::Write(const Vec4 &value, JSON::Writer &writer) {
  auto a = JSON::ArrayEncloser{writer};
  JSON::Write(value.x, writer);
  JSON::Write(value.y, writer);
  JSON::Write(value.z, writer);
  JSON::Write(value.w, writer);
}

void JSONImpl<Vec4>::Read(Vec4 &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Vec4"};
  auto array = JSON::Read<std::array<float, 4>>(value, data);
  out = Vec4{array[0], array[1], array[2], array[3]};
}

