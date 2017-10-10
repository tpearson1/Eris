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
#include <game/controllablecamera.h>
#include <scene/camera.h>
#include <scene/mesh.h>
#include <scene/meshconfig.h>
#include <scene/scene.h>
#include <scene/tagmanager.h>

class NSpectatorCamera : public NControllableCamera {
public:
  virtual void OnMouseMove(Vec2 pos) override;
};

void NSpectatorCamera::OnMouseMove(Vec2 pos) {
  if (!Window::GetActive()->GetInput().IsKeyDown(KeyCode::F)) {
    auto delta = GetMouseMovementChange(pos);

    auto cam = NCamera::active;
    cam->transform.RotateGlobal(0.0f, -delta.x, 0.0f);
    cam->transform.Rotate(delta.y, 0.0f, 0.0f);
    // Window::GetActive()->GetInput().SetMouseMode(MouseMode::DISABLED);
  } else
    Window::GetActive()->GetInput().SetMouseMode(MouseMode::NORMAL);
}

class MyGame : public Game {
  Scene scene;
  NMesh *sphere;
  Vec3 rot;
  bool go = false;
  std::list<NMesh *> rend;
  std::list<KeyState::RegistrationType> keyRegistrations;

public:
  MyGame();
  void Tick(float delta) override {
    glClearColor(0.3f, 0.6f, 0.6f, 1.0f);
    scene.Render();

    if (go) {
      auto spawn = reinterpret_cast<NMesh *>(sphere->Child(0));

      auto n = new NMesh(*spawn);
      n->Parent(nullptr); // So it keeps its location
      n->transform.Location(spawn->GlobalLocation());
      n->transform.Scale(Vec3::one * 0.1f);

      rend.push_back(n);
      if (rend.size() > 99) {
        delete rend.front();
        rend.erase(rend.begin());
      }

      sphere->transform.Rotate(rot * delta);
    }

    float mul = delta * 45.0f;
    if (GetInput().IsKeyDown(KeyCode::SHIFT)) mul *= 4.0f;

    Vec3 movement;
    if (GetInput().IsKeyDown(KeyCode::W)) movement += Vec3(0.0f, 0.0f, -mul);
    if (GetInput().IsKeyDown(KeyCode::S)) movement += Vec3(0.0f, 0.0f, mul);
    if (GetInput().IsKeyDown(KeyCode::A)) movement += Vec3(-mul, 0.0f, 0.0f);
    if (GetInput().IsKeyDown(KeyCode::D)) movement += Vec3(mul, 0.0f, 0.0f);
    if (GetInput().IsKeyDown(KeyCode::Q)) movement += Vec3(0.0f, -mul, 0.0f);
    if (GetInput().IsKeyDown(KeyCode::E)) movement += Vec3(0.0f, mul, 0.0f);
    NCamera::active->transform.Translate(NCamera::active->GlobalRotation() *
                                         movement);
  }
};

MyGame::MyGame() {
  keyRegistrations.emplace_back(
      GetInput().RegisterKeyCallback(KeyCode::ESCAPE, [](InputEvent action) {
        if (action == InputEvent::PRESS) Window::GetActive()->Close();
      }));

  keyRegistrations.emplace_back(
      GetInput().RegisterKeyCallback(KeyCode::U, [this](InputEvent action) {
        if (action == InputEvent::PRESS) go = !go;
      }));

  keyRegistrations.emplace_back(
      GetInput().RegisterKeyCallback(KeyCode::I, [this](InputEvent action) {
        if (action != InputEvent::PRESS) return;

        if (GetInput().IsKeyDown(KeyCode::SHIFT))
          rot.x -= 30.0f;
        else
          rot.x += 30.0f;
      }));

  keyRegistrations.emplace_back(
      GetInput().RegisterKeyCallback(KeyCode::O, [this](InputEvent action) {
        if (action != InputEvent::PRESS) return;

        if (GetInput().IsKeyDown(KeyCode::SHIFT))
          rot.y -= 30.0f;
        else
          rot.y += 30.0f;
      }));

  keyRegistrations.emplace_back(
      GetInput().RegisterKeyCallback(KeyCode::P, [this](InputEvent action) {
        if (action != InputEvent::PRESS) return;

        if (GetInput().IsKeyDown(KeyCode::SHIFT))
          rot.z -= 30.0f;
        else
          rot.z += 30.0f;
      }));

  auto tm = std::make_unique<TagManager>();
  TagManager::active = tm.get();
  auto resources = std::make_unique<Resources>();
  Resources::active = resources.get();

  auto typeManager = std::make_shared<JSON::TypeManager>();
  JSON::ReadData readData{typeManager};
  RegisterSceneTypeAssociations(*typeManager);

  (*typeManager)["NSpectatorCamera"] =
      DefaultNodeTypeRegistration<NSpectatorCamera, NCamera>;

  {
    using namespace MeshRenderConfigs;
    configurationGenerators["single-texture"] =
        MakeGenerator<AddTextures<Standard<Single>>>();
  }

  scene.SetActive();

  JSON::Document shaders, textures, sceneDoc;

  JSON::GetDataFromFile(shaders, "mods/orbit/res/shaders.json");
  JSON::Read(Resources::active->shaders, shaders, readData);

  JSON::GetDataFromFile(textures, "mods/orbit/res/textures.json");
  JSON::Read(Resources::active->textures, textures, readData);

  JSON::GetDataFromFile(sceneDoc, "mods/orbit/res/scene.json");
  JSON::Read(scene, sceneDoc, readData);

  sphere = TagManager::active->Get<NMesh>("sphere");
}

extern "C" bool Orbit_Run() {
  MyGame().Start();
  return true;
}
