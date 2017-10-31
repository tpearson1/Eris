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

#include <spectatorcamera.h>

void NSpectatorCamera::OnMouseMove(Vec2 pos) {
  auto &input = GetInput();

  if (input.IsKeyDown(KeyCode::F)) {
    if (disableMouseOnFreeze)
      input.SetMouseMode(MouseMode::Disabled);
    else
      input.SetMouseMode(MouseMode::Normal);
    return;
  }

  auto delta = GetMouseMovementChange(pos);
  auto change = delta * sensitivity;

  auto cam = NCamera::active;
  cam->transform.RotateGlobal(0.0f, -change.x, 0.0f);
  cam->transform.Rotate(change.y, 0.0f, 0.0f);

  if (mouseDisabled)
    input.SetMouseMode(MouseMode::Disabled);
  else
    input.SetMouseMode(MouseMode::Normal);
}
