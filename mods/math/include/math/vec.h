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

#ifndef _MATH__VEC_H
#define _MATH__VEC_H

#include <iostream>
#include <cmath>
#include <core/readwrite.h>
#include <math/math.h>

template <typename T>
struct TVec2 {
  T x, y;

  TVec2() : x(0), y(0) {}
  TVec2(T _x, T _y) : x(_x), y(_y) {}
};

using IVec2 = TVec2<int>;
using UVec2 = TVec2<unsigned>;
using BVec2 = TVec2<bool>;

template <typename T>
inline std::ostream &operator<<(std::ostream &os, TVec2<T> v) {
  os << v.x << ',' << v.y;
  return os;
}

template <typename T>
struct TVec3 {
  T x, y, z;

  TVec3() : x(0), y(0), z(0) {}
  TVec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
};

using IVec3 = TVec3<int>;
using UVec3 = TVec3<unsigned>;
using BVec3 = TVec3<bool>;

template <typename T>
inline std::ostream &operator<<(std::ostream &os, TVec3<T> v) {
  os << v.x << ',' << v.y << ',' << v.z;
  return os;
}

template <typename T>
struct TVec4 {
  T x, y, z, w;

  TVec4() : x(0), y(0), z(0), w(0) {}
  TVec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
};

using IVec4 = TVec4<int>;
using UVec4 = TVec4<unsigned>;
using BVec4 = TVec4<bool>;

template <typename T>
inline std::ostream &operator<<(std::ostream &os, TVec4<T> v) {
  os << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
  return os;
}

struct Vec2 {
  float x, y;
  static const Vec2 up, down, left, right, one, zero;

  Vec2() : x(0.0f), y(0.0f) {}
  Vec2(float _x, float _y) : x(_x), y(_y) {}

  void Set(float _x, float _y)
    { x = _x; y = _y; }

  void Normalize() {
    float len = Length();
    if (len)
      *this /= len;
    else
      x = y = 0.0f;
  }

  Vec2 Normalized() const {
    float len = Length();
    if (len)
      return *this / len;
    else
      return Vec2::zero;
  }

  float SqrLength() const
    { return x * x + y * y; }
  float Length() const
    { return sqrtf(SqrLength()); }

  Vec2 Ortho() const
    { return Vec2(y, -x); }

  bool operator==(const Vec2 &r) const
    { return x == r.x && y == r.y; }
  bool operator!=(const Vec2 &r) const
    { return x != r.x || y != r.y; }

  Vec2 operator+(const Vec2 &r) const
    { return Vec2(x + r.x, y + r.y); }
  Vec2 operator+(float r) const
    { return Vec2(x + r, y + r); }
  Vec2 &operator+=(const Vec2 &r)
    { x += r.x; y += r.y; return *this; }
  Vec2 &operator+=(float r)
    { x += r; y += r; return *this; }

  Vec2 operator-() const
    { return Vec2(-x, -y); }
  Vec2 operator-(const Vec2 &r) const
    { return Vec2(x - r.x, y - r.y); }
  Vec2 operator-(float r) const
    { return Vec2(x - r, y - r); }
  Vec2 &operator-=(const Vec2 &r)
    { x -= r.x; y -= r.y; return *this; }
  Vec2 &operator-=(float r)
    { x -= r; y -= r; return *this; }

  Vec2 operator*(const Vec2 &r) const
    { return Vec2(x * r.x, y * r.y); }
  Vec2 operator*(float r) const
    { return Vec2(x * r, y * r); }
  Vec2 &operator*=(const Vec2 &r)
    { x *= r.x; y *= r.y; return *this; }
  Vec2 &operator*=(float r)
    { x *= r; y *= r; return *this; }

  Vec2 operator/(const Vec2 &r) const
    { return Vec2(x / r.x, y / r.y); }
  Vec2 operator/(float r) const
    { return Vec2(x / r, y / r); }
  Vec2 &operator/=(const Vec2 &r)
    { x /= r.x; y /= r.y; return *this; }
  Vec2 &operator/=(float r)
    { x /= r; y /= r; return *this; }

  static float Angle(Vec2 from, Vec2 to)
    { return atan2f(from.y - to.y, from.x - to.x); }

  static float SqrDistance(Vec2 a, Vec2 b) {
    float x = b.x - a.x;
    float y = b.y - a.y;
    return x * x + y * y;
  }

  static float Distance(Vec2 a, Vec2 b)
    { return sqrtf(SqrDistance(a, b)); }

  static float Dot(Vec2 a, Vec2 b)
    { return a.x * b.x + a.y * b.y; }

  static float Cross(Vec2 a, Vec2 b)
    { return a.x * b.y + a.y * b.x; }

  static Vec2 Min(Vec2 a, Vec2 b)
    { return Vec2(Math::Min(a.x, b.x), Math::Min(a.y, b.y)); }
  static Vec2 Max(Vec2 a, Vec2 b)
    { return Vec2(Math::Max(a.x, b.x), Math::Max(a.y, b.y)); }

  static Vec2 Lerp(Vec2 a, Vec2 b, float t)
    { return Vec2(Math::Lerp(a.x, b.x, t), Math::Lerp(a.y, b.y, t)); }

  static Vec2 MoveTowards(Vec2 a, Vec2 b, float maxDistance);

  static Vec2 Reflect(Vec2 direction, Vec2 normal)
    { return direction - normal * 2 * Dot(direction, normal); }
};

inline std::ostream &operator<<(std::ostream &os, Vec2 v) {
  os << v.x << ',' << v.y;
  return os;
}

