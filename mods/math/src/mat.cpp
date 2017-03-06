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

#include <mat.h>

const Mat4 Mat4::identity{1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f};

Mat4 Mat4::Inverse() {
  float v22332332 = (double)value[2][2] * (double)value[3][3] - (double)value[2][3] * (double)value[3][2];
  float v21332331 = (double)value[2][1] * (double)value[3][3] - (double)value[2][3] * (double)value[3][1];
  float v21322231 = (double)value[2][1] * (double)value[3][2] - (double)value[2][2] * (double)value[3][1];
  float v20332330 = (double)value[2][0] * (double)value[3][3] - (double)value[2][3] * (double)value[3][0];
  float v20322230 = (double)value[2][0] * (double)value[3][2] - (double)value[2][2] * (double)value[3][0];
  float v20312130 = (double)value[2][0] * (double)value[3][1] - (double)value[2][1] * (double)value[3][0];
  float v11_12_13_ = (double)value[1][1] * (double)v22332332 - (double)value[1][2] * (double)v21332331 + (double)value[1][3] * (double)v21322231;
  float v10_12_13_ = -(double)value[1][0] * (double)v22332332 - (double)value[1][2] * (double)v20332330 + (double)value[1][3] * (double)v20322230;
  float v10_11_13_ = (double)value[1][0] * (double)v21332331 - (double)value[1][1] * (double)v20332330 + (double)value[1][3] * (double)v20312130;
  float v10_11_12_ = -(double)value[1][0] * (double)v21322231 - (double)value[1][1] * (double)v20322230 + (double)value[1][2] * (double)v20312130;
  float v00_01_02_03_ = 1.0 / ((double)value[0][0] * (double)v11_12_13_ + (double)value[0][1] * (double)v10_12_13_ + (double)value[0][2] * (double)v10_11_13_ + (double)value[0][3] * (double)v10_11_12_);
  float v12331332 = (double)value[1][2] * (double)value[3][3] - (double)value[1][3] * (double)value[3][2];
  float v11331331 = (double)value[1][1] * (double)value[3][3] - (double)value[1][3] * (double)value[3][1];
  float v11321231 = (double)value[1][1] * (double)value[3][2] - (double)value[1][2] * (double)value[3][1];
  float v10331330 = (double)value[1][0] * (double)value[3][3] - (double)value[1][3] * (double)value[3][0];
  float v10321230 = (double)value[1][0] * (double)value[3][2] - (double)value[1][2] * (double)value[3][0];
  float v10311130 = (double)value[1][0] * (double)value[3][1] - (double)value[1][1] * (double)value[3][0];
  float v12231322 = (double)value[1][2] * (double)value[2][3] - (double)value[1][3] * (double)value[2][2];
  float v11231321 = (double)value[1][1] * (double)value[2][3] - (double)value[1][3] * (double)value[2][1];
  float v11221221 = (double)value[1][1] * (double)value[2][2] - (double)value[1][2] * (double)value[2][1];
  float v10231320 = (double)value[1][0] * (double)value[2][3] - (double)value[1][3] * (double)value[2][0];
  float v10221220 = (double)value[1][0] * (double)value[2][2] - (double)value[1][2] * (double)value[2][0];
  float v10211120 = (double)value[1][0] * (double)value[2][1] - (double)value[1][1] * (double)value[2][0];

  return Mat4(v11_12_13_ * v00_01_02_03_,
              -((double)value[0][1] * (double)v22332332 - (double)value[0][2] * (double)v21332331 + (double)value[0][3] * (double)v21322231) * v00_01_02_03_,
              ((double)value[0][1] * (double)v12331332 - (double)value[0][2] * (double)v11331331 + (double)value[0][3] * (double)v11321231) * v00_01_02_03_,
              -((double)value[0][1] * (double)v12231322 - (double)value[0][2] * (double)v11231321 + (double)value[0][3] * (double)v11221221) * v00_01_02_03_,
              v10_12_13_ * v00_01_02_03_,
              ((double)value[0][0] * (double)v22332332 - (double)value[0][2] * (double)v20332330 + (double)value[0][3] * (double)v20322230) * v00_01_02_03_,
              -((double)value[0][0] * (double)v12331332 - (double)value[0][2] * (double)v10331330 + (double)value[0][3] * (double)v10321230) * v00_01_02_03_,
              ((double)value[0][0] * (double)v12231322 - (double)value[0][2] * (double)v10231320 + (double)value[0][3] * (double)v10221220) * v00_01_02_03_,
              v10_11_13_ * v00_01_02_03_,
              -((double)value[0][0] * (double)v21332331 - (double)value[0][1] * (double)v20332330 + (double)value[0][3] * (double)v20312130) * v00_01_02_03_,
              ((double)value[0][0] * (double)v11331331 - (double)value[0][1] * (double)v10331330 + (double)value[0][3] * (double)v10311130) * v00_01_02_03_,
              -((double)value[0][0] * (double)v11231321 - (double)value[0][1] * (double)v10231320 + (double)value[0][3] * (double)v10211120) * v00_01_02_03_,
              v10_11_12_ * v00_01_02_03_,
              ((double)value[0][0] * (double)v21322231 - (double)value[0][1] * (double)v20322230 + (double)value[0][2] * (double)v20312130) * v00_01_02_03_,
              -((double)value[0][0] * (double)v11321231 - (double)value[0][1] * (double)v10321230 + (double)value[0][2] * (double)v10311130) * v00_01_02_03_,
              ((double)value[0][0] * (double)v11221221 - (double)value[0][1] * (double)v10221220 + (double)value[0][2] * (double)v10211120) * v00_01_02_03_);
}

