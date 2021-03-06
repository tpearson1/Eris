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

#include <tagmanager.h>
#include <iostream>

TagManager *TagManager::active = nullptr;

void *TaggedTypeRegistration(const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "TaggedTypeRegistration"};
  const auto &array = JSON::GetArray(value, data);

  JSON::ParseAssert(array.Size() == 3, data, "'Tagged' object's array must be of size 3");
  JSON::ParseAssert(array[1].IsString(), data, "The second element in a 'Tagged' object's array must be of type string");

  const auto &tagData = JSON::GetObject(array[0], data);
  const auto typeString = JSON::Read<std::string>(array[1], data);

  const auto tag = JSON::GetMember<std::string>("tag", tagData, data);

  auto it = data.typeManager->find(typeString);
  if (it == std::end(*data.typeManager)) {
    std::cerr << "Tagged type '" << typeString << "' has not been registered\n";
    return nullptr;
  }

  void *object = it->second(array[2], data);

  JSON::ParseAssert(TagManager::active, data, "In order to load a 'Tagged' object, there should be an active TagManager object");
  TagManager::active->map[tag] = object;

  return object;
}
