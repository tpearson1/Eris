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

#include <game/game.h>
#include <core/file.h>
#include <base/resources.h>
#include <input/input.h>
#include <scene/scene.h>
#include <scene/camera.h>
#include <scene/tagmanager.h>
#include <scene/meshrenderer.h>

class MyGame : public Game {
  Scene scene;
  NMeshRenderer *sphere;
  Vec3 rot;
  bool go = false;
  std::list<NMeshRenderer *> rend;

public:
  MyGame();
  void Tick(float delta) override {
    glClearColor(0.3f, 0.6f, 0.6f, 1.0f);
    scene.Render();

    if (go) {
      auto spawn = reinterpret_cast<NMeshRenderer *>(sphere->transform.Child(0));

      auto n = new NMeshRenderer(*spawn);
      n->transform.Location(spawn->transform.GlobalLocation());
      n->transform.Scale(Vec3::one * 0.1f);

      rend.push_back(n);
      if (rend.size() > 99) {
        delete rend.front();
          rend.erase(rend.begin());
      }

      sphere->transform.Rotate(rot * delta);
    }

    float mul = delta * 45.0f;
    if (Input::IsKeyDown(KeyCode::SHIFT))
      mul *= 4.0f;

    Vec3 movement;
    if (Input::IsKeyDown(KeyCode::W))
      movement += Vec3(0.0f, 0.0f, -mul);
    if (Input::IsKeyDown(KeyCode::S))
      movement += Vec3(0.0f, 0.0f, mul);
    if (Input::IsKeyDown(KeyCode::A))
      movement += Vec3(-mul, 0.0f, 0.0f);
    if (Input::IsKeyDown(KeyCode::D))
      movement += Vec3(mul, 0.0f, 0.0f);
    if (Input::IsKeyDown(KeyCode::Q))
      movement += Vec3(0.0f, -mul, 0.0f);
    if (Input::IsKeyDown(KeyCode::E))
      movement += Vec3(0.0f, mul, 0.0f);
    NCamera::active->transform.Translate(NCamera::active->transform.GlobalRotation() * movement);
  }
};

static double startDragX = 0.0, startDragY = 0.0;

void OnMouseMove(double xPos, double yPos) {
  if (!Input::IsKeyDown(KeyCode::F)) {
    NCamera::active->transform.RotateGlobal(0.0f, static_cast<float>(startDragX - xPos), 0.0f);
    NCamera::active->transform.Rotate(static_cast<float>(yPos - startDragY), 0.0f, 0.0f);
    Input::SetMouseMode(MouseMode::DISABLED);
  }
  else
    Input::SetMouseMode(MouseMode::NORMAL);

  startDragX = xPos;
  startDragY = yPos;
}

MyGame::MyGame() {
  Input::RegisterMouseMoveCallback(OnMouseMove);
  Input::RegisterKeyCallback(KeyCode::ESCAPE, [](InputEvent action) {
    if (action == InputEvent::PRESS)
      Window::inst->Close();
  });

  Input::RegisterKeyCallback(KeyCode::U, [this](InputEvent action) {
    if (action == InputEvent::PRESS)
      go = !go;
  });

  Input::RegisterKeyCallback(KeyCode::I, [this](InputEvent action) {
    if (action != InputEvent::PRESS)
      return;

    if (Input::IsKeyDown(KeyCode::SHIFT))
      rot.x -= 30.0f;
    else
      rot.x += 30.0f;
  });

  Input::RegisterKeyCallback(KeyCode::O, [this](InputEvent action) {
    if (action != InputEvent::PRESS)
      return;

    if (Input::IsKeyDown(KeyCode::SHIFT))
      rot.y -= 30.0f;
    else
      rot.y += 30.0f;
  });

  Input::RegisterKeyCallback(KeyCode::P, [this](InputEvent action) {
    if (action != InputEvent::PRESS)
      return;

    if (Input::IsKeyDown(KeyCode::SHIFT))
      rot.z -= 30.0f;
    else
      rot.z += 30.0f;
  });

  Input::GetMousePosition(startDragX, startDragY);

  auto tm = std::make_unique<TagManager>();
  TagManager::active = tm.get();
  auto resources = std::make_unique<Resources>();
  Resources::active = resources.get();

  auto typeManager = std::make_shared<JSON::TypeManager>();
  JSON::ReadData readData{typeManager};
  RegisterSceneTypeAssociations(*typeManager);

  scene.SetActive();

  JSON::Document shaders, textures, sceneDoc;

  JSON::GetDataFromFile(shaders, "mods/orbit/res/shaders.json");
  JSON::Read(Resources::active->shaders, shaders, readData);

  JSON::GetDataFromFile(textures, "mods/orbit/res/textures.json");
  JSON::Read(Resources::active->textures, textures, readData);

  JSON::GetDataFromFile(sceneDoc, "mods/orbit/res/scene.json");
  JSON::Read(scene, sceneDoc, readData);

  sphere = TagManager::active->Get<NMeshRenderer>("sphere");
}

extern "C" bool Orbit_Run() {
  MyGame().Start();
  return true;
}