Mat4 Mat4::LookAt(Vec3 cameraPos, Vec3 target, Vec3 cameraUp) {
  Vec3 vec = (cameraPos - target).Normalized();
  Vec3 vec2 = Vec3::Cross(cameraUp, vec).Normalized();
  Vec3 vec3 = Vec3::Cross(vec, vec2);
  return Mat4(vec2.x, vec3.x, vec.x, 0.0f,
              vec2.y, vec3.y, vec.y, 0.0f,
              vec2.z, vec3.z, vec.z, 0.0f,
              -Vec3::Dot(vec2, cameraPos), -Vec3::Dot(vec3, cameraPos), -Vec3::Dot(vec, cameraPos), 1.0f);
}

Mat4 Mat4::Ortho(float width, float height, float near, float far) {
  return Mat4(2.0f / width, 0.0f, 0.0f, 0.0f,
              0.0f, 2.0f / height, 0.0f, 0.0f,
              0.0f, 0.0f, 1.0f / (near - far), 0.0f,
              0.0f, 0.0f, near / (near - far), 1.0f);
}

Mat4 Mat4::Ortho(float left, float right, float bottom, float top, float near, float far) {
  return Mat4(2.0f / (right - left), 0.0f, 0.0f, 0.0f,
              0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
              0.0f, 0.0f, 1.0f / (near - far), 0.0f,
              (left + right) / (left - right), (top + bottom) / (bottom - top), near / (near - far), 1.0f);
}

Mat4 Mat4::Perspective(float width, float height, float near, float far) {
  return Mat4((2.0f * near) / width, 0.0f, 0.0f, 0.0f,
              0.0f, (2.0f * near) / height, 0.0f, 0.0f,
              0.0f, 0.0f, far / (near - far), -1.0f,
              0.0f, 0.0f, (near * far) / (near - far), 0.0f);
}

Mat4 Mat4::PerspectiveFOV(float fov, float aspect, float near, float far) {
  float r = 1.0f / (float)tan((double)(fov * 0.5f));
  return Mat4(r / aspect, 0.0f, 0.0f, 0.0f,
              0.0f, r, 0.0f, 0.0f,
              0.0f, 0.0f, far / (near - far), -1.0f,
              0.0f, 0.0f, (near * far) / (near - far), 0.0f);
}

Mat4 Mat4::Perspective(float left, float right, float bottom, float top, float near, float far) {
  return Mat4((2.0f * near) / (right - left), 0.0f, 0.0f, 0.0f,
              0.0f, (2.0f * near) / (top - bottom), 0.0f, 0.0f,
              (left + right) / (right - left), (top + bottom) / (top - bottom), far / (near - far), -1.0f,
              0.0f, 0.0f, (near * far) / (near - far), 0.0f);
}

