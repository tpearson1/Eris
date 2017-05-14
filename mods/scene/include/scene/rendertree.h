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

#ifndef _CORE_MATH__RENDER_TREE_H
#define _CORE_MATH__RENDER_TREE_H

#include <vector>
#include <algorithm>

template <typename T>
class RenderTree {
protected:
  T *parent = nullptr;
  std::vector<T *> children;
  T *node;

public:
  virtual ~RenderTree() {}

  using iterator = typename std::vector<T *>::iterator;
  using const_iterator = typename std::vector<T *>::const_iterator;

  iterator begin()
    { return children.begin(); }
  const_iterator begin() const
    { return children.begin(); }

  iterator end()
    { return children.end(); }
  const_iterator end() const
    { return children.end(); }

  void Parent(T *other) {
    if (parent == other)
      return;
    if (parent)
      parent->transform.RemoveChild(node);

    if (other)
      other->transform.children.push_back(node);
    parent = other;
  }

  T *Parent() { return parent; }
  const T *Parent() const { return parent; }

  auto ChildCount() const { return children.size(); }
  T *Child(typename std::vector<T *>::size_type i) const
    { return children[i]; }

  void RemoveChild(T *child) {
    auto it = std::find(std::begin(children), std::end(children), child);
    if (it == std::end(children))
      return; // REVIEW: Doesn't treat as an error
    child->transform.parent = nullptr;
    children.erase(it);
  }

  friend class NNode;
  friend class CanvasItem;
};

#endif // _CORE_MATH__RENDER_TREE_H
