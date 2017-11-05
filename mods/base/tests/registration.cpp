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

#include <list>
#include <registration.h>

TEST_CASE("Registration with std::list", "[Registration]") {
  std::list<int> integers;
  using Reg = Registration<std::list<int>>;

  {
    auto reg5 = Reg{5, integers};
    auto reg7 = Reg{7, integers};
    REQUIRE(integers.size() == 2);

    REQUIRE(reg5.Registered());
    REQUIRE(reg7.Registered());

    reg5.Unregister();
    REQUIRE(integers.size() == 1);
  }

  REQUIRE(integers.size() == 0);

  SECTION("Moving object") {
    auto reg = Reg{17, integers};

    auto newReg = std::move(reg);
    REQUIRE(newReg.Registered());

    newReg.Unregister();
    REQUIRE(!newReg.Registered());
  }

  REQUIRE(integers.size() == 0);
}

TEST_CASE("Registration with CallbackList", "[Registration]") {
  CallbackList<void(), false> callbacks;
  using Reg = Registration<decltype(callbacks)>;

  auto id = [] {};

  auto reg = Reg{id, callbacks};
  REQUIRE(reg.Registered());
  REQUIRE(callbacks.Size() == 1);

  {
    auto reg5 = Reg{id, callbacks};
    auto reg7 = Reg{id, callbacks};

    REQUIRE(reg5.Registered());
    REQUIRE(reg7.Registered());
    REQUIRE(callbacks.Size() == 3);

    reg5.Unregister();
    REQUIRE(callbacks.Size() == 2);
  }

  REQUIRE(callbacks.Size() == 1);

  SECTION("Moving Registration") {
    auto oldReg = Reg{id, callbacks};

    auto newReg = std::move(oldReg);
    REQUIRE(newReg.Registered());

    newReg.Unregister();
    REQUIRE(!newReg.Registered());
  }

  reg.Unregister();
  REQUIRE(!reg.Registered());

  REQUIRE(callbacks.Size() == 0);
}
