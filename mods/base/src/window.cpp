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

#include <cstdlib>
#include <iostream>

#include <input.h>

Window *Window::active = nullptr;

void Window::SetViewportSize(IVec2 size) { glViewport(0, 0, size.x, size.y); }

void Window::OnResize(GLFWwindow *glfwWindow, int width, int height) {
  auto window = GetWindowFromGLFWwindow(glfwWindow);
  window->size = {width, height};

  if (!window->IsActive()) {
    auto cachedActive = GetActive();
    window->MakeActive();
    SetViewportSize(window->size);
    cachedActive->MakeActive();
  } else
    SetViewportSize(window->size);

  for (auto &callback : window->resizeCallbacks) callback(window->size);
}

Window::Window(IVec2 _size) {
  assert(GLFW::IsSetup());
  size = _size;

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL::majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL::minorVersion);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // We don't want the old OpenGL
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create OpenGL context
  window = glfwCreateWindow(size.x, size.y, "Game", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to open window\n";
    glfwTerminate();
    std::exit(-1);
  }

  auto activeTmp = active;
  MakeActive();
  glfwSetFramebufferSizeCallback(window, OnResize);
  if (activeTmp && activeTmp != this)
    activeTmp->MakeActive();

  // Associate this Window with the GLFWwindow
  glfwSetWindowUserPointer(window, this);

  input = std::make_unique<Input>(window);
}

Window::~Window() { Close(); }

void Window::Close() { glfwSetWindowShouldClose(window, true); }