struct Vec3 {
  float x, y, z;
  static const Vec3 up, down, left, right, front, back, one, zero;

  Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
  Vec3(float _x, float _y, float _z = 0.0f) : x(_x), y(_y), z(_z) {}
  Vec3(Vec2 val) : x(val.x), y(val.y), z(0.0f) {}

  void Set(float _x, float _y, float _z)
    { x = _x; y = _y; z = _z; }

  void Normalize() {
    float len = Length();
    if (len)
      *this /= len;
    else
      x = y = 0.0f;
  }

  Vec3 Normalized() const {
    float len = Length();
    if (len)
      return *this / len;
    else
      return Vec3::zero;
  }

  float SqrLength() const
    { return x * x + y * y + z * z; }
  float Length() const
    { return sqrtf(SqrLength()); }

  // TODO
  // Vec3 Ortho() const
  //   {}

  Vec3 Radians()
    { return Vec3(Math::Radians(x), Math::Radians(y), Math::Radians(z)); }

  Vec3 Degrees()
    { return Vec3(Math::Degrees(x), Math::Degrees(y), Math::Degrees(z)); }

  bool operator==(const Vec3 &r) const
    { return x == r.x && y == r.y && z == r.z; }
  bool operator!=(const Vec3 &r) const
    { return x != r.x || y != r.y || z != r.z; }

  Vec3 operator+(const Vec3 &r) const
    { return Vec3(x + r.x, y + r.y, z + r.z); }
  Vec3 operator+(float r) const
    { return Vec3(x + r, y + r, z + r); }
  Vec3 &operator+=(const Vec3 &r)
    { x += r.x; y += r.y; z += r.z; return *this; }
  Vec3 &operator+=(float r)
    { x += r; y += r; z += r; return *this; }

  Vec3 operator-() const
    { return Vec3(-x, -y, -z); }
  Vec3 operator-(const Vec3 &r) const
    { return Vec3(x - r.x, y - r.y, z - r.z); }
  Vec3 operator-(float r) const
    { return Vec3(x - r, y - r, z - r); }
  Vec3 &operator-=(const Vec3 &r)
    { x -= r.x; y -= r.y; z -= r.z; return *this; }
  Vec3 &operator-=(float r)
    { x -= r; y -= r; z -= r; return *this; }

  Vec3 operator*(const Vec3 &r) const
    { return Vec3(x * r.x, y * r.y, z * r.z); }
  Vec3 operator*(float r) const
    { return Vec3(x * r, y * r, z * r); }
  Vec3 &operator*=(const Vec3 &r)
    { x *= r.x; y *= r.y; z *= r.z; return *this; }
  Vec3 &operator*=(float r)
    { x *= r; y *= r; z *= r; return *this; }

  Vec3 operator/(const Vec3 &r) const
    { return Vec3(x / r.x, y / r.y, z / r.z); }
  Vec3 operator/(float r) const
    { return Vec3(x / r, y / r, z / r); }
  Vec3 &operator/=(const Vec3 &r)
    { x /= r.x; y /= r.y; z /= r.z; return *this; }
  Vec3 &operator/=(float r)
    { x /= r; y /= r; z /= r; return *this; }

  // TODO
  // static float Angle(Vec3 from, Vec3 to)
  //   {}

  static float SqrDistance(const Vec3 &a, const Vec3 &b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;
    return x * x + y * y + z * z;
  }

  static float Distance(const Vec3 &a, const Vec3 &b)
    { return sqrtf(SqrDistance(a, b)); }

  static float Dot(const Vec3 &a, const Vec3 &b)
    { return a.x * b.x + a.y * b.y + a.z * b.z; }

  static Vec3 Cross(const Vec3 &a, const Vec3 &b)
    { return Vec3(a.y * b.z - b.y * a.z, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y); }

  static Vec3 Min(const Vec3 &a, const Vec3 &b)
    { return Vec3((a.x < b.x) ? a.x : b.x, (a.y < b.y) ? a.y : b.y, (a.z < b.z) ? a.z : b.z); }
  static Vec3 Max(const Vec3 &a, const Vec3 &b)
    { return Vec3((a.x > b.x) ? a.x : b.x, (a.y > b.y) ? a.y : b.y, (a.z > b.z) ? a.z : b.z); }

  static Vec3 Lerp(const Vec3 &a, const Vec3 &b, float t)
    { return Vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t); }

  static Vec3 MoveTowards(const Vec3 &a, const Vec3 &b, float maxDistance);

  // TODO
  // static Vec3 Reflect(Vec3 direction, Vec3 normal)
  //   {}
};

inline std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
  os << v.x << ',' << v.y << ',' << v.z;
  return os;
}

struct Vec4 {
  float x, y, z, w;

  Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
  Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

inline std::ostream &operator<<(std::ostream &os, const Vec4 &v) {
  os << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
  return os;
}

template <>
struct JSONImpl<Vec2> {
  static void Read(Vec2 &out, const JSON::Value &value, const JSON::ReadData &data);
  static void Write(const Vec2 &value, JSON::Writer &writer);
};

template <>
struct JSONImpl<Vec3> {
  static void Read(Vec3 &out, const JSON::Value &value, const JSON::ReadData &data);
  static void Write(const Vec3 &value, JSON::Writer &writer);
};

template <>
struct JSONImpl<Vec4> {
  static void Read(Vec4 &out, const JSON::Value &value, const JSON::ReadData &data);
  static void Write(const Vec4 &value, JSON::Writer &writer);
};

#endif // _MATH__VEC_H
