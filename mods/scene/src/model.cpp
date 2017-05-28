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

#include <model.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <base/resources.h>

static std::shared_ptr<RawMesh> ProcessMesh(aiMesh *mesh) {
  std::vector<GLfloat> verts, uvs, normals;
  std::vector<GLuint> indices;

  bool hasUVs = true;
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
  std::shared_ptr<RawMesh> meshPtr;
  if (hasUVs)
    meshPtr = std::make_shared<StandardMesh>(verts, indices, uvs, normals);
  else
    meshPtr = std::make_shared<NormalMesh>(verts, indices, normals);

  return meshPtr;
}

static NMeshRenderer *ProcessMeshRenderer(aiMesh *mesh, const RenderRequirements &rr, NMeshRenderer::PreRenderFunctionType preRenderFunc) {
  auto m = ProcessMesh(mesh);
  auto *mr = new NMeshRenderer(rr);
  mr->preRenderFunction = preRenderFunc;
  mr->renderer.Set(m);
  return mr;
}

static void ProcessNode(aiNode *node, const aiScene *scene, NNode *parent, const RenderRequirements &rr, NMeshRenderer::PreRenderFunctionType preRenderFunc) {
  for (auto i = 0u; i < node->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[node->mMeshes[i]];
    ProcessMeshRenderer(mesh, rr, preRenderFunc)->transform.Parent(parent);
  }

  for (auto i = 0u; i < node->mNumChildren; i++)
    ProcessNode(node->mChildren[i], scene, parent, rr, preRenderFunc);
}

static const aiScene *LoadScene(const std::string &path, Assimp::Importer &importer, unsigned int flags) {
  const auto *scene = importer.ReadFile(path, flags);
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "Failed to load Scene" << importer.GetErrorString() << '\n';
    return nullptr;
  }
  return scene;
}

NNode *LoadModel(const std::string &path, const RenderRequirements &rr, NMeshRenderer::PreRenderFunctionType preRenderFunc) {
  Assimp::Importer importer;
  const auto *scene = LoadScene(path, importer, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
  auto *root = new NNode;
  ProcessNode(scene->mRootNode, scene, root, rr, preRenderFunc);

  return root;
}

std::shared_ptr<RawMesh> LoadMesh(const std::string &path) {
  Assimp::Importer importer;
  const auto *scene = LoadScene(path, importer, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);

  const auto *meshes = scene->mRootNode->mChildren[0];

  if (meshes->mNumMeshes != 1) {
    std::cerr << "Failed to load mesh - there is more than one mesh in the file\n";
    return nullptr;
  }

  return ProcessMesh(scene->mMeshes[meshes->mMeshes[0]]);
}

NMeshRenderer *LoadMeshRenderer(const std::string &path, const RenderRequirements &rr) {
  Assimp::Importer importer;
  const auto *scene = LoadScene(path, importer, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);

  const auto *meshes = scene->mRootNode->mChildren[0];

  if (meshes->mNumMeshes != 1) {
    std::cerr << "Failed to load mesh - there is more than one mesh in the file\n";
    return nullptr;
  }

  return ProcessMeshRenderer(scene->mMeshes[meshes->mMeshes[0]], rr, nullptr);
}

NNode *ModelTypeRegistration(const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "ModelTypeRegistration"};

  const auto &object = JSON::GetObject(value, data);
  const auto path = JSON::GetMember<std::string>("path", object, data);

  NMeshRenderer::PreRenderFunctionType preRenderFunction;
  auto prerenderFuncIt = object.FindMember("prerender-func");
  if (prerenderFuncIt != object.MemberEnd()) {
    const auto funcName = JSON::Read<std::string>(prerenderFuncIt->value, data);
    preRenderFunction = NMeshRenderer::preRenderFunctions.at(funcName);
  }

  auto rr = JSON::GetMember<RenderRequirements>("requirements", object, data);
  auto *model = LoadModel(path, rr, preRenderFunction);

  JSON::GetMember<NNode>(*model, "NNode", object, data);
  return model;
}

NMeshRenderer *MeshRendererTypeRegistration(const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "MeshRendererTypeRegistration"};

  const auto &object = JSON::GetObject(value, data);
  const auto path = JSON::GetMember<std::string>("path", object, data);

  NMeshRenderer::PreRenderFunctionType preRenderFunction;
  auto prerenderFuncIt = object.FindMember("prerender-func");
  if (prerenderFuncIt != object.MemberEnd()) {
    const auto funcName = JSON::Read<std::string>(prerenderFuncIt->value, data);
    preRenderFunction = NMeshRenderer::preRenderFunctions.at(funcName);
  }

  auto rr = JSON::GetMember<RenderRequirements>("requirements", object, data);
  auto *mr = LoadMeshRenderer(path, rr);
  mr->preRenderFunction = preRenderFunction;

  JSON::GetMember<NNode>(*mr, "NNode", object, data);
  return mr;
}
