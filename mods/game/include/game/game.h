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

#ifndef _GAME__GAME_H
#define _GAME__GAME_H

#include <base/window.h>
#include <functional>
#include <memory>

class NNode;

class Game {
  using TickFunction = std::function<void(float)>;
  using TickFunctionList = std::list<TickFunction>;
  using TickRegistration = TickFunctionList::iterator;

  TickFunctionList tickFunctions;
  float elapsedTime = 0.0f;

protected:
  virtual void Initialize() {}
  virtual void Tick(float /* delta */) {}

public:
  Game();
  virtual ~Game();

  TickRegistration RegisterTick(TickFunction func) {
    tickFunctions.push_front(func);
    return tickFunctions.begin();
  }

  void UnregisterTick(TickRegistration registration) {
    tickFunctions.erase(registration);
  }

  void Start();

  float GetElapsedTime() { return elapsedTime; }

  std::unique_ptr<Window> window;
};

#endif // _GAME__GAME_H
