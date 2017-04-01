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

std::vector<NCamera *> NCamera::all;
NCamera *NCamera::active = nullptr;

NCamera::~NCamera() {
  auto it = std::find(all.begin(), all.end(), this);
  if (it != all.end())
    all.erase(it);
}

Mat4 NCamera::ProjectionMatrix() const {
  float aspect = (float)Window::inst->width / (float)Window::inst->height;
  if (perspective)
    return Mat4::PerspectiveFOV(fov, aspect, near, far);
  else {
    aspect *= currentZoom;
    return Mat4::Ortho(-aspect, aspect, -currentZoom, currentZoom, near, far);
  }
}

Mat4 NCamera::ViewMatrix() const {
  Vec3 loc = -transform.GlobalLocation();
  Quat rot = transform.GlobalRotation();
  return Mat4::LookAt(loc, loc + rot * Vec3::front, rot * Vec3(0.0f, 1.0f, 0.0f));
}

void NCamera::WriteToJSON(JSON::Writer &writer) const {
  writer.String("NCamera", strlen("NCamera"));
  writer.StartObject();
    NNode::WriteToJSON(writer);

    writer.String("active", strlen("active"));
    writer.Bool(active == this);

    writer.String("perspective", strlen("perspective"));
    if (perspective) {
      writer.Bool(true);

      writer.String("fov", strlen("fov"));
      writer.Double(static_cast<double>(fov));
    }
    else {
      writer.Bool(false);

      writer.String("min-zoom", strlen("min-zoom"));
      writer.Double(static_cast<double>(minZoom));
      writer.String("max-zoom", strlen("max-zoom"));
      writer.Double(static_cast<double>(maxZoom));
      writer.String("zoom", strlen("zoom"));
      writer.Double(static_cast<double>(currentZoom));
    }

    writer.String("near", strlen("near"));
    writer.Double(static_cast<double>(near));
    writer.String("far", strlen("far"));
    writer.Double(static_cast<double>(far));
  writer.EndObject();
}

bool NCamera::ReadFromJSON(const rapidjson::Value &data, JSON::TypeManager &manager) {
  PARSE_CHECK(data.IsObject(), "Type 'NCamera' must be an object")
  auto object = data.GetObject();

  PARSE_CHECK(object.HasMember("near"), "'NCamera' object must have member 'near'")
  auto &nearVal = object["near"];
  PARSE_CHECK(nearVal.IsNumber(), "Member 'near' of 'NCamera' must be of type float")
  near = nearVal.GetFloat();

  PARSE_CHECK(object.HasMember("far"), "'NCamera' object must have member 'far'")
  auto &farVal = object["far"];
  PARSE_CHECK(farVal.IsNumber(), "Member 'far' of 'NCamera' must be of type float")
  far = farVal.GetFloat();

  PARSE_CHECK(object.HasMember("perspective"), "'NCamera' object must have member 'perspective'")
  auto &perspVal = object["perspective"];
  PARSE_CHECK(perspVal.IsBool(), "Member 'perspective' of 'NCamera' must be of type bool")
  perspective = perspVal.GetBool();

  if (perspective) {
    PARSE_CHECK(object.HasMember("fov"), "Because the 'NCamera' object has member 'perspective' set to true, it must have member 'fov'")
    auto &fovVal = object["fov"];
    PARSE_CHECK(fovVal.IsNumber(), "Member 'fov' of 'NCamera' must be of type float")
    fov = fovVal.GetFloat();
  }
  else {
    PARSE_CHECK(object.HasMember("min-zoom"), "Because the 'NCamera' object has member 'perspective' set to false, it must have member 'min-zoom'")
    auto &minZoomVal = object["min-zoom"];
    PARSE_CHECK(minZoomVal.IsNumber(), "Member 'min-zoom' of 'NCamera' must be of type float")
    minZoom = minZoomVal.GetFloat();

    PARSE_CHECK(object.HasMember("max-zoom"), "Because the 'NCamera' object has member 'perspective' set to false, it must have member 'max-zoom'")
    auto &maxZoomVal = object["max-zoom"];
    PARSE_CHECK(maxZoomVal.IsNumber(), "Member 'max-zoom' of 'NCamera' must be of type float")
    maxZoom = maxZoomVal.GetFloat();

    PARSE_CHECK(object.HasMember("zoom"), "Because the 'NCamera' object has member 'perspective' set to false, it must have member 'zoom'")
    auto &zoomVal = object["zoom"];
    PARSE_CHECK(zoomVal.IsNumber(), "Member 'zoom' of 'NCamera' must be of type float")
    currentZoom = zoomVal.GetFloat();
  }

  if (object.HasMember("active")) {
    auto &activeVal = object["active"];
    PARSE_CHECK(activeVal.IsBool(), "Member 'active' of 'NCamera' must be of type bool")
    if (activeVal.GetBool())
      active = this;
  }

  PARSE_CHECK(object.HasMember("NNode"), "'NCamera' object must have member 'NNode'")
  return NNode::ReadFromJSON(object["NNode"], manager);
}
