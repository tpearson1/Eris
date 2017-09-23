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
  Vec3 loc = -GlobalLocation();
  Quat rot = GlobalRotation();
  return Mat4::LookAt(loc, loc + rot * Vec3::front, rot * Vec3(0.0f, 1.0f, 0.0f));
}

void JSONImpl<NCamera>::Write(const NCamera &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
  JSON::Write(static_cast<const NNode &>(value), writer);

  JSON::WritePair("active", NCamera::active == &value, writer);

  JSON::WritePair("perspective", value.perspective, writer);
  if (value.perspective)
    JSON::WritePair("fov", value.fov, writer);
  else {
    JSON::WritePair("min-zoom", value.MinZoom(), writer);
    JSON::WritePair("max-zoom", value.MaxZoom(), writer);
    JSON::WritePair("zoom", value.Zoom(), writer);
  }

  JSON::WritePair("near", value.near, writer);
  JSON::WritePair("far", value.far, writer);
}

void JSONImpl<NCamera>::Read(NCamera &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "NCamera"};

  const auto &object = JSON::GetObject(value, data);
  JSON::GetMember(out.near, "near", object, data);
  JSON::GetMember(out.far, "far", object, data);
  JSON::GetMember(out.perspective, "perspective", object, data);

  if (out.perspective)
    JSON::GetMember(out.fov, "fov", object, data);
  else {
    JSON::GetMember(out.minZoom, "min-zoom", object, data);
    JSON::GetMember(out.maxZoom, "max-zoom", object, data);
    JSON::GetMember(out.currentZoom, "zoom", object, data);
  }

  auto isActive = JSON::TryGetMember<bool>("active", object, false, data);
  if (isActive)
    NCamera::active = &out;

  JSON::GetMember<NNode>(out, "NNode", object, data);
}
