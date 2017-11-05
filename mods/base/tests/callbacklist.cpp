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

#include <catch.hpp>

#include <callbacklist.h>

TEST_CASE("Registering and unregistering callbacks with CallbackList",
          "[CallbackList]") {
  CallbackList<void(int), true> callbacks;

  int x = 0;
  auto reg = callbacks.Register([&](int arg) { x += arg * 2; });
  auto reg2 = callbacks.Register([&](int arg) { x += arg * 3; });

  REQUIRE(callbacks.Size() == 2);

  // x = 0;
  // x += 5 * 2;
  // x += 5 * 3;
  callbacks.CallAll(5);
  REQUIRE(x == 25);

  callbacks.Unregister(reg);
  REQUIRE(callbacks.Size() == 1);

  // x = 25;
  // x += 2 * 3;
  callbacks.CallAll(2);
  REQUIRE(x == 31);

  callbacks.Unregister(reg2);
  REQUIRE(callbacks.Size() == 0);

  callbacks.CallAll(3);
  REQUIRE(x == 31); // Should not have changed

  SECTION("Unregistering in registered callback") {
    x = 0;
    reg = callbacks.Register([&](int /* arg */) {
      x += 8;
      callbacks.Unregister(reg);
    });

    callbacks.CallAll(0);
    REQUIRE(x == 8);

    callbacks.CallAll(0);
    // Should not have changed because the callback should have unregistered
    // itself
    REQUIRE(x == 8);
  }
}
