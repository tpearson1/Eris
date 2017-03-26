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
#include <core/ref.h>

template <typename K, typename V, typename Hash = std::hash<K>>
class Mapping {
protected:
  std::unordered_map<K, V, Hash> map;

public:
  void Register(const K &key, V val)
    { map[key] = val; }

  V Get(const K &key) const
    { return map.at(key); }

  using iterator = typename std::unordered_map<std::string, V, Hash>::iterator;
  using const_iterator = typename std::unordered_map<std::string, V, Hash>::const_iterator;

  iterator begin()
    { return map.begin(); }
  const_iterator begin() const
    { return map.begin(); }

  iterator end()
    { return map.end(); }
  const_iterator end() const
    { return map.end(); }
};

template <typename V, typename Hash = std::hash<std::string>>
class SerializableMapping : public SaveLoad {
  std::unordered_map<std::string, V, Hash> map;

public:
  void Register(const std::string &key, V val)
    { map[key] = val; }

  V Get(const std::string &key) const
    { return map.at(key); }

  using iterator = typename std::unordered_map<std::string, V, Hash>::iterator;
  using const_iterator = typename std::unordered_map<std::string, V, Hash>::const_iterator;

  iterator begin()
    { return map.begin(); }
  const_iterator begin() const
    { return map.begin(); }

  iterator end()
    { return map.end(); }
  const_iterator end() const
    { return map.end(); }

  virtual void SerializeToJSON(Save::Writer &writer) const override {
    writer.StartObject();
    for (auto &pair : map) {
      writer.String(pair.first.c_str(), pair.first.size());
      Save::SerializeValue(pair.second, writer);
    }
    writer.EndObject();
  }

  virtual bool LoadFromJSON(const rapidjson::Value &data, JSONTypeManager &manager) override {
    PARSE_CHECK(data.IsObject(), "Mapping must be an object")
    const auto &object = data.GetObject();

    for (auto it = object.MemberBegin(); it != object.MemberEnd(); it++) {
      const auto &name = it->name.GetString();
      const auto &value = it->value;
      V val = LoadValue<V>(value, manager);
      Register(name, val);
    }
    return true;
  }
};

template <typename Value, typename Data, typename Hash = std::hash<std::string>>
class DeferredLoadableMapping : public Load {
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
    // We assume that jsonIt is a valid iterator
    Value val;
    val.Load(dataIt->second);
    
    notLoaded.erase(dataIt);
    values[key] = val;
    return val;
  }

  virtual bool LoadFromJSON(const rapidjson::Value &data, JSONTypeManager &manager) override {
    PARSE_CHECK(data.IsObject(), "Mapping must be an object")
    const auto &object = data.GetObject();

    for (auto it = object.MemberBegin(); it != object.MemberEnd(); it++) {
      const auto &name = it->name.GetString();
      auto &value = it->value;
      Data d;
      d.LoadFromJSON(value, manager);
      Register(name, d);
    }
    return true;
  }
};

#endif // _CORE__MAPPING_H
