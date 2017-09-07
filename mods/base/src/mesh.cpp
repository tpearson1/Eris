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

#include <mesh.h>
#include <shader.h>

Mesh::Mesh(const std::vector<GLfloat> &verts,
           const std::vector<GLuint> &indexData,
           const std::shared_ptr<MeshRenderConfigs::None> &conf)
    : vertices(0, 3, verts), config(conf) {
  // Vertices, Element buffer and passing attribute to vertex shader
  vao.Generate();
  vao.Use();

  vertices.Setup();

  indices.Data(indexData);
  indices.Generate();

  config->Setup();

  VertexArray::ClearUse();
}

void Mesh::Draw(const Mat4 &mvp) const {
  auto shaderMVP = Shader::Current()->GetUniform("MVP");
  Shader::SetUniformMatrix4(shaderMVP, 1, GL_FALSE, mvp);

  config->PreRender();
  vao.Use();

  indices.Draw();

  VertexArray::ClearUse();
  config->PostRender();
}

