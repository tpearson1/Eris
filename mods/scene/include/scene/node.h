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

#include <scene/transform.h>
#include <scene/transformationtree.h>

class NNode;

template <>
struct JSONImpl<NNode> {
  static void Read(NNode &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(const NNode &value, JSON::Writer &writer);
};

class NNode : public TransformationTree<NNode> {
protected:
  static void RecursiveDestroy(NNode *n);

  virtual void OnDestroy() {}

public:
  Vec3 GlobalLocation() const;

  Quat GlobalRotation() const;

  Vec3 GlobalScale() const;

  /*
   * This function is equivalent to:
   * return Transform(
   *   GlobalLocation(),
   *   GlobalRotation(),
   *   GlobalScale()
   * );
   */
  Transform GlobalTransform() const;

  virtual ~NNode() {}

  void Destroy() { RecursiveDestroy(this); }

  Transform transform;

  friend void JSONImpl<NNode>::Write(const NNode &value, JSON::Writer &writer);
  friend void JSONImpl<NNode>::Read(NNode &out, const JSON::Value &value,
                                    const JSON::ReadData &data);
};

#endif // _SCENE__NODE_H
