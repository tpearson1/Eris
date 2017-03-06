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

#ifndef _MATH__MAT_H
#define _MATH__MAT_H

#include <cstddef>
#include <math/vec.h>

class Mat2 {
  float value[2][2];

public:
  Mat2() : value{{0.0f, 0.0f}, {0.0f, 0.0f}} {}
  Mat2(float _00, float _01, float _10, float _11) : value{{_00, _01}, {_10, _11}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat3 {
  float value[3][3];

public:
  Mat3() : value{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}} {}
  Mat3(float _00, float _01, float _02,
       float _10, float _11, float _12,
       float _20, float _21, float _22)
       : value{{_00, _01, _02}, {_10, _11, _12}, {_20, _21, _22}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat2x3 {
  float value[2][3];

public:
  Mat2x3() : value{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}} {}
  Mat2x3(float _00, float _01, float _02, float _10, float _11, float _12)
         : value{{_00, _01, _02}, {_10, _11, _12}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat3x2 {
  float value[3][2];

public:
  Mat3x2() : value{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}} {}
  Mat3x2(float _00, float _01, float _10, float _11, float _20, float _21)
         : value{{_00, _01}, {_10, _11}, {_20, _21}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat2x4 {
  float value[2][4];

public:
  Mat2x4() : value{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}} {}
  Mat2x4(float _00, float _01, float _02, float _03,
         float _10, float _11, float _12, float _13)
         : value{{_00, _01, _02, _03}, {_10, _11, _12, _13}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat4x2 {
  float value[4][2];

public:
  Mat4x2() : value{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}} {}
  Mat4x2(float _00, float _01, float _10, float _11, float _20, float _21, float _30, float _31)
         : value{{_00, _01}, {_10, _11}, {_20, _21}, {_30, _31}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat3x4 {
  float value[3][4];

public:
  Mat3x4() : value{{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}} {}
  Mat3x4(float _00, float _01, float _02, float _03,
         float _10, float _11, float _12, float _13,
         float _20, float _21, float _22, float _23)
         : value{{_00, _01, _02, _03}, {_10, _11, _12, _13}, {_20, _21, _22, _23}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat4x3 {
  float value[4][3];

public:
  Mat4x3() : value{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}} {}
  Mat4x3(float _00, float _01, float _02, float _10, float _11, float _12,
         float _20, float _21, float _22, float _30, float _31, float _32)
         : value{{_00, _01, _02}, {_10, _11, _12}, {_20, _21, _22}, {_30, _31, _32}} {}

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }
};

class Mat4 {
  float value[4][4];

public:
  static const Mat4 identity;

  Mat4() { *this = identity; }
  Mat4(float _00, float _01, float _02, float _03,
       float _10, float _11, float _12, float _13,
       float _20, float _21, float _22, float _23,
       float _30, float _31, float _32, float _33)
       : value{{_00, _01, _02, _03},
               {_10, _11, _12, _13},
               {_20, _21, _22, _23},
               {_30, _31, _32, _33}} {}

  Vec3 Forward()
    { return -Back(); }
  void Forward(Vec3 val)
    { Down(-val); }
  Vec3 Back()
    { return Vec3(value[2][0], value[2][1], value[2][2]); }
  void Back(Vec3 val)
    { value[2][0] = val.x; value[2][1] = val.y; value[2][2] = val.z; }
  Vec3 Up()
    { return Vec3(value[1][0], value[1][1], value[1][2]); }
  void Up(Vec3 val)
    { value[1][0] = val.x; value[1][1] = val.y; value[1][2] = val.z; }
  Vec3 Down()
    { return -Up(); }
  void Down(Vec3 val)
    { Up(-val); }
  Vec3 Left()
    { return -Right(); }
  void Left(Vec3 val)
    { Right(-val); }
  Vec3 Right()
    { return Vec3(value[0][0], value[0][1], value[0][2]); }
  void Right(Vec3 val)
    { value[0][0] = val.x; value[0][1] = val.y; value[0][2] = val.z; }

  Mat4 Inverse();

  static Mat4 LookAt(Vec3 cameraPos, Vec3 target, Vec3 cameraUp);

  static Mat4 Ortho(float width, float height, float near, float far);

  static Mat4 Ortho(float left, float right, float bottom, float top, float near, float far);

  static Mat4 Perspective(float width, float height, float near, float far);

  static Mat4 PerspectiveFOV(float fov, float aspect, float near, float far);

  static Mat4 Perspective(float left, float right, float bottom, float top, float near, float far);

  static Mat4 Scale(float x, float y, float z) {
    return Mat4(x, 0.0f, 0.0f, 0.0f,
                0.0f, y, 0.0f, 0.0f,
                0.0f, 0.0f, z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
  }

  static Mat4 Scale(Vec3 v)
    { return Scale(v.x, v.y, v.z); }

  static Mat4 Translate(float x, float y, float z) {
    return Mat4(1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                x, y, z, 1.0f);
  }

  static Mat4 Translate(Vec3 v)
    { return Translate(v.x, v.y, v.z); }

  const float *operator[](size_t i) const
    { return value[i]; }
  float *operator[](size_t i)
    { return value[i]; }

  Mat4 operator*(const Mat4 &r) const;

  Vec3 operator*(const Vec3 &r);

  Vec4 operator*(const Vec4 &r);

  Mat4 &operator*=(const Mat4 &r) {
    *this = *this * r;
    return *this;
  }
};

std::ostream &operator<<(std::ostream &os, const Mat4 &m);

#endif // _MATH__MAT_H
