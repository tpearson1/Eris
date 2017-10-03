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

#include <base/gl.h>
#include <cstdlib>
#include <iostream>

bool GLEW::Detail::setup = false;
bool GLFW::Detail::setup = false;

bool GLFW::Setup() {
  auto result = glfwInit();
  if (result) Detail::setup = true;
  return result;
}

GLenum GLEW::Setup() {
  glewExperimental = GL_TRUE; // Needed in core profile
  GLenum error = glewInit();
  if (error == GLEW_OK) Detail::setup = true;
  return error;
}

std::string GLEW::GetError(GLenum error) {
  static_assert(
      sizeof(GLubyte) == sizeof(char),
      "These types must be the same for this function to work");
  return reinterpret_cast<const char *>(glewGetErrorString(error));
}

void GLFW::Terminate() {
  glfwTerminate();
  Detail::setup = false;
}
