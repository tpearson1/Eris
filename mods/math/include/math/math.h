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

#ifndef _MATH__MATH_H
#define _MATH__MATH_H

#include <cmath>

namespace Math {
  inline float Radians(float degrees)
    { return degrees * M_PI / 180.0f; }
  inline float Degrees(float radians)
    { return radians * 180.0f / M_PI; }

  template <typename T>
  inline T Clamp(T value, T min = 0.0f, T max = 1.0f) {
    if (value < min) value = min;
    else if (value > max) value = max;
    return value;
  }

  template <typename T>
  inline T Min(T a, T b) { return (a < b) ? a : b; }
  template <typename T>
  inline T Max(T a, T b) { return (a > b) ? a : b; }

  template <typename T>
  inline T Lerp(T a, T b, T t) { return a + (b - a) * t; }
  template <typename T>
  inline T MoveTowards(T a, T b, T distance)
    { return Lerp(b, a, Clamp(distance / (b - a), -1.0f, 1.0f)); }
}

#endif // _MATH__MATH_H
