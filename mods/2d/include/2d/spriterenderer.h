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

#ifndef _2D__CANVAS_IMAGE_H
#define _2D__CANVAS_IMAGE_H

#include <2d/canvasitem.h>
#include <base/mesh.h>
#include <scene/camera.h>

struct Sprite;

class SpriteRenderer : public CanvasItem {
  const Sprite *sprite;
  bool visible = true;
  Mesh quad;
  static std::vector<GLfloat> quadVertices;
  static std::vector<GLuint> quadIndices;

protected:
  void Draw();

public:
  const Sprite *Data() { return sprite; }
  void Data(const Sprite *spr);

  bool Visible() const { return visible; }
  void Visible(bool value) { visible = value; }
};

#endif // _2D__CANVAS_IMAGE_H
