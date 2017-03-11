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

#ifndef _CORE__SAVE_LOAD_H
#define _CORE__SAVE_LOAD_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>

#define PARSE_ERROR(message) {\
  std::cerr << "> JSON: error: " << (message) << '\n';\
  return false;\
}

#define PARSE_FAIL_IF(expr, failMessage) {\
  if (expr)\
    PARSE_ERROR((failMessage))\
}

#define PARSE_CHECK(expr, failMessage) PARSE_FAIL_IF(!(expr), failMessage)

#define CHECK_RETURN(condition, string, ret) \
{ \
  if (!(condition)) \
    { ERROR(string) return (ret); } \
}

#define CHECK(condition, string) \
{ \
  if (!(condition)) \
    ERROR(string) \
}


#define ERROR(string) std::cerr << "> " << string << '\n';

template <typename K, typename Hash = std::hash<K>>
class FunctionalSelfMapping {
  using LoadFunc = std::function<struct SaveLoad *(const rapidjson::Value &, FunctionalSelfMapping &)>;
  std::unordered_map<K, LoadFunc, Hash> map; 

public:
  void Register(const K &key, LoadFunc func)
    { map[key] = func; }
  
  LoadFunc Get(const K &key) const
    { return map.at(key); } 
};

using JSONTypeManager = FunctionalSelfMapping<std::string>;

struct Save {
  using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

  virtual void SerializeToJSON(Writer &writer) const = 0;
};

struct Load {
  virtual bool LoadFromJSON(const rapidjson::Value &data, JSONTypeManager &manager) = 0;
};

struct SaveLoad : public Save, public Load {};

#endif // _CORE__SAVE_LOAD_H
