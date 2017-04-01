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

#include <canvasitem.h>
#include <base/window.h>
#include <scene/renderer.h>

void CanvasItem::RecursiveDestroy(CanvasItem *c) {
  c->OnDestroy();
  for (auto &elem : c->transform)
    RecursiveDestroy(elem);
  c->transform.Parent(nullptr);
  delete c;
}

void CanvasItem::WriteToJSON(JSON::Writer &writer) {
  writer.StartObject();
    writer.String("visible", strlen("visible"));
    writer.Bool(visible);

    writer.String("transform", strlen("transform"));
    transform.WriteToJSON(writer);
  writer.EndObject();
}

bool CanvasItem::ReadFromJSON(const rapidjson::Value &data) {
  PARSE_CHECK(data.IsObject(), "Type 'CanvasItem' must be an object")
  auto object = data.GetObject();

  PARSE_CHECK(object.HasMember("visible"), "'CanvasItem' object must have member 'visible'")
  auto &visVal = object["visible"];
  PARSE_CHECK(visVal.IsBool(), "Member 'visible' in 'CanvasItem' object must be of type bool")
  visible = visVal.GetBool();

  PARSE_CHECK(object.HasMember("transform"), "'CanvasItem' object must have member 'transform'")
  transform.node = this;
  return transform.ReadFromJSON(object["transform"]);
}
