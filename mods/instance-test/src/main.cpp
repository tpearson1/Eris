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

#include <base/resources.h>
#include <core/file.h>
#include <game/game.h>
#include <input/input.h>
#include <scene/camera.h>
#include <scene/instancedmeshconfig.h>
#include <scene/mesh.h>
#include <scene/meshconfig.h>
#include <scene/meshload.h>
#include <scene/renderer.h>
#include <scene/scene.h>
#include <scene/tagmanager.h>

struct NIM : public Renderable {
  std::shared_ptr<Mesh> spheres;
};

class MyGame : public Game {
  Scene scene;
  NIM nim;

public:
  MyGame();
  void Tick(float delta) override {
    glClearColor(0.3f, 0.6f, 0.6f, 1.0f);
    scene.Render();

    float mul = delta * 45.0f;
    if (Input::IsKeyDown(KeyCode::SHIFT)) mul *= 4.0f;

    Vec3 movement;
    if (Input::IsKeyDown(KeyCode::W)) movement += Vec3(0.0f, 0.0f, -mul);
    if (Input::IsKeyDown(KeyCode::S)) movement += Vec3(0.0f, 0.0f, mul);
    if (Input::IsKeyDown(KeyCode::A)) movement += Vec3(-mul, 0.0f, 0.0f);
    if (Input::IsKeyDown(KeyCode::D)) movement += Vec3(mul, 0.0f, 0.0f);
    if (Input::IsKeyDown(KeyCode::Q)) movement += Vec3(0.0f, -mul, 0.0f);
    if (Input::IsKeyDown(KeyCode::E)) movement += Vec3(0.0f, mul, 0.0f);
    NCamera::active->transform.Translate(
        NCamera::active->transform.GlobalRotation() * movement);
  }
};

static double startDragX = 0.0, startDragY = 0.0;

void OnMouseMove(double xPos, double yPos) {
  if (!Input::IsKeyDown(KeyCode::F)) {
    NCamera::active->transform.RotateGlobal(
        0.0f, static_cast<float>(startDragX - xPos), 0.0f);
    NCamera::active->transform.Rotate(static_cast<float>(yPos - startDragY),
                                      0.0f, 0.0f);
    Input::SetMouseMode(MouseMode::DISABLED);
  } else
    Input::SetMouseMode(MouseMode::NORMAL);

  startDragX = xPos;
  startDragY = yPos;
}

MyGame::MyGame() {
  Input::RegisterMouseMoveCallback(OnMouseMove);
  Input::RegisterKeyCallback(KeyCode::ESCAPE, [](InputEvent action) {
    if (action == InputEvent::PRESS) Window::inst->Close();
  });

  Input::GetMousePosition(startDragX, startDragY);

  auto resources = std::make_unique<Resources>();
  Resources::active = resources.get();

  auto typeManager = std::make_shared<JSON::TypeManager>();
  JSON::ReadData readData{typeManager};

  scene.SetActive();

  NCamera::active = new NCamera;
  NCamera::active->transform.Parent(&scene.root);

  JSON::Document shaders, textures;

  JSON::GetDataFromFile(shaders, "mods/instance-test/res/shaders.json");
  JSON::Read(Resources::active->shaders, shaders, readData);

  JSON::GetDataFromFile(textures, "mods/instance-test/res/textures.json");
  JSON::Read(Resources::active->textures, textures, readData);

  using namespace MeshRenderConfigs;
  auto config =
      std::make_shared<Instanced::AddTransformation<AddTextures<Standard>>>();
  config->textures.push_back(
      {"material.diffuse", Resources::active->textures.Get("sphere")});

  constexpr const auto width = 5, length = 5, height = 7;
  constexpr const auto instanceCount = width * length * height;
  constexpr const auto spacing = 4.5f;

  std::vector<Mat4> mats;
  mats.reserve(instanceCount);
  for (int i = 0; i < width; i++)
    for (int j = 0; j < length; j++)
      for (int k = 0; k < height; k++)
        mats.push_back(Mat4::Translate(i * spacing, j * spacing, k * spacing));
  config->SetupAddTransformation(mats);

  nim.spheres = MeshData{"mods/instance-test/res/sphere.blend"}.GenerateMesh(
      config, instanceCount);

  auto shader = Resources::active->shaders.Get("instanced");
  Renderer::active->Register([&] { nim.spheres->Draw(Mat4::identity); }, &nim,
                             shader);
}

extern "C" bool InstanceTest_Run() {
  MyGame().Start();
  return true;
}
