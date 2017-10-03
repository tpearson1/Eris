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

#include <tickmanager.h>

void TickManager::Tick(float delta) {
  inTick = true;
  for (auto &tickFunc : tickFunctions) tickFunc(delta);
  inTick = false;
  for (auto registrationIt : toUnregister)
    tickFunctions.erase(registrationIt);
}

TickManager::TickRegistration TickManager::RegisterTick(TickFunction func) {
  tickFunctions.push_front(func);
  return tickFunctions.begin();
}

void TickManager::UnregisterTick(TickRegistration registration) {
  if (!inTick) {
    tickFunctions.erase(registration);
    return;
  }

  // We are currently executing the tick functions, and thus it is dangerous to
  // erase an element. Instead we wait until the tick functions are done being
  // executed and then remove it
  toUnregister.push_back(registration);
}

