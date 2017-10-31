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

#ifndef _BASE__REGISTRATION_H
#define _BASE__REGISTRATION_H

#include <base/callbacklist.h>

template <typename List>
class Registration {
  List *registrationList = nullptr;
  using ListIterator = typename List::const_iterator;
  ListIterator it;

  void UnregisterUnchecked() {
    registrationList->erase(it);
    registrationList = nullptr;
  }

public:
  template <typename Value>
  Registration(Value v, List &regList) : registrationList(&regList) {
    registrationList->emplace_front(v);
    it = std::begin(*registrationList);
  }

  Registration() {}

  bool Registered() const { return registrationList; }

  // Copying a registration is not permitted but moving is
  Registration(const Registration &) = delete;

  Registration(Registration &&other) {
    registrationList = other.registrationList;
    it = other.it;
    // Prevent other's destructor from unregistering
    other.registrationList = nullptr;
  }

  Registration &operator=(Registration &&other) {
    if (&other == this) return *this;
    if (Registered()) UnregisterUnchecked();

    registrationList = other.registrationList;
    it = other.it;
    // Prevent other's destructor from unregistering
    other.registrationList = nullptr;
    return *this;
  }

  void Unregister() {
    assert(Registered());
    UnregisterUnchecked();
  }

  ~Registration() {
    if (Registered()) UnregisterUnchecked();
  }
};

template <typename FuncSig, bool AllowUnregisterDuringCallAll>
class Registration<CallbackList<FuncSig, AllowUnregisterDuringCallAll>> {
  using Callbacks = CallbackList<FuncSig, AllowUnregisterDuringCallAll>;

  Callbacks *callbacks = nullptr;
  typename Callbacks::Registration reg;

  void UnregisterUnchecked() {
    callbacks->Unregister(reg);
    callbacks = nullptr;
  }

public:
  using Callback = typename Callbacks::Function;

  Registration(Callback func, Callbacks &cs) :
      callbacks(&cs), reg(callbacks->Register(func)) {}

  Registration() {}

  bool Registered() const { return callbacks; }

  // Copying a registration is not permitted but moving is
  Registration(const Registration &) = delete;

  Registration(Registration &&other) {
    callbacks = other.callbacks;
    reg = other.reg;
    // Prevent other's destructor from unregistering
    other.callbacks = nullptr;
  }

  Registration &operator=(Registration &&other) {
    if (&other == this) return *this;
    if (Registered()) UnregisterUnchecked();

    callbacks = other.callbacks;
    reg = other.reg;
    // Prevent other's destructor from unregistering
    other.callbacks = nullptr;
    return *this;
  }

  void Unregister() {
    assert(Registered());
    UnregisterUnchecked();
  }

  ~Registration() {
    if (Registered())
      UnregisterUnchecked();
  }
};

#endif // _BASE__REGISTRATION_H

