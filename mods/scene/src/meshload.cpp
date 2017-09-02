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
---------------------------------------------------------------------------
*/

#include <meshload.h>
#include <assimp/postprocess.h>
#include <meshconfig.h>
#include <base/resources.h>
#include <test/macros.h>

std::shared_ptr<Mesh>
MeshData::GenerateMesh(const std::shared_ptr<MeshRenderConfigs::Standard> &config) {
  if (!successful) return nullptr;
  config->SetupStandard(uvs, normals);
  auto mesh = std::make_shared<Mesh>(verts, indices, config);
  return mesh;
}

void MeshData::Load(const aiMesh *mesh) {
  hasUVs = true;
  for (auto i = 0u; i < mesh->mNumVertices; i++) {
    auto vert = mesh->mVertices[i];
    verts.push_back(vert.x);
    verts.push_back(vert.y);
    verts.push_back(vert.z);

    auto normal = mesh->mNormals[i];
    normals.push_back(normal.x);
    normals.push_back(normal.y);
    normals.push_back(normal.z);

    // Does the mesh have texture coordinates?
    // REVIEW: Consider multiple sets of UV coords?
    if (mesh->mTextureCoords[0]) {
      auto coords = mesh->mTextureCoords[0][i];
      uvs.push_back(coords.x);
      uvs.push_back(coords.y);
    }
    else
      hasUVs = false;
  }

  for (auto i = 0u; i < mesh->mNumFaces; i++) {
    auto face = mesh->mFaces[i];
    for (auto j = 0u; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // TODO:
  // if (mesh->mMaterialIndex >= 0) {
  //   auto *material = scene->mMaterials[mesh->mMaterialIndex];
  // }
}

static constexpr const auto sceneProcessFlags =
    aiProcess_Triangulate | aiProcess_CalcTangentSpace;

const aiScene *LoadScene(const std::string &path, Assimp::Importer &importer) {
  const auto *scene = importer.ReadFile(path, sceneProcessFlags);
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "Failed to load Scene" << importer.GetErrorString() << '\n';
    return nullptr;
  }
  return scene;
}

void MeshData::Load(const std::string &path) {
  Assimp::Importer importer;
  const auto *scene = LoadScene(path, importer);
  const auto *meshes = scene->mRootNode->mChildren[0];

  if (meshes->mNumMeshes != 1) {
    std::cerr << "Failed to load mesh - there is more than one mesh in the file\n";
    successful = false;
    hasUVs = false;
    return;
  }

  Load(scene->mMeshes[meshes->mMeshes[0]]);
}

static auto ReadMeshConfig(const JSON::Value &value,
                           const JSON::ReadData &data,
                           const NMesh &mesh) {
  auto t = Trace::Pusher{data.trace, "ReadMeshConfig"};
  const auto &object = JSON::GetObject(value, data);

  const auto type = JSON::GetMember<std::string>("type", object, data);
  const auto generatorIt = MeshRenderConfigs::configurationGenerators.find(type);
  if (generatorIt == std::end(MeshRenderConfigs::configurationGenerators))
    JSON::ParseError(data, "Configuration generator of type '"
      + type + "' not found in MeshRenderConfig::configurationGenerators");

  const auto dataIt = object.FindMember("data");
  JSON::ParseFailIf(dataIt == object.MemberEnd(), data,
                    "Member 'data' must be present in 'config'");
  return (generatorIt->second)(dataIt->value, data, mesh);
}

NMesh *MeshTypeRegistration(const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "MeshTypeRegistration"};

  const auto &object = JSON::GetObject(value, data);
  const auto path = JSON::GetMember<std::string>("path", object, data);

  auto shaderStr = JSON::GetMember<std::string>("shader", object, data);
  auto shader = Resources::active->shaders.Get(shaderStr);
  auto *nm = new NMesh(shader);

  const auto configObjIt = object.FindMember("config");
  JSON::ParseFailIf(configObjIt == object.MemberEnd(), data, "Member 'config' must be present");
  auto config = ReadMeshConfig(configObjIt->value, data, *nm);

  auto m = MeshData(path).GenerateMesh(std::move(config));
  nm->Set(m);

  JSON::GetMember<NNode>(*nm, "NNode", object, data);
  return nm;
}

