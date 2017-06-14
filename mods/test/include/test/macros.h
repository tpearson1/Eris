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

#ifndef _TEST__MACROS_H
#define _TEST__MACROS_H

/*
 * Concatenated two tokens
 */
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

/*
 * Converts the token into a string literal
 */
#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)

/*
 * Macro which (ideally) increments each time it is used
 */
#ifdef __COUNTER__
  #define COUNTER __COUNTER__
#else
  #define COUNTER __LINE__
#endif

/*
 * Suffixes 'str' with a counter, to create an anonymous variable.
 * Not guaranteed to have a unique name to other uses of this macro in other
 * translation units
 */
#define ANONYMOUS_VARIABLE(str) CONCAT(str, COUNTER)

/*
 * Provided a block of code or a single statement, the code will be executed
 * when the library / binary is loaded
 */
#define STATIC_INIT(code)\
struct CONCAT(StaticInit_, __LINE__) { CONCAT(StaticInit_, __LINE__)() { code }\
} ANONYMOUS_VARIABLE(staticInitInst);

#endif // _TEST__MACROS_H

