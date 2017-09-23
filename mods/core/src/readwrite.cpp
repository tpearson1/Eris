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

#include <core/file.h>
#include <readwrite.h>

template <typename T>
void JSONImpl<T *>::Read(T *out, const JSON::Value &value,
                         const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "T *"};
  JSONImpl<T>::Read(*out, value, data);
}

void JSONImpl<bool>::Read(bool &out, const JSON::Value &value,
                          const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "bool"};
  JSON::ParseAssert(value.IsBool(), data, "Must be a boolean");
  out = value.GetBool();
}

void JSONImpl<float>::Read(float &out, const JSON::Value &value,
                           const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "float"};
  JSON::ParseAssert(value.IsNumber(), data, "Must be a number");
  out = value.GetFloat();
}

void JSONImpl<double>::Read(double &out, const JSON::Value &value,
                            const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "double"};
  JSON::ParseAssert(value.IsNumber(), data, "Must be a number");
  out = value.GetDouble();
}

void JSONImpl<std::string>::Read(std::string &out, const JSON::Value &value,
                                 const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "std::string"};
  JSON::ParseAssert(value.IsString(), data, "Must be a string");
  out = value.GetString();
}

void JSONImpl<int>::Read(int &out, const JSON::Value &value,
                         const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "int"};
  JSON::ParseAssert(value.IsInt(), data, "Must be an integer");
  out = value.GetInt();
}

void JSONImpl<unsigned>::Read(unsigned &out, const JSON::Value &value,
                              const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "unsigned"};
  JSON::ParseAssert(value.IsUint(), data, "Must be an unsigned integer");
  out = value.GetUint();
}

void JSONImpl<std::size_t>::Read(std::size_t &out, const JSON::Value &value,
                                 const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "std::size_t"};
  JSON::ParseAssert(value.IsUint(), data, "Must be an unsigned integer");
  out = value.GetUint();
}

JSON::ConstObject JSON::GetObject(const JSON::Value &value,
                                  const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "JSON::GetObject"};

  JSON::ParseAssert(value.IsObject(), data, "Must be an object");
  return value.GetObject();
}

JSON::ConstArray JSON::GetArray(const JSON::Value &value,
                                const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "JSON::GetArray"};

  JSON::ParseAssert(value.IsArray(), data, "Must be an array");
  return value.GetArray();
}

void JSON::GetData(JSON::Document &out, const std::string &json) {
  if (out.Parse<JSON::parseFlags>(json.c_str()).HasParseError())
    throw JSON::ParseException{};
}

void JSON::GetDataFromFile(JSON::Document &out, const std::string &path) {
  std::string text;
  File::Read(path, text);
  JSON::GetData(out, text);
}
