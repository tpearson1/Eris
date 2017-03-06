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

#ifndef _SCENE__TAG_MANAGER_H
#define _SCENE__TAG_MANAGER_H

#include <unordered_map>
#include <string>
#include <base/ref.h>
#include <core/saveload.h>

class TagManager {
  std::unordered_map<std::string, SaveLoad *> taggedObjects;

public:
  SaveLoad *Get(const std::string &key) const
    { return taggedObjects.at(key); }

  template <typename T>
  T *Get(const std::string &key) const
    { return (T *)taggedObjects.at(key); }

  void Register(const std::string &key, SaveLoad *value)
    { taggedObjects[key] = value; }
  
  static Ref<TagManager> active;
};

/*
 * Function that can be registered for the ability to use the tag system in scene loading.
 * An example of a tagged object would be:
 *
 * [
 *   "Tagged",
 *   [
 *     { "tag": "player" },
 *     "NPlayer",
 *     { ... }  
 *   ]
 * ]
 */
SaveLoad *TagRegistration(const rapidjson::Value &val, JSONTypeManager &manager);

#endif // _SCENE__TAG_MANAGER_H