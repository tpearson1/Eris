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

NMeshRenderer *ProcessMesh(aiMesh *mesh, const aiScene * /* scene */, const RenderRequirements &rr, NMeshRenderer::PreRenderFunctionType preRenderFunc) {
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
  Ref<Mesh> meshPtr;
  if (hasUVs) {
    meshPtr = Ref<Mesh>::Create(
      verts, indices, std::vector<VertexAttribute<>>{
        {1, 2, uvs},
        {2, 3, normals}
      }
    );
  }
  else {
    meshPtr = Ref<Mesh>::Create(
      verts, indices, std::vector<VertexAttribute<>>{
        {2, 3, normals}
      }
    );
  }
 
  auto *mr = new NMeshRenderer(rr);
  mr->preRenderFunction = preRenderFunc;
  mr->renderer.Set(meshPtr);
  return mr;
}

static void ProcessNode(aiNode *node, const aiScene *scene, NNode *parent, const RenderRequirements &rr, NMeshRenderer::PreRenderFunctionType preRenderFunc) {
  for (auto i = 0u; i < node->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[node->mMeshes[i]];
    ProcessMesh(mesh, scene, rr, preRenderFunc)->transform.Parent(parent);
  }

  for (auto i = 0u; i < node->mNumChildren; i++)
    ProcessNode(node->mChildren[i], scene, parent, rr, preRenderFunc);
}

static const aiScene *LoadScene(const std::string &path, Assimp::Importer &importer, unsigned int flags) {
  const auto *scene = importer.ReadFile(path, flags); 
  CHECK_RETURN(scene && scene->mFlags != AI_SCENE_FLAGS_INCOMPLETE && scene->mRootNode, "Failed to load Scene" << importer.GetErrorString(), nullptr)
  return scene;
}

NNode *LoadModel(const std::string &path, const RenderRequirements &rr, NMeshRenderer::PreRenderFunctionType preRenderFunc) {
  Assimp::Importer importer;
  const auto *scene = LoadScene(path, importer, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
  auto *root = new NNode;
  ProcessNode(scene->mRootNode, scene, root, rr, preRenderFunc);

  return root;
}

NMeshRenderer *LoadMesh(const std::string &path, const RenderRequirements &rr) {
  Assimp::Importer importer;
  const auto *scene = LoadScene(path, importer, aiProcess_Triangulate | aiProcess_CalcTangentSpace);// | aiProcess_OptimizeMeshes);
  
  const auto *meshes = scene->mRootNode->mChildren[0];

  CHECK_RETURN(meshes->mNumMeshes == 1, "Failed to load mesh - there is more than one mesh in the file", nullptr)

  return ProcessMesh(scene->mMeshes[meshes->mMeshes[0]], scene, rr, nullptr);
}

NNode *ModelRegistration(const rapidjson::Value &val, JSON::TypeManager &manager) {
  CHECK_RETURN(val.IsObject(), "'Model' type must be an object", nullptr)
  const auto &object = val.GetObject();

  CHECK_RETURN(object.HasMember("path"), "'Model' object must have member 'path'", nullptr)
  const auto &path = object["path"];
  CHECK_RETURN(path.IsString(), "Member 'path' in 'Model' must be of type string", nullptr)

  NMeshRenderer::PreRenderFunctionType preRenderFunction;
  if (object.HasMember("prerender-func")) {
    const auto &preRenderFunc = object["prerender-func"];
    CHECK_RETURN(preRenderFunc.IsString(), "Member 'prerender-func' in 'Model' must be of type string", nullptr)
    preRenderFunction = NMeshRenderer::preRenderFunctions.Get(preRenderFunc.GetString()); 
  }

  CHECK_RETURN(object.HasMember("NNode"), "'Model' object must have member 'NNode'", nullptr)
  const auto &node = object["NNode"];
  
  CHECK_RETURN(object.HasMember("requirements"), "'Model' object must have member 'requirements'", nullptr)
  const auto &requirements = object["requirements"];
  
  RenderRequirements rr;
  rr.ReadFromJSON(requirements, manager);

  auto model = LoadModel(path.GetString(), rr, preRenderFunction); 
  model->ReadFromJSON(node, manager);
  return model;
}

NMeshRenderer *MeshRegistration(const rapidjson::Value &val, JSON::TypeManager &manager) {
  CHECK_RETURN(val.IsObject(), "'Mesh' type must be an object", nullptr)
  const auto &object = val.GetObject();

  CHECK_RETURN(object.HasMember("path"), "'Mesh' object must have member 'path'", nullptr)
  const auto &path = object["path"];
  CHECK_RETURN(path.IsString(), "Member 'path' in 'Mesh' must be of type string", nullptr)

  NMeshRenderer::PreRenderFunctionType preRenderFunction;
  if (object.HasMember("prerender-func")) {
    const auto &preRenderFunc = object["prerender-func"];
    CHECK_RETURN(preRenderFunc.IsString(), "Member 'prerender-func' in 'Mesh' must be of type string", nullptr)
    preRenderFunction = NMeshRenderer::preRenderFunctions.Get(preRenderFunc.GetString()); 
  }

  CHECK_RETURN(object.HasMember("NNode"), "'Mesh' object must have member 'NNode'", nullptr)
  const auto &node = object["NNode"];
  
  CHECK_RETURN(object.HasMember("requirements"), "'Mesh' object must have member 'requirements'", nullptr)
  const auto &requirements = object["requirements"];
  
  RenderRequirements rr;
  rr.ReadFromJSON(requirements, manager);

  auto *model = LoadMesh(path.GetString(), rr); 
  model->preRenderFunction = preRenderFunction;
  model->ReadFromJSON(node, manager);
  return model;
}
