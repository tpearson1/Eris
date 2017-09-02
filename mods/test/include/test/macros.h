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

#include <type_traits>

/*
 * Concatenates two tokens
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

// Used in template metaprogramming with SFINAE
template <typename... Exprs>
using Void = void;

/*
 * Sometimes helpful when using a comma confuses the preprocesssor
 */
#define COMMA_CHARACTER ,

/*
 * Macro which defines a template struct with a given name, containing a static
 * boolean member called 'value'. Member 'value' is 'true' if 'expr' is a valid
 * expression and 'false' otherwise. A template argument 'Type' is provided,
 * which can be used by the expression. Example use:
 *
 * IS_VALID_EXPR(HasFooMemberFunction, std::declval<Type>().Foo())
 *
 * template <typename T>
 * T CallFooIfExists() {
 *   T obj;
 *   if (HasFooMemberFunction<T>::value)
 *     obj.Foo();
 *   return obj;
 * }
 */
#define IS_VALID_EXPR(name, expr)\
template <typename Type, typename = void>\
struct name : public std::false_type {};\
template <typename Type>\
struct name<Type, Void<decltype(expr)>> : public std::true_type {};

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
#define STATIC_INIT \
static void CONCAT(SIH, __LINE__)();\
struct CONCAT(SI, __LINE__) {\
  CONCAT(SI, __LINE__)() { CONCAT(SIH, __LINE__)(); }\
} ANONYMOUS_VARIABLE(staticInitInst);\
static void CONCAT(SIH, __LINE__)()

#endif // _TEST__MACROS_H

