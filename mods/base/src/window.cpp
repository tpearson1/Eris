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

#include <window.h>
#include <iostream>
#include <base/gl.h>

Window *Window::inst;

void Window::OnResize(GLFWwindow * /* window */, int width, int height) {
  inst->width = width;
  inst->height = height;
  glViewport(0, 0, width, height);

  for (auto &elem : inst->resizeCallbacks)
    elem(width, height);
}

Window::Window() {
  inst = this;
  if (!glfwInit()) {
    std::cerr << "> Failed to initialize GLFW\n";
    exit(-1);
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create OpenGL context
  window = glfwCreateWindow(width, height, "Game", NULL, NULL);
  if (!window) {
    std::cerr << "> Failed to open window\n";
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(inst->window, OnResize);
}

void Window::Close() {
  glfwSetWindowShouldClose(window, true);
}

Window::~Window() {
  // Close OpenGL window and terminate GLFW
  glfwTerminate();
}
