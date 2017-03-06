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

Ref<TagManager> TagManager::active;

SaveLoad *TagRegistration(const rapidjson::Value &val, JSONTypeManager &manager) {
  CHECK_RETURN(val.IsArray(), "'Tagged' object must be an array", nullptr)
  const auto &array = val.GetArray();

  CHECK_RETURN(array.Size() == 3, "'Tagged' object's array must be of size 3", nullptr)
  CHECK_RETURN(array[0].IsObject(), "The first element in a 'Tagged' object's array must be of type object", nullptr)
  CHECK_RETURN(array[1].IsString(), "The second element in a 'Tagged' object's array must be of type string", nullptr)

  const auto &tagData = array[0].GetObject();
  const auto &typeString = array[1].GetString();

  CHECK_RETURN(tagData.HasMember("tag"), "The first object in a 'Tagged' object's array must have member 'tag'", nullptr)
  const auto &tag = tagData["tag"];
  CHECK_RETURN(tag.IsString(), "The 'Tagged' object's 'tag' member must be of type string", nullptr)

  auto *object = manager.GetAssociatedFunction(typeString)(array[2], manager);

  if (TagManager::active)
    TagManager::active->Register(tag.GetString(), object);
  else
    ERROR("In order to load a 'Tagged' object, there should be an active TagManager object")

  return object;
}