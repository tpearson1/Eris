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

#ifndef _SCENE__NODE_H
#define _SCENE__NODE_H

#include <deque>
#include <algorithm>
#include <cstddef>
#include <scene/transform.h>
#include <scene/renderable.h>

template <>
struct JSONImpl<NNode> {
  static void Read(NNode &out, const JSON::Value &value, const JSON::ReadData &data);
  static void Write(const NNode &value, JSON::Writer &writer);
};

class NNode : public Renderable {
  void SetTransformNodeMember()
    { transform.node = this; }

protected:
  static void RecursiveDestroy(NNode *n);

  virtual void OnDestroy() {}
  virtual void _Tick(float /*delta*/) {}

public:
  NNode()
    { SetTransformNodeMember(); }

  virtual void Tick(float delta)
    { if (visible) _Tick(delta); }

  virtual ~NNode() {}

  void Destroy()
    { RecursiveDestroy(this); }

  Transform transform;

  friend void JSONImpl<NNode>::Read(NNode &out, const JSON::Value &value, const JSON::ReadData &data);
};

#endif // _SCENE__NODE_H
