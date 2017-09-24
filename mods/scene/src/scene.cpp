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

#include <camera.h>
#include <lightmanager.h>
#include <meshload.h>
#include <scene.h>
#include <tagmanager.h>

void JSONImpl<Scene>::Read(Scene &out, const JSON::Value &value,
                           const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Scene"};

  const auto &object = JSON::GetObject(value, data);

  auto nodesIt = object.FindMember("nodes");
  JSON::ParseAssert(nodesIt != object.MemberEnd(), data,
                    "'Scene' object must have member 'nodes'");
  JSON::ParseAssert(nodesIt->value.IsArray(), data,
                    "Member 'nodes' in 'Scene' object must be of type array");

  const auto &nodesArr = nodesIt->value.GetArray();
  for (auto it = nodesArr.Begin(); it != nodesArr.End(); it++) {
    const auto type = JSON::Read<std::string>(*it, data);

    JSON::ParseAssert(++it != nodesArr.End(), data, "Member 'nodes' in 'Scene' "
                                                    "object must have data "
                                                    "after each type string");

    const auto funcIt = data.typeManager->find(type);
    if (funcIt == std::end(*data.typeManager)) {
      std::cerr << "Type '" << type
                << "' is not registered in the current TypeManager\n";
      return;
    }

    // Call the registered function.
    // This function returns a newly created node pointer using the JSON data.
    // This is then parented to the root node
    const auto &func = funcIt->second;
    reinterpret_cast<NNode *>(func(*it, data))->Parent(&out.root);
  }
}

void RegisterSceneTypeAssociations(JSON::TypeManager &manager) {
  manager["NNode"] = DefaultNodeTypeRegistration<NNode>;
  manager["NCamera"] = DefaultNodeTypeRegistration<NCamera>;
  manager["NPointLight"] = DefaultNodeTypeRegistration<NPointLight>;
  manager["NDirectionalLight"] = DefaultNodeTypeRegistration<NDirectionalLight>;
  manager["Tagged"] = TaggedTypeRegistration;
  manager["NMesh"] = MeshTypeRegistration;
}
