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

#ifndef _MATH__QUAT_H
#define _MATH__QUAT_H

#include <cmath>
#include <math/vec.h>
#include <math/math.h>
#include <math/mat.h>

struct Quat {
  float x, y, z, w;
  static const Quat identity;

  Quat() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
  Quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
  Quat(Vec3 euler) { SetDeg(euler); }
  Quat(Mat4 mat);

  void Set(float _x, float _y, float _z, float _w)
    { x = _x; y = _y; z = _z; w = _w; }

  void SetRad(Vec3 angles)
    { return SetRad(angles.x, angles.y, angles.z); }
  void SetRad(float _x, float _y, float _z);

  void SetDeg(Vec3 euler)
    { return SetDeg(euler.x, euler.y, euler.z); }
  void SetDeg(float _x, float _y, float _z)
    { SetRad(Math::Radians(_x), Math::Radians(_y), Math::Radians(_z)); }

  Quat Inverse() const {
    Quat inv;
    float tmp = SqrLength();
    float mul = 1.0f / tmp;
    inv.x = -x * mul;
    inv.y = -y * mul;
    inv.z = -z * mul;
    inv.w = w * mul;
    return inv;
  }

  Mat4 Matrix() const;

  float SqrLength() const
    { return x * x + y * y + z * z + w * w; }
  float Length() const
    { return sqrtf(SqrLength()); }

  static float Dot(Quat a, Quat b)
    { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

  static Quat Lerp(Quat a, Quat b, float t);

  bool operator==(const Quat &r) const
    { return x == r.x && y == r.y && z == r.z && w == r.w; }
  bool operator!=(const Quat &r) const
    { return x != r.x || y != r.y || z != r.z || w != r.w; }

  Quat operator*(const Quat &r) const;
  Vec3 operator*(const Vec3 &r) const;
  Quat &operator*=(const Quat &r) {
    *this = *this * r;
    return *this;
  }

  void WriteToJSON(JSON::Writer &writer) const;

  bool ReadFromJSON(const rapidjson::Value &data);
};

inline std::ostream &operator<<(std::ostream &os, Quat q) {
  os << q.x << ',' << q.y << ',' << q.z << ',' << q.w;
  return os;
}

#endif // _MATH__QUAT_H
