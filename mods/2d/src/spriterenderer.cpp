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

#include <spriterenderer.h>
#include <iostream>
#include <base/shader.h>
#include <2d/sprite.h>

std::vector<GLfloat> SpriteRenderer::quadVertices{
  1.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f
};

std::vector<GLuint> SpriteRenderer::quadIndices{
  0, 1, 2,
  1, 2, 3
};

void SpriteRenderer::Draw() {
  if (!visible)
    return;

  GLint shaderMVP = Shader::Current().GetUniform("MVP");
  Shader::SetUniformMatrix4(shaderMVP, 1, GL_FALSE, transform.Matrix());

  quad.Draw();
}

void SpriteRenderer::Data(const Sprite *spr) {
  sprite = spr;
  std::vector<GLfloat> uvs = sprite->TexCoords();
  //requirements.texture = &sprite->sheet->Tex();
  quad.Setup(quadVertices, quadIndices, {VertexAttribute<>(1, 2, uvs)});
}
