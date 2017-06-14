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

#ifndef _CORE__MAPPING_H
#define _CORE__MAPPING_H

#include <unordered_map>
#include <core/readwrite.h>

/*
 * Helper class to call Load correctly on a value type which may be a smart pointer
 */
template <typename Value, typename Data>
struct LoadCallHelper {
  // Nothing special - base case
  static void Load(Value &value, const Data &data)
    { value.Load(data); }
};

template <typename ValueObj, typename Data>
struct LoadCallHelper<std::shared_ptr<ValueObj>, Data> {
  // Shared pointer
  static void Load(std::shared_ptr<ValueObj> &value, const Data &data) {
    value = std::make_shared<ValueObj>();
    value->Load(data);
  }
};

template <typename ValueObj, typename Data>
struct LoadCallHelper<std::unique_ptr<ValueObj>, Data> {
  // Unique pointer
  static void Load(std::unique_ptr<ValueObj> &value, const Data &data) {
    value = std::make_unique<ValueObj>();
    value->Load(data);
  }
};

template <typename Value, typename Data, typename Hash = std::hash<std::string>>
class JSONDeferredReadMapping {
  std::unordered_map<std::string, Value, Hash> values;
  std::unordered_map<std::string, Data, Hash> notLoaded;

public:
  void Register(const std::string &key, Value val)
    { values[key] = val; }
  void Register(const std::string &key, Data data)
    { notLoaded[key] = data; }

  Value Get(const std::string &key) {
    auto it = values.find(key);
    if (it != values.end())
      return it->second;

    auto dataIt = notLoaded.find(key);
    // We assume that dataIt is a valid iterator
    Value val;
    // Value type may be a smart pointer. This helper class handles that correctly
    LoadCallHelper<Value, Data>::Load(val, dataIt->second);

    notLoaded.erase(dataIt);
    values[key] = val;
    return val;
  }

  auto &GetLoaded() { return values; }
  auto &GetDeferred() { return notLoaded; }
};

template <typename Value, typename Data, typename Hash>
struct JSONImpl<JSONDeferredReadMapping<Value, Data, Hash>> {
  static void Read(JSONDeferredReadMapping<Value, Data, Hash> &out, const JSON::Value &value, const JSON::ReadData &data) {
    auto t = Trace::Pusher{data.trace, "JSONDeferredMapping"};
    JSON::Read(out.GetDeferred(), value, data);
  }
};

#endif // _CORE__MAPPING_H
