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

#include <array>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#endif

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <core/trace.h>

#include <test/macros.h>

#ifndef FUNC_NAME
#define FUNC_NAME __PRETTY_FUNTION__
#endif

namespace JSON {
using Document = rapidjson::Document;
using Value = rapidjson::Value;

using Array = Value::Array;
using ConstArray = Value::ConstArray;

using Object = Value::Object;
using ConstObject = Value::ConstObject;

using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

struct ReadData;

using TypeManager =
    std::unordered_map<std::string,
                       std::function<void *(const Value &, const ReadData &)>>;

struct ReadData {
  // Used to show where Read calls fail
  mutable Trace trace;

  std::shared_ptr<TypeManager> typeManager;

  ReadData(std::shared_ptr<TypeManager> manager)
      : typeManager(std::move(manager)) {}
};

struct ParseException : public std::exception {};

[[noreturn]] inline void ParseError(const ReadData &data,
                                    const std::string &message) {
  data.trace.Unwind(message);
  throw ParseException{};
}

inline void ParseFailIf(bool expr, const ReadData &data,
                        const std::string &message) {
  if (expr) ParseError(data, message);
}

inline void ParseAssert(bool expr, const ReadData &data,
                        const std::string &message) {
  if (!expr) ParseError(data, message);
}

constexpr const auto parseFlags = rapidjson::kParseCommentsFlag |
                                  rapidjson::kParseTrailingCommasFlag |
                                  rapidjson::kParseNanAndInfFlag;
}

/*
 * Class for Reading and Writing JSON to and from arbitrary classes. Either or
 * both functions below can be implemented as part of your partially specialized
 * class for your type 'T'.
 *
 * static T Read(const JSON::Value &value, const JSON::ReadData &data)
 * static void Write(const T &value, JSON::Writer &data)
 */
template <typename T>
struct JSONImpl;

template <typename T>
struct JSONImpl<T *> {
  static void Read(T *out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(const T *value, JSON::Writer &writer) {
    JSONImpl<T>::Write(*value, writer);
  }
};

template <>
struct JSONImpl<bool> {
  static void Read(bool &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(bool value, JSON::Writer &writer) { writer.Bool(value); }
};

template <>
struct JSONImpl<float> {
  static void Read(float &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(float value, JSON::Writer &writer) {
    writer.Double(static_cast<double>(value));
  }
};

template <>
struct JSONImpl<double> {
  static void Read(double &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(double value, JSON::Writer &writer) {
    writer.Double(value);
  }
};

template <>
struct JSONImpl<int> {
  static void Read(int &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(int value, JSON::Writer &writer) { writer.Int(value); }
};

template <>
struct JSONImpl<unsigned> {
  static void Read(unsigned &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(unsigned value, JSON::Writer &writer) {
    writer.Uint(value);
  }
};

template <>
struct JSONImpl<std::size_t> {
  static void Read(std::size_t &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(std::size_t value, JSON::Writer &writer) {
    writer.Uint(value);
  }
};

template <>
struct JSONImpl<std::string> {
  static void Read(std::string &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(const std::string &value, JSON::Writer &writer) {
    writer.String(value);
  }
};

template <typename T, std::size_t Size>
struct JSONImpl<std::array<T, Size>> {
  static void Read(std::array<T, Size> &out, const JSON::Value &value,
                   const JSON::ReadData &data) {
    auto t = Trace::Pusher{data.trace, "std::array"};

    JSON::ParseAssert(value.IsArray(), data, "Must be an array");
    const auto &arr = value.GetArray();
    JSON::ParseAssert(arr.Size() == Size, data, "Incorrect size for array");

    for (auto i = 0u; i < Size; i++) JSONImpl<T>::Read(out[i], arr[i], data);
  }

  static void Write(const std::array<T, Size> &value, JSON::Writer &writer) {
    writer.StartArray();
    for (auto &&elem : value) JSONImpl<T>::Write(elem, writer);
    writer.EndArray();
  }
};

template <typename T>
struct JSONImpl<std::vector<T>> {
  static void Read(std::vector<T> &out, const JSON::Value &value,
                   const JSON::ReadData &data) {
    auto t = Trace::Pusher{data.trace, "std::vector"};

    JSON::ParseAssert(value.IsArray(), data, "Must be an array");
    const auto &arr = value.GetArray();

    auto newSize = out.size() + arr.Size();
    out.reserve(newSize);
    for (auto &&elem : arr) {
      T v;
      JSONImpl<T>::Read(v, elem, data);
      out.push_back(v);
    }
  }

  static void Write(const std::vector<T> &value, JSON::Writer &writer) {
    writer.StartArray();
    for (auto &&elem : value) JSONImpl<T>::Write(elem, writer);
    writer.EndArray();
  }
};

template <typename V, typename Hash>
struct JSONImpl<std::unordered_map<std::string, V, Hash>> {
  static void Read(std::unordered_map<std::string, V, Hash> &out,
                   const JSON::Value &value, const JSON::ReadData &data) {
    auto t = Trace::Pusher{data.trace, "std::unordered_map"};

    JSON::ParseAssert(value.IsObject(), data, "Mapping must be an object");
    const auto &object = value.GetObject();

    for (auto &&member : object) {
      V v;
      JSONImpl<V>::Read(v, member.value, data);
      out[member.name.GetString()] = v;
    }
  }

