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

#ifndef _TEST__TEST_H
#define _TEST__TEST_H

#include <vector>
#include <iostream>
#include <utility>
#include <test/macros.h>

namespace Test {
  template <typename T>
  struct StaticRegistrator {
    static std::vector<T> registered;
    StaticRegistrator(const T &val) { registered.push_back(val); }
  };

  template <typename T>
  std::vector<T> StaticRegistrator<T>::registered;

  using StandardRegistrator = StaticRegistrator<std::pair<std::string, void (*)()>>;

  struct UnsatisfiedRequire {
    std::string conditionString;
    std::string file;
    std::string line;
  };

  inline std::ostream &operator<<(std::ostream &os, const UnsatisfiedRequire &require) {
    os << "Condition '" << require.conditionString << "' unsatisfied: "
       << require.file << " line " << require.line;
    return os;
  }

  inline bool CallTestCase(const std::string &name, void (*test)()) {
    try { test(); }
    catch (Test::UnsatisfiedRequire error) {
      std::cerr << "Test case '" << name << "': " << error << '\n';
      return false;
    }
    return true;
  }

  // failTolerance of 0 means it runs through all tests even if others fail
  inline int RunTests(int failTolerance = 1) {
    const auto &r = StandardRegistrator::registered;
    auto fails = 0;
    for (auto &test : r) {
      if (CallTestCase(test.first, test.second))
        continue;

      if (++fails == failTolerance) {
        std::cerr << "Fail Tolerance exceeded. Finishing\n";
        return 1;
      }
    }

    std::cout << "Testing finished. " << (r.size() - fails) << '/' << r.size()
              << " tests were successful\n";
    return 0;
  }
}

#define REQUIRE(value) {\
  if (!(value))\
    throw Test::UnsatisfiedRequire{STR(value), __FILE__, STR(__LINE__)};\
}

#define TEST_CASE(name, code)\
  static auto ANONYMOUS_VARIABLE(testCase) = Test::StandardRegistrator{{(name), [] code }};

#define GENERATE_TEST_MAIN(failTolerance)\
int main() { return Test::RunTests(failTolerance); }

#endif // _TEST__TEST_H

