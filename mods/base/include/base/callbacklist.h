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

#ifndef _BASE__CALLBACK_LIST_H
#define _BASE__CALLBACK_LIST_H

#include <cassert>
#include <functional>
#include <list>
#include <vector>

namespace Detail {
template <typename FuncSig>
class CallbackListBase {
public:
  using Function = std::function<FuncSig>;
  using FunctionList = std::list<Function>;
  using Registration = typename FunctionList::iterator;

protected:
  FunctionList callbackList;

public:
  Registration Register(Function func) {
    callbackList.push_front(func);
    return std::begin(callbackList);
  }

  using iterator = typename FunctionList::iterator;
  using const_iterator = typename FunctionList::const_iterator;

  iterator begin() { return std::begin(callbackList); }
  const_iterator begin() const { return std::begin(callbackList); }

  iterator end() { return std::end(callbackList); }
  const_iterator end() const { return std::end(callbackList); }
};
} // namespace Detail

template <typename FuncSig, bool AllowUnregisterDuringCallAll>
class CallbackList;

template <typename FuncSig>
class CallbackList<FuncSig, false> : public Detail::CallbackListBase<FuncSig> {
  using Base = Detail::CallbackListBase<FuncSig>;

#ifndef NDEBUG
  bool inCallAll = false;
#endif

public:
  template <typename... Args>
  void CallAll(Args &&... args) {
#ifndef NDEBUG
    inCallAll = true;
#endif
    for (auto &callback : this->callbackList)
      callback(std::forward<Args>(args)...);
#ifndef NDEBUG
    inCallAll = false;
#endif
  }

  void Unregister(typename Base::Registration registration) {
#ifndef NDEBUG
    assert(!inCallAll);
#endif
    this->callbackList.erase(registration);
  }
};

template <typename FuncSig>
class CallbackList<FuncSig, true> : public Detail::CallbackListBase<FuncSig> {
  using Base = Detail::CallbackListBase<FuncSig>;
  std::vector<typename Base::Registration> toUnregister;

  bool inCallAll = false;

public:
  template <typename... Args>
  void CallAll(Args &&... args) {
    inCallAll = true;
    for (auto &callback : this->callbackList)
      callback(std::forward<Args>(args)...);
    inCallAll = false;

    UnregisterPending();
  }

  void UnregisterPending() {
    for (auto reg : toUnregister)
      this->callbackList.erase(reg);
    toUnregister.clear();
  }

  void Unregister(typename Base::Registration registration) {
    if (!inCallAll) {
      this->callbackList.erase(registration);
      return;
    }

    // We are currently calling the callbacks, and thus it is dangerous to
    // erase an element. Instead we wait until the callbacks are done being
    // called and then remove it
    toUnregister.push_back(registration);
  }
};

#endif // _BASE__CALLBACK_LIST_H
