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

#ifndef _SCENE__MESH_RENDERER_H
#define _SCENE__MESH_RENDERER_H

#include <base/mesh.h>
#include <memory>

class Shader;

class MeshRenderer {
  std::unique_ptr<Mesh> mesh;

protected:
  std::vector<VertexAttribute> vertexAttributes;

  virtual void GetUniforms(const Shader *) {}

public:
  const Mesh *GetMesh() { return mesh.get(); }

  void SetMeshAndSetupAttributes(std::unique_ptr<Mesh> _mesh);

  void SetShader(const Shader *s) {
    assert(s);
    GetUniforms(s);
  }

  virtual void PreRender() const {}

  void Draw() { mesh->Draw(); }

  virtual ~MeshRenderer() {}
};

#endif // _SCENE__MESH_RENDERER_H
