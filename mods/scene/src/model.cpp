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
#include <scene/meshrenderer.h>
#include <base/resourcemanager.h>

NMeshRenderer *ProcessMesh(aiMesh *mesh, const aiScene * /* scene */, const RenderRequirements &rr, std::function<void()> preRenderFunc) {
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
  std::shared_ptr<Mesh> meshPtr;
  if (hasUVs) {
    meshPtr = std::make_shared<Mesh>(
      verts, indices, std::vector<VertexAttribute<>>{
        {1, 2, uvs},
        {2, 3, normals}
      }
    );
  }
  else {
    meshPtr = std::make_shared<Mesh>(
      verts, indices, std::vector<VertexAttribute<>>{
        {2, 3, normals}
      }
    );
  }
  meshPtr->preRenderFunc = preRenderFunc;
 
  auto *mr = new NMeshRenderer(rr);
  mr->renderer.Set(meshPtr);
  return mr;
}

static void ProcessNode(aiNode *node, const aiScene *scene, NNode *parent, const RenderRequirements &rr, std::function<void()> preRenderFunc) {
  for (auto i = 0u; i < node->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[node->mMeshes[i]];
    ProcessMesh(mesh, scene, rr, preRenderFunc)->transform.Parent(parent);
  }

  for (auto i = 0u; i < node->mNumChildren; i++)
    ProcessNode(node->mChildren[i], scene, parent, rr, preRenderFunc);
}

NNode *LoadModel(const std::string &path, const RenderRequirements &rr, std::function<void()> preRenderFunc) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "> ERROR: Failed to load Model: " << importer.GetErrorString() << '\n';
    return nullptr;
  }

  auto *root = new NNode;
  ProcessNode(scene->mRootNode, scene, root, rr, preRenderFunc);

  return root;
}

NNode *ModelRegistration(const rapidjson::Value &val, JSONTypeManager &manager) {
  CHECK_RETURN(val.IsObject(), "'Model' type must be an object", nullptr)
  const auto &object = val.GetObject();

  CHECK_RETURN(object.HasMember("path"), "'Model' object must have member 'path'", nullptr)
  const auto &path = object["path"];
  CHECK_RETURN(path.IsString(), "Member 'path' in 'Model' must be of type string", nullptr)

  CHECK_RETURN(object.HasMember("texture"), "'Model' object must have member 'texture'", nullptr)
  const auto &texture = object["texture"];
  CHECK_RETURN(texture.IsString(), "Member 'texture' in 'Model' must be of type string", nullptr)

  CHECK_RETURN(object.HasMember("NNode"), "'Model' object must have member 'NNode'", nullptr)
  const auto &node = object["NNode"];

  CHECK_RETURN(object.HasMember("shader"), "'Model' object must have member 'shader'", nullptr)
  const auto &shader = object["shader"];
  CHECK_RETURN(shader.IsObject(), "Member 'shader' in 'Model' must be of type object", nullptr)
  const auto &shaderObj = shader.GetObject();

  CHECK_RETURN(shaderObj.HasMember("vertex"), "Member 'shader' of 'Model' object must have member 'vertex'", nullptr)
  const auto &vertexShader = shaderObj["vertex"];
  CHECK_RETURN(vertexShader.IsString(), "Member 'vertex' in member 'shader' of 'Model' must be of type string", nullptr)

  CHECK_RETURN(shaderObj.HasMember("fragment"), "Member 'shader' of 'Model' object must have member 'fragment'", nullptr)
  const auto &fragmentShader = shaderObj["fragment"];
  CHECK_RETURN(fragmentShader.IsString(), "Member 'fragment' in member 'shader' of 'Model' must be of type string", nullptr)

  std::function<void()> preRenderFunction;
  if (shaderObj.HasMember("prerender-func")) {
    const auto &preRenderFunc = shaderObj["prerender-func"];
    CHECK_RETURN(preRenderFunc.IsString(), "Member 'prerender-func' in member 'shader' of 'Model' must be of type string", nullptr)
    preRenderFunction = Resources::active->preRenderMeshFuncs.Get(preRenderFunc.GetString()); 
  }

  Shader::Definitions definitions;
  if (shaderObj.HasMember("definitions")) {
    const auto &defsObj = shaderObj["definitions"];
    CHECK_RETURN(defsObj.IsArray(), "Member 'definition' in member 'shader' of 'Model' must be of type array", nullptr)
    const auto &defs = defsObj.GetArray();
    for (auto it = defs.Begin(); it != defs.End(); it++) {
      CHECK_RETURN(it->IsString(), "Shader definitions must be strings", nullptr)
      auto name = it->GetString();
      CHECK_RETURN(++it != defs.End() && it->IsString(), "Shader definitions must occur in string pairs", nullptr)
      
      definitions.push_back({name, it->GetString()}); 
    }
  }

  RenderRequirements rr;

  auto texturePath = texture.GetString();
  auto textureRef = Resources::active->textures.Get(texturePath);
  if (textureRef)
    rr.texture = textureRef;
  else {
    Ref<Texture> t;
    t.Reset();
    t->Load(texturePath);
    Resources::active->textures.Add(texturePath, t);
    rr.texture = t;
  }

  auto vertexPath = vertexShader.GetString();
  auto fragmentPath = fragmentShader.GetString();
  bool success = false;
  ShaderResource shaderRes;
  for (auto &shader : Resources::active->shaders) {
    if (shader.vertexPath == vertexPath
        && shader.fragmentPath == fragmentPath
        && shader.definitions == definitions) {
      success = true; 
      shaderRes = shader;
    }
  }
  
  if (success)
    rr.shader = shaderRes.shader;
  else {
    ShaderResource s;
    s.vertexPath = vertexPath;
    s.fragmentPath = fragmentPath;
    s.definitions = definitions;
    s.shader = Ref<Shader>::Create(vertexPath, fragmentPath, definitions);
    Resources::active->shaders.push_back(s);
    rr.shader = s.shader;
  }

  // TODO: Assets for shader and texture support?
  auto model = LoadModel(path.GetString(), rr, preRenderFunction); 
  model->LoadFromJSON(node, manager);
  return model;
}
