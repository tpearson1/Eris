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

#ifndef _2D__RECT_TRANSFORM_H
#define _2D__RECT_TRANSFORM_H

#include <math/vec.h>
#include <math/quat.h>
#include <core/readwrite.h>
#include <scene/rendertree.h>

class RectTransform : public RenderTree<class CanvasItem>, public JSON::ReadWrite {
  /*
   * The relative position of the UI element relative to its anchors
   */
  Vec3 location;

  /*
   * The pivot location for the UI element. Relative to the object
   */
  Vec2 pivot{0.5f, 0.5f};

  /*
   * The location of the UI element's anchors. Relative to the size of the
   * parent element
   */
  Vec2 anchors{0.5f, 0.5f};

  /*
   * The size of the UI element
   */
  Vec2 size{1.0f, 1.0f};

  /*
   * The UI element's rotation in radians
   */
  float rotation = 0.0f;

public:
  Vec3 Location() const { return location; }
  void Location(Vec3 v) { location = v; }
  void Location(float x, float y, float z) { location = Vec3(x, y, z); }
  void Translate(Vec3 v) { location += v; }
  void Translate(float x, float y, float z) { location += Vec3(x, y, z); }

  Vec2 Pivot() const { return pivot; }
  void Pivot(Vec2 v) { pivot = v; }
  void Pivot(float x, float y) { pivot = Vec2(x, y); }

  Vec2 Anchors() const { return anchors; }
  void Anchors(Vec2 v) { anchors = v; }
  void Anchors(float x, float y) { anchors = Vec2(x, y); }

  Vec2 Size() const { return size; }
  void Size(Vec2 v) { size = v; }
  void Size(float x, float y) { size = Vec2(x, y); }

  float Rotation() const { return rotation; }
  void Rotation(float radians) { rotation = radians; }
  void Rotate(float radians) { rotation += radians; }

  Mat4 Matrix() const;
};

#endif // _2D__RECT_TRANSFORM_H
