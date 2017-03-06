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

#ifndef _SCENE__TRANSFORM_H
#define _SCENE__TRANSFORM_H

#include <ostream>
#include <iomanip>
#include <math/vec.h>
#include <math/quat.h>
#include <core/saveload.h>
#include <scene/rendertree.h>

class Transform : public RenderTree<class NNode>, public SaveLoad {
  Vec3 location, scale;
  Quat rotation;

  mutable Mat4 matrix;
  mutable bool dirty = true;

public:
  Transform() : scale(Vec3(1.0f, 1.0f, 1.0f)) {}
  Transform(Vec3 loc, Quat rot, Vec3 scl) : location(loc), scale(scl), rotation(rot), dirty(true) {}

  Vec3 Location() const { return location; }
  Quat Rotation() const { return rotation; }
  Vec3 Scale() const { return scale; }

  Mat4 Matrix() const;

  Transform operator*(const Transform &other);

  Transform &operator*=(const Transform &other);

  Vec3 GlobalLocation() const;

  Quat GlobalRotation() const;

  Vec3 GlobalScale() const;

  Transform GlobalTransform() const;

  friend std::ostream &operator<<(std::ostream &os, const Transform &t);

  void Translate(Vec3 value)
    { location += value; dirty = true; }
  void Translate(float x, float y, float z)
    { Translate(Vec3(x, y, z)); }

  void RotateGlobal(Vec3 value)
    { rotation = rotation * Quat(value); dirty = true; }
  void RotateGlobal(Quat value)
    { rotation = rotation * value; dirty = true; }
  void RotateGlobal(float x, float y, float z)
    { RotateGlobal(Vec3(x, y, z)); }

  void Rotate(Vec3 value)
    { rotation = Quat(value) * rotation; dirty = true; }
  void Rotate(Quat value)
    { rotation = value * rotation; dirty = true; }
  void Rotate(float x, float y, float z)
    { Rotate(Vec3(x, y, z)); }

  void ChangeScale(Vec3 value)
    { scale += value; dirty = true; }
  void ChangeScale(float x, float y, float z)
    { ChangeScale(Vec3(x, y, z)); }

  void Location(Vec3 loc)
    { location = loc; dirty = true; }
  void Location(float x, float y, float z)
    { Location(Vec3(x, y, z)); }

  void Rotation(Vec3 rot)
    { rotation = Quat(rot); dirty = true; }
  void Rotation(float x, float y, float z)
    { Rotation(Vec3(x, y, z)); }
  void Rotation(Quat rot)
    { rotation = rot; dirty = true; }

  void Scale(Vec3 scl)
    { scale = scl; dirty = true; }
  void Scale(float x, float y, float z)
    { Scale(Vec3(x, y, z)); }

  virtual void SerializeToJSON(Writer &writer) const override;
  virtual bool LoadFromJSON(const rapidjson::Value &data, JSONTypeManager &/* manager */) override;
};

inline std::ostream &operator<<(std::ostream &os, const Transform &t) {
  os << "Location: " << t.location.x << "," << t.location.y << "," << t.location.z
     << "\nRotation: " << t.rotation.x << "," << t.rotation.y << "," << t.rotation.z << "," << t.rotation.w
     << "\nScale: " << t.scale.x << "," << t.scale.y << "," << t.scale.z;
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const Mat4 &m) {
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 3; j++)
      os << std::setw(6) << std::fixed << std::setprecision(2) << m[i][j] << ",";
    os << std::setw(6) << std::fixed << std::setprecision(2) << m[i][3] << '\n';
  }
  return os;
}

#endif // _SCENE__TRANSFORM_H
