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

#ifndef _CORE__READ_WRITE_H
#define _CORE__READ_WRITE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>
#include <core/file.h>

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

#define JSON_FLAGS rapidjson::kParseCommentsFlag |\
             rapidjson::kParseTrailingCommasFlag |\
             rapidjson::kParseNanAndInfFlag

template <typename K, typename Hash>
class FunctionalSelfMapping;

namespace JSON {
  using TypeManager = FunctionalSelfMapping<std::string, std::hash<std::string>>;

  using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

  struct Write {
    virtual void WriteToJSON(JSON::Writer &writer) const = 0;
  };

  struct Read {
    virtual bool ReadFromJSON(const rapidjson::Value &data, TypeManager &manager) = 0;
  };

  inline void WriteValue(Write &value, Writer &writer)
    { value.WriteToJSON(writer); }

  inline void WriteValue(const std::string &value, Writer &writer)
    { writer.String(value.c_str(), value.size()); }

  inline void WriteValue(double value, Writer &writer)
    { writer.Double(value); }

  inline void WriteValue(int value, Writer &writer)
    { writer.Int(value); }

  inline void WriteValue(unsigned value, Writer &writer)
    { writer.Uint(value); }

  template <typename T>
  inline T ReadValue(const rapidjson::Value &data, TypeManager &manager) {
    T value;
    value.ReadFromJSON(data, manager);
    return value;
  }

  template <>
  inline double ReadValue(const rapidjson::Value &data, TypeManager &/* manager */) {
    CHECK_RETURN(data.IsNumber(), "Must be a number", 0.0)
    return data.GetDouble();
  }

  template <>
  inline std::string ReadValue(const rapidjson::Value &data, TypeManager &/* manager */) {
    CHECK_RETURN(data.IsString(), "Must be a string", "")
    return data.GetString();
  }

  template <>
  inline int ReadValue(const rapidjson::Value &data, TypeManager &/* manager */) {
    CHECK_RETURN(data.IsInt(), "Must be an integer", 0)
    return data.GetInt();
  }

  template <>
  inline unsigned ReadValue(const rapidjson::Value &data, TypeManager &/* manager */) {
    CHECK_RETURN(data.IsUint(), "Must be an integer", 0)
    return data.GetUint();
  }

  inline rapidjson::Document GetDocument(const std::string &json) {
    rapidjson::Document document;
    document.Parse<JSON_FLAGS>(json.c_str());
    return document;
  }

  inline rapidjson::Document GetDocumentFromFile(const std::string &path) {
    std::string text;
    File::Read(path, text);
    return GetDocument(text);
  }

  template <typename Func>
  std::string WriteToString(Func func) {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{buffer};
    func(writer);
    return buffer.GetString();
  }

  struct ReadWrite : public Read, public Write {};
}

template <typename K, typename Hash = std::hash<K>>
class FunctionalSelfMapping {
  using LoadFunc = std::function<struct JSON::ReadWrite *(const rapidjson::Value &, FunctionalSelfMapping &)>;
  std::unordered_map<K, LoadFunc, Hash> map; 

public:
  void Register(const K &key, LoadFunc func)
    { map[key] = func; }
  
  LoadFunc Get(const K &key) const
    { return map.at(key); } 
};

#endif // _CORE__READ_WRITE_H
