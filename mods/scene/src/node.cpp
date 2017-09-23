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
  for (auto *child : *n) RecursiveDestroy(child);
  // Make sure the parent isn't holding on to this node
  n->Parent(nullptr);

  delete n;
}

Vec3 NNode::GlobalLocation() const {
  Vec3 pos;
  const NNode *cur = this;
  const NNode *par = Parent();
  while (par) {
    auto &pt = par->transform, &ct = cur->transform;
    pos += pt.Rotation() * (ct.Location() * pt.Scale());
    cur = par;
    par = par->Parent();
  }
  return pos + cur->transform.Location();
}

Quat NNode::GlobalRotation() const {
  Quat rot;
  const NNode *cur = this;
  const NNode *par;
  do {
    rot *= cur->transform.Rotation();
    par = cur->Parent();
    if (par) cur = par;
  } while (par);
  return rot;
}

Vec3 NNode::GlobalScale() const {
  Vec3 scale{1.0f, 1.0f, 1.0f};
  const NNode *cur = this;
  const NNode *par;
  do {
    scale *= cur->transform.Scale();
    par = cur->Parent();
    if (par) cur = par;
  } while (par);
  return scale;
}

Transform NNode::GlobalTransform() const {
  Transform t;
  const NNode *cur = this;
  const NNode *par;
  do {
    t *= cur->transform;
    par = cur->Parent();
    if (par) cur = par;
  } while (par);
  return t;
}

void JSONImpl<NNode>::Write(const NNode &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
  JSON::WritePair("transform", value.transform, writer);
  JSON::WritePair("children", value.children, writer);
}

void JSONImpl<NNode>::Read(NNode &out, const JSON::Value &value,
                           const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "NNode"};

  const auto &object = JSON::GetObject(value, data);
  JSON::GetMember(out.transform, "transform", object, data);

  auto childrenIt = object.FindMember("children");
  if (childrenIt == object.MemberEnd()) return;

  JSON::ParseAssert(childrenIt->value.IsArray(), data,
                    "Member 'children' of 'Transform' must be of type array");

  const auto &childrenArr = childrenIt->value.GetArray();
  for (auto it = childrenArr.Begin(); it != childrenArr.End(); it++) {
    const auto type = JSON::Read<std::string>(*it, data);

    JSON::ParseAssert(++it != childrenArr.End(), data,
                      "Array 'children' of 'Transform' must have data after "
                      "each type string");

    auto func = data.typeManager->at(type);
    reinterpret_cast<NNode *>(func(*it, data))->Parent(&out);
  }
}
