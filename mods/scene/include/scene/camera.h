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

#ifndef _SCENE__CAMERA_H
#define _SCENE__CAMERA_H

#include <vector>
#include <algorithm>
#include <scene/node.h>
#include <base/window.h>
#include <math/mat.h>

class NCamera : public NNode {
  static std::vector<NCamera *> all;
  float currentZoom = 10.0f, minZoom = 3.0f, maxZoom = 20.0f;

public:
  bool perspective = true;
  float near = 0.1f, far = 100.1f;
  float fov = 45.0f;

  NCamera() { all.push_back(this); }
  ~NCamera();

  static NCamera *active;
  std::vector<NCamera *> &All() const { return all; }

  float Zoom() const { return currentZoom; }
  float MinZoom() const { return minZoom; }
  float MaxZoom() const { return maxZoom; }

  void ChangeZoom(float relative)
    { currentZoom = Math::Clamp(currentZoom + relative, minZoom, maxZoom); }

  void Zoom(float value) { currentZoom = value; }
  void MinZoom(float value) { minZoom = value; }
  void MaxZoom(float value) { maxZoom = value; }

  Mat4 ProjectionMatrix() const;

  Mat4 ViewMatrix() const;

  Mat4 Matrix(Mat4 model) const
    { return ProjectionMatrix() * ViewMatrix() * model; }

  virtual void SerializeToJSON(Writer &writer) const override;
  virtual bool LoadFromJSON(const rapidjson::Value &data, JSONTypeManager &manager) override;
};

#endif // _SCENE__CAMERA_H