Mat4 Mat4::operator*(const Mat4 &r) const {
  return Mat4(r[0][0] * value[0][0] + r[0][1] * value[1][0] + r[0][2] * value[2][0] + r[0][3] * value[3][0],
              r[0][0] * value[0][1] + r[0][1] * value[1][1] + r[0][2] * value[2][1] + r[0][3] * value[3][1],
              r[0][0] * value[0][2] + r[0][1] * value[1][2] + r[0][2] * value[2][2] + r[0][3] * value[3][2],
              r[0][0] * value[0][3] + r[0][1] * value[1][3] + r[0][2] * value[2][3] + r[0][3] * value[3][3],
              r[1][0] * value[0][0] + r[1][1] * value[1][0] + r[1][2] * value[2][0] + r[1][3] * value[3][0],
              r[1][0] * value[0][1] + r[1][1] * value[1][1] + r[1][2] * value[2][1] + r[1][3] * value[3][1],
              r[1][0] * value[0][2] + r[1][1] * value[1][2] + r[1][2] * value[2][2] + r[1][3] * value[3][2],
              r[1][0] * value[0][3] + r[1][1] * value[1][3] + r[1][2] * value[2][3] + r[1][3] * value[3][3],
              r[2][0] * value[0][0] + r[2][1] * value[1][0] + r[2][2] * value[2][0] + r[2][3] * value[3][0],
              r[2][0] * value[0][1] + r[2][1] * value[1][1] + r[2][2] * value[2][1] + r[2][3] * value[3][1],
              r[2][0] * value[0][2] + r[2][1] * value[1][2] + r[2][2] * value[2][2] + r[2][3] * value[3][2],
              r[2][0] * value[0][3] + r[2][1] * value[1][3] + r[2][2] * value[2][3] + r[2][3] * value[3][3],
              r[3][0] * value[0][0] + r[3][1] * value[1][0] + r[3][2] * value[2][0] + r[3][3] * value[3][0],
              r[3][0] * value[0][1] + r[3][1] * value[1][1] + r[3][2] * value[2][1] + r[3][3] * value[3][1],
              r[3][0] * value[0][2] + r[3][1] * value[1][2] + r[3][2] * value[2][2] + r[3][3] * value[3][2],
              r[3][0] * value[0][3] + r[3][1] * value[1][3] + r[3][2] * value[2][3] + r[3][3] * value[3][3]);
}

Vec3 Mat4::operator*(const Vec3 &r) {
  return Vec3(r.x * value[0][0] + r.y * value[1][0] + r.z * value[2][0] + value[3][0],
              r.x * value[0][1] + r.y * value[1][1] + r.z * value[2][1] + value[3][1],
              r.x * value[0][2] + r.y * value[1][2] + r.z * value[2][2] + value[3][2]);
}

Vec4 Mat4::operator*(const Vec4 &r) {
  return Vec4(r.x * value[0][0] + r.y * value[1][0] + r.z * value[2][0] + r.w * value[3][0],
              r.x * value[0][1] + r.y * value[1][1] + r.z * value[2][1] + r.w * value[3][1],
              r.x * value[0][2] + r.y * value[1][2] + r.z * value[2][2] + r.w * value[3][2],
              r.x * value[0][3] + r.y * value[1][3] + r.z * value[2][3] + r.w * value[3][3]);
}

std::ostream &operator<<(std::ostream &os, const Mat4 &m) {
  os << m[0][0] << ',' << m[0][1] << ',' << m[0][2] << ',' << m[0][3] << ",\n"
     << m[1][0] << ',' << m[1][1] << ',' << m[1][2] << ',' << m[1][3] << ",\n"
     << m[2][0] << ',' << m[2][1] << ',' << m[2][2] << ',' << m[2][3] << ",\n"
     << m[3][0] << ',' << m[3][1] << ',' << m[3][2] << ',' << m[3][3];
  return os;
}
