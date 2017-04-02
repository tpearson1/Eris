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

#ifndef _CORE__REF_H
#define _CORE__REF_H

#include <ostream>
#include <core/readwrite.h>

template <typename T>
class Ref {
  struct Stub {
    unsigned refCount;
    T data;

    Stub(unsigned rc) { refCount = rc; }

    template <typename... Args>
    Stub(unsigned rc, Args&&... args) : refCount(rc), data(std::forward<Args>(args)...) {}
  };

  Stub *stub = nullptr;

  Ref(void *_stub) { stub = (Stub *)_stub; }

public:
  Ref() {}

  Ref(const Ref<T> &other) {
    *this = other;
  }

  Ref(Ref<T> &other) {
    *this = other;
  }

  void Reset() {
    if (stub)
      delete stub;
    stub = new Stub(1);
  }

  template <typename... Args>
  static Ref<T> Create(Args&&... args) {
    Ref<T> ref;
    ref.stub = new Stub(1, std::forward<Args>(args)...);
    return ref;
  }

  template <typename... Args>
  void Reset(Args&&... args) {
    if (stub)
      delete stub;
    stub = new Stub(stub->refCount, std::forward<Args>(args)...);
  }

  bool operator==(const Ref<T> &other) const { return stub == other.stub; }
  bool operator!=(const Ref<T> &other) const { return stub != other.stub; }
  bool operator<(const Ref<T> &other) const { return stub < other.stub; }
  bool operator<=(const Ref<T> &other) const { return stub <= other.stub; }
  bool operator>(const Ref<T> &other) const { return stub > other.stub; }
  bool operator>=(const Ref<T> &other) const { return stub >= other.stub; }

  Ref<T> &operator=(const Ref<T> &r) {
    if (r.Valid()) {
      stub = r.stub;
      stub->refCount++;
    }
    return *this;
  }

  Ref<T> &operator=(Ref<T> &&r) {
    if (r.Valid()) {
      stub = r.stub;
      stub->refCount++;
    }
    return *this;
  }

  explicit operator bool() const { return Valid(); }

  operator Ref<const T>() { return Ref<const T>(stub); }

  template <typename B>
  explicit operator Ref<B>() { return Ref<B>(stub); }

  auto &operator[](unsigned index)
    { return stub->data[index]; }

  const auto &operator[](unsigned index) const
    { return stub->data[index]; }

  bool Valid() const { return stub; }

  void Swap(Ref<T> &other) {
    auto temp = stub;
    stub = other.stub;
    other.stub = temp;
  }

  T &operator*() const
    { return stub->data; }

  T *operator->() const
    { return &stub->data; }

  T *Get() { return stub ? &stub->data : nullptr; }

  auto Count() const { return Valid() ? stub->refCount : 0u; }

  ~Ref() {
    if (stub && --stub->refCount == 0)
      delete stub;
  }

  void WriteToJSON(JSON::Writer &writer) const
    { stub->data.WriteToJSON(writer); }

  bool ReadFromJSON(const rapidjson::Value &data, JSON::TypeManager &manager)
    { return stub->data.ReadFromJSON(data, manager); }

  template <typename Data>
  void Load(const Data &value) {
    if (!stub)
      stub = new Stub(1);
    stub->data.Load(value);
  }

  template <typename U>
  friend class Ref;

  template <typename R>
  friend std::ostream &operator<<(std::ostream &os, Ref<R> &ref);
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, Ref<T> &ref) {
  os << ref.stub->data;
  return os;
}

void RunRefTests();

#endif // _CORE__REF_H
