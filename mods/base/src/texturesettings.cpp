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

#include <texturesettings.h>
#include <iostream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <core/file.h>

void JSONImpl<TextureSettings>::Write(const TextureSettings &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};

  JSON::WritePair("path", value.path, writer);

  const std::unordered_map<TextureType, std::string> textureTypes = {
    {TextureType::Tex1D, "tex-1d"},
    {TextureType::Tex2D, "tex-2d"},
    {TextureType::Tex3D, "tex-3d"},
    {TextureType::Rectangle, "rectangle"},
    {TextureType::Buffer, "buffer"},
    {TextureType::Cubemap, "cubemap"},
    {TextureType::Tex1DArray, "tex-1d-array"},
    {TextureType::Tex2DArray, "tex-2d-array"},
    {TextureType::CubemapArray, "cubemap-array"},
    {TextureType::Tex2DMultisample, "tex-2d-multisample"},
    {TextureType::Tex2DMultisampleArray, "tex-2d-multisample-array"}
  };

  JSON::WritePair("type", textureTypes.at(value.type), writer);

  const std::unordered_map<TextureShrinkType, std::string> shrinkTypes = {
    {TextureShrinkType::NearestMipmapNearest, "nearest-mipmap-nearest"},
    {TextureShrinkType::LinearMipmapNearest, "linear-mipmap-nearest"},
    {TextureShrinkType::NearestMipmapLinear, "nearest-mipmap-linear"},
    {TextureShrinkType::LinearMipmapLinear, "linear-mipmap-linear"},
    {TextureShrinkType::Nearest, "nearest"},
    {TextureShrinkType::Linear, "linear"}
  };

  JSON::WritePair("shrink-filter", shrinkTypes.at(value.shrinkFilter), writer);

  const std::unordered_map<TextureEnlargeType, std::string> enlargeTypes = {
    {TextureEnlargeType::Nearest, "nearest"},
    {TextureEnlargeType::Linear, "linear"}
  };

  JSON::WritePair("enlarge-filter", enlargeTypes.at(value.enlargeFilter), writer);
}

void JSONImpl<TextureSettings>::Read(TextureSettings &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "TextureSettings"};

  const auto &object = JSON::GetObject(value, data);

  JSON::GetMember(out.path, "path", object, data);

  const std::unordered_map<std::string, TextureType> textureTypes = {
    {"tex-1d", TextureType::Tex1D},
    {"tex-2d", TextureType::Tex2D},
    {"tex-3d", TextureType::Tex3D},
    {"rectangle", TextureType::Rectangle},
    {"buffer", TextureType::Buffer},
    {"cubemap", TextureType::Cubemap},
    {"tex-1d-array", TextureType::Tex1DArray},
    {"tex-2d-array", TextureType::Tex2DArray},
    {"cubemap-array", TextureType::CubemapArray},
    {"tex-2d-multisample", TextureType::Tex2DMultisample},
    {"tex-2d-multisample-array", TextureType::Tex2DMultisampleArray}
  };

  auto texType = JSON::GetMember<std::string>("type", object, data);
  JSON::GetAssociated(out.type, textureTypes, texType, data);

  const std::unordered_map<std::string, TextureShrinkType> shrinkTypes = {
    {"nearest-mipmap-nearest", TextureShrinkType::NearestMipmapNearest},
    {"linear-mipmap-nearest", TextureShrinkType::LinearMipmapNearest},
    {"nearest-mipmap-linear", TextureShrinkType::NearestMipmapLinear},
    {"linear-mipmap-linear", TextureShrinkType::LinearMipmapLinear},
    {"nearest", TextureShrinkType::Nearest},
    {"linear", TextureShrinkType::Linear}
  };

  auto shrink = JSON::GetMember<std::string>("shrink-filter", object, data);
  JSON::GetAssociated(out.shrinkFilter, shrinkTypes, shrink, data);

  const std::unordered_map<std::string, TextureEnlargeType> enlargeTypes = {
    {"nearest", TextureEnlargeType::Nearest},
    {"linear", TextureEnlargeType::Linear}
  };

  auto enlarge = JSON::GetMember<std::string>("enlarge-filter", object, data);
  JSON::GetAssociated(out.enlargeFilter, enlargeTypes, enlarge, data);
}
