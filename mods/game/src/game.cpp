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

#include <game.h>
#include <iostream>
#include <base/mesh.h>
#include <input/input.h>
#include <scene/node.h>

Game::Game() {
  window = Ref<Window>::Create();

  Input::Setup();

  glewExperimental = GL_TRUE; // Needed in core profile
  GLenum error;
  if ((error = glewInit()) != GLEW_OK) {
    std::cerr << "> Failed to initialize GLEW: '" << glewGetErrorString(error) << "'\n";
    exit(-1);
  }

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if closer to the camera than the former one
  glDepthFunc(GL_LESS);
  glCullFace(GL_BACK);

  MeshTemplates::SetupStatics();
}

Game::~Game() {
  Input::Cleanup();
}

void Game::Start() {
  Initialize();

  float delta = 0.0f;
  glfwSetTime(0.0);
  while (!window->ShouldClose()) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    delta = glfwGetTime();
    glfwSetTime(0.0);
    // Any nodes that are removed from the list during a 'Tick' call
    // will no longer cause problems
    auto tempList = tickNodes;
    for (auto &elem : tempList)
      elem->Tick(delta);
    Tick(delta);

    window->SwapBuffers();
  }
}

