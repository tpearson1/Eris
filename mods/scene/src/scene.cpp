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

#include <scene.h>
#include <camera.h>
#include <tagmanager.h>
#include <model.h>

void Scene::WriteToJSON(JSON::Writer &writer) const {
  writer.StartObject();
    writer.String("nodes", strlen("nodes"));
    writer.StartArray();
      for (auto &elem : root.transform)
        elem->WriteToJSON(writer);
    writer.EndArray();
  writer.EndObject();
}

bool Scene::ReadFromJSON(const rapidjson::Value &data, JSON::TypeManager &manager) {
  PARSE_CHECK(data.IsObject(), "Type 'Scene' must be an object")
  const auto &object = data.GetObject();

  PARSE_CHECK(object.HasMember("nodes"), "'Scene' object must have member 'nodes'")
  const auto &nodesVal = object["nodes"];
  PARSE_CHECK(nodesVal.IsArray(), "Member 'nodes' in 'Scene' object must be of type array")
  const auto &nodesArr = nodesVal.GetArray();

  for (auto it = nodesArr.Begin(); it != nodesArr.End(); it++) {
    PARSE_CHECK(it->IsString(), "Member 'nodes' in 'Scene' object must alternate between type string and another type")
    std::string string = it->GetString();

    PARSE_CHECK(++it != nodesArr.End(), "Member 'nodes' in 'Scene' object must have data after each type string")

    // Call a function depending on what the value of the string is.
    // This function returns a newly created node pointer using the JSON data.
    // This is then parented to the root node
    const auto &func = manager.Get(string);
    ((NNode *)func(*it, manager))->transform.Parent(&root);
  }

  return true;
}

void RegisterSceneTypeAssociations(JSON::TypeManager &manager) {
  manager.Register("NNode", DefaultTypeRegistration<NNode>);
  manager.Register("NCamera", DefaultTypeRegistration<NCamera>);
  manager.Register("Tagged", TagRegistration);
  manager.Register("Model", ModelRegistration);
}
