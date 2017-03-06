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

#include <node.h>
#include <renderer.h>

void NNode::RecursiveDestroy(NNode *n) {
  n->OnDestroy();
  for (auto &elem : n->transform)
    RecursiveDestroy(elem);
  n->transform.Parent(nullptr); // Make sure the parent isn't holding on to this node
  delete n;
}

void NNode::SerializeToJSON(Writer &writer) const {
  writer.String("NNode", strlen("NNode"));
  writer.StartObject();
    writer.String("visible", strlen("visible"));
    writer.Bool(visible);

    writer.String("transform", strlen("transform"));
    transform.SerializeToJSON(writer);
  writer.EndObject();
}

bool NNode::LoadFromJSON(const rapidjson::Value &data, JSONTypeManager &manager) {
  PARSE_CHECK(data.IsObject(), "Type 'NNode' must be an object")
  auto object = data.GetObject();

  if (object.HasMember("visible")) {
    auto &visVal = object["visible"];
    PARSE_CHECK(visVal.IsBool(), "Member 'visible' in 'NNode' object must be of type bool")
    visible = visVal.GetBool();
  }
  else
    visible = true;
  
  PARSE_CHECK(object.HasMember("transform"), "'NNode' object must have member 'transform'")
  transform.node = this;
  return transform.LoadFromJSON(object["transform"], manager);
}