  static void Write(const std::unordered_map<std::string, V, Hash> &value,
                    JSON::Writer &writer) {
    writer.StartObject();
    for (auto &&elem : value) {
      JSONImpl<std::string>::Write(elem.first, writer);
      JSONImpl<V>::Write(elem.second, writer);
    }
    writer.EndObject();
  }
};

template <typename T>
struct JSONImpl<std::unique_ptr<T>> {
  static void Read(std::unique_ptr<T> &out, const JSON::Value &value,
                   const JSON::ReadData &data) {
    auto t = Trace::Pusher{data.trace, "std::unique_ptr"};
    out = std::make_unique<T>();
    JSONImpl<T>::Read(*out, value, data);
  }

  static void Write(const std::unique_ptr<T> &value, JSON::Writer &writer) {
    JSONImpl<T>::Write(*value, writer);
  }
};

template <typename T>
struct JSONImpl<std::shared_ptr<T>> {
  static void Read(std::shared_ptr<T> &out, const JSON::Value &value,
                   const JSON::ReadData &data) {
    auto t = Trace::Pusher{data.trace, "std::shared_ptr"};
    out = std::make_shared<T>();
    JSONImpl<T>::Read(*out, value, data);
  }

  static void Write(const std::shared_ptr<T> &value, JSON::Writer &writer) {
    JSONImpl<T>::Write(*value, writer);
  }
};

namespace JSON {
template <typename T>
T Read(const Value &value, const ReadData &data) {
  // We do not push and pop the trace for data because this is merely a
  // helper function and would not provide much useful debug information
  T v;
  JSONImpl<T>::Read(v, value, data);
  return v;
}

template <typename T>
void Read(T &out, const Value &value, const ReadData &data) {
  JSONImpl<T>::Read(out, value, data);
}

/*
 * Helper function for JSONImpl<T>::Write
 */
template <typename T>
void Write(const T &value, Writer &writer) {
  JSONImpl<T>::Write(value, writer);
}


IS_VALID_EXPR(ImplementsReadImpl, JSONImpl<Type>::Read)
IS_VALID_EXPR(ImplementsWriteImpl, JSONImpl<Type>::Write)

/*
 * These will be false for reference types because
 * JSONImpl<T &> is not specialized by default
 */
template <typename T>
inline constexpr bool implementsRead = ImplementsReadImpl<T>::value;

template <typename T>
inline constexpr bool implementsWrite = ImplementsWriteImpl<T>::value;

/*
 * Write a JSON key-value pair
 */
template <typename T>
void WritePair(const std::string &key, const T &value, Writer &writer) {
  JSONImpl<std::string>::Write(key, writer);
  JSONImpl<T>::Write(value, writer);
}

class ObjectEncloser {
  Writer &writer;

public:
  ObjectEncloser(Writer &_writer) : writer(_writer) { writer.StartObject(); }

  ~ObjectEncloser() { writer.EndObject(); }
};

template <typename ObjectFunction>
void WriteObject(const std::string &key, Writer &writer, ObjectFunction func) {
  JSONImpl<std::string>::Write(key, writer);
  writer.StartObject();
  func();
  writer.EndObject();
}

class ArrayEncloser {
  Writer &writer;

public:
  ArrayEncloser(Writer &_writer) : writer(_writer) { writer.StartArray(); }

  ~ArrayEncloser() { writer.EndArray(); }
};

template <typename ArrayFunction>
void WriteArray(const std::string &key, Writer &writer, ArrayFunction func) {
  JSONImpl<std::string>::Write(key, writer);
  writer.StartArray();
  func();
  writer.EndArray();
}

template <typename T>
void GetMember(T &out, const std::string &memberName, const ConstObject &object,
               const ReadData &data) {
  auto t = Trace::Pusher{data.trace, "JSON::GetMember"};

  const auto memIt = object.FindMember(memberName);
  if (memIt == object.MemberEnd())
    ParseError(data, "Member '" + memberName + "' expected");
  JSONImpl<T>::Read(out, memIt->value, data);
}

template <typename T>
T GetMember(const std::string &memberName, const ConstObject &object,
            const ReadData &data) {
  T t;
  GetMember<T>(t, memberName, object, data);
  return t;
}

template <typename T>
void TryGetMember(T &out, const std::string &memberName,
                  const ConstObject &object, const T &fallback,
                  const ReadData &data) {
  auto t = Trace::Pusher{data.trace, "JSON::TryGetMember"};

  const auto memIt = object.FindMember(memberName);
  if (memIt == object.MemberEnd())
    out = fallback;
  else
    JSONImpl<T>::Read(out, memIt->value, data);
}

template <typename T>
T TryGetMember(const std::string &memberName, const ConstObject &object,
               const T &fallback, const ReadData &data) {
  T t;
  TryGetMember<T>(t, memberName, object, fallback, data);
  return t;
}

ConstObject GetObject(const Value &value, const ReadData &data);

ConstArray GetArray(const Value &value, const ReadData &data);

template <typename Key, typename Value, typename AssocContainer>
Value GetAssociated(const AssocContainer &cont, const Key &key,
                    const ReadData &data) {
  auto t = Trace::Pusher{data.trace, "JSON::GetAssociated"};
  auto it = cont.find(key);
  if (it == std::end(cont))
    ParseError(data, "Invalid value for string for conversion to enumerable");
  return it->second;
}

template <typename Key, typename Value, typename AssocContainer>
void GetAssociated(Value &out, const AssocContainer &cont, const Key &key,
                   const ReadData &data) {
  out = GetAssociated<Key, Value, AssocContainer>(cont, key, data);
}

void GetData(Document &out, const std::string &json);

void GetDataFromFile(Document &out, const std::string &path);

template <typename Func>
std::string WriteToString(Func func) {
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{buffer};
  func(writer);
  return buffer.GetString();
}
}

#endif // _CORE__READ_WRITE_H
