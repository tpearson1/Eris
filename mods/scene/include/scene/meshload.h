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

#ifndef _SCENE__MESH_LOAD_H
#define _SCENE__MESH_LOAD_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <scene/scene.h>
#include <vector>

class NMesh;
class MeshRenderer;
class InstancedMesh;

namespace MeshRenderConfigs {
template <typename ConfigBase>
struct Standard;

struct Single;
}

struct MeshData {
  std::vector<GLfloat> verts, uvs, normals;
  std::vector<GLuint> indices;
  bool hasUVs = true, successful = true;

  using SingleConfigType =
      MeshRenderConfigs::Standard<MeshRenderConfigs::Single>;
  using InstancedConfigType = MeshRenderConfigs::Standard<MeshRenderer>;

  MeshData() {}
  MeshData(const aiMesh *mesh) { Load(mesh); }
  MeshData(const std::string &path) { Load(path); }

  NMesh *GenerateNMesh(const std::shared_ptr<const Shader> &shader,
                       const std::shared_ptr<SingleConfigType> &mr,
                       unsigned instanceCount);

  std::unique_ptr<InstancedMesh>
  GenerateInstancedMesh(const std::shared_ptr<const Shader> &shader,
                        const std::shared_ptr<InstancedConfigType> &mr,
                        unsigned instanceCount);

  void Load(const aiMesh *mesh);
  void Load(const std::string &path);
};

const aiScene *LoadScene(const std::string &path, Assimp::Importer &importer);

/*
 * Function that can be registered for the ability to load single meshes as
 * shown below:
 * [
 *   "NMesh",
 *   {
 *     "path": "mods/mod/res/mesh.blend",
 *     "shader": "unlit",
 *     "config": {
 *       "type": "none",
 *       "data": {}
 *     },
 *     "NNode": { ... }
 *   }
 * ]
 */
NMesh *MeshTypeRegistration(const JSON::Value &value,
                            const JSON::ReadData &data);

#endif // _SCENE__MESH_LOAD_H
