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

#ifndef _SCENE__TRANSFORMATION_TREE_H
#define _SCENE__TRANSFORMATION_TREE_H

#include <algorithm>
#include <vector>

// Only classes of the template type argument 'T' should derive from this class
template <typename T>
class TransformationTree {
  // Since this class is should be derived by type 'T', this reinterpret_cast is
  // justified
  T *This() { return reinterpret_cast<T *>(this); }
  const T *This() const { return reinterpret_cast<const T *>(this); }

protected:
  T *parent = nullptr;
  std::vector<T *> children;

  // Prevent this class being directly instantiated
  TransformationTree() {}

public:
  virtual ~TransformationTree() {}

  TransformationTree(const TransformationTree &other) {
    Parent(other.parent);
    children = other.children;
  }

  TransformationTree &operator=(const TransformationTree &other) {
    if (this == &other) return *this;
    Parent(other.parent);
    children == other.children;
    return *this;
  }

  using iterator = typename std::vector<T *>::iterator;
  using const_iterator = typename std::vector<T *>::const_iterator;

  iterator begin() { return children.begin(); }
  const_iterator begin() const { return children.begin(); }

  iterator end() { return children.end(); }
  const_iterator end() const { return children.end(); }

  void Parent(T *other) {
    if (parent == other) return;
    if (parent) parent->RemoveChild(This());

    if (other) other->children.push_back(This());
    parent = other;
  }

  T *Parent() { return parent; }
  const T *Parent() const { return parent; }

  auto ChildCount() const { return children.size(); }
  T *Child(typename std::vector<T *>::size_type i) const { return children[i]; }

  void RemoveChild(T *child) {
    auto it = std::find(std::begin(children), std::end(children), child);
    if (it == std::end(children)) return; // REVIEW: Doesn't treat as an error
    child->parent = nullptr;
    children.erase(it);
  }

  friend class NNode;
  friend class CanvasItem;
};

#endif // _SCENE__TRANSFORMATION_TREE_H
