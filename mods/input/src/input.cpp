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

#include <input.h>
#include <iostream>
#include <cstring>
#include <base/image.h>
#include <core/statics.h>

std::unordered_map<MouseButton, std::list<Input::MouseButtonCallback>> Input::mouseButtonCallbacks;
std::list<Input::MouseMoveCallback> Input::mouseMoveCallbacks;
std::list<Input::MouseScrollCallback> Input::mouseScrollCallbacks;

std::unordered_map<KeyCode, KeyState> Input::keyStates;

struct GLFWcursor *Input::cursor = nullptr;

void Input::OnMouseButton(GLFWwindow * /* window */, int button, int action, int mods) {
  for (auto &elem : mouseButtonCallbacks[static_cast<MouseButton>(button)])
    elem(action, mods);
}

void Input::OnMouseMove(GLFWwindow * /* window */, double xPos, double yPos) {
  for (auto &elem : mouseMoveCallbacks)
    elem(xPos, yPos);
}

void Input::OnMouseScroll(GLFWwindow * /* window */, double xOffset, double yOffset) {
  for (auto &elem : mouseScrollCallbacks)
    elem(xOffset, yOffset);
}

void Input::Setup() {
  glfwSetCursorPosCallback(Window::inst->window, Input::OnMouseMove);
  glfwSetMouseButtonCallback(Window::inst->window, Input::OnMouseButton);
  glfwSetScrollCallback(Window::inst->window, Input::OnMouseScroll);
  
  glfwSetKeyCallback(Window::inst->window, Input::OnKey);
  // glfwSetCharCallback(Window::inst->window, Input::OnChar);
}

void Input::Cleanup() {
  if (cursor)
    glfwDestroyCursor(cursor);
}

void Input::SetCursor(const std::string &path) {
  RawImage r;
  r.Load(path);

  GLFWimage image;
  image.width = r.width;
  image.height = r.height;
  image.pixels = r.data.data();

  if (cursor)
    glfwDestroyCursor(cursor);

  cursor = glfwCreateCursor(&image, 0, 0);
  glfwSetCursor(Window::inst->window, cursor);
}

void Input::UpdateKeyState(int action, KeyCode key) {
  auto event = static_cast<InputEvent>(action);
  auto &keyState = keyStates[key];
  if (event == InputEvent::PRESS)
    keyState.down = true;
  else if (event == InputEvent::RELEASE)
    keyState.down = false;

  for (auto &callback : keyState.callbacks)
    callback(event);
}

void Input::UpdateLeftRightKeyState(int action, KeyCode pressed, KeyCode other, KeyCode both) {
  auto event = static_cast<InputEvent>(action);
  auto &pressedState = keyStates[pressed],
       &bothState = keyStates[both],
       &otherState = keyStates[other];
  if (event == InputEvent::PRESS) {
    pressedState.down = true;
    bothState.down = true;

    if (!otherState.down)
      for (auto &callback : bothState.callbacks)
        callback(event);
  }
  else if (event == InputEvent::RELEASE) {
    pressedState.down = false;

    if (!otherState.down) {
      bothState.down = false;
      for (auto &callback : bothState.callbacks)
        callback(event);
    }
  }
  
  for (auto &callback : pressedState.callbacks)
    callback(event);
}

#define KEY_CASE2(glfwLetter, letter) case GLFW_KEY_##glfwLetter: UpdateKeyState(action, KeyCode::letter); break;

#define KEY_CASE(letter) KEY_CASE2(letter, letter)

#define NUM_KEY_CASE(number) case GLFW_KEY_##number: UpdateKeyState(action, KeyCode::NUM##number); break;

void Input::OnKey(GLFWwindow* window, int key, int /* scanCode */, int action, int /* mods */) {
  window = window;

  switch (key) {
    KEY_CASE(Q) KEY_CASE(W) KEY_CASE(E) KEY_CASE(R) KEY_CASE(T) KEY_CASE(Y) KEY_CASE(U) KEY_CASE(I) KEY_CASE(O) KEY_CASE(P)
    KEY_CASE(A) KEY_CASE(S) KEY_CASE(D) KEY_CASE(F) KEY_CASE(G) KEY_CASE(H) KEY_CASE(J) KEY_CASE(K) KEY_CASE(L)
    KEY_CASE(Z) KEY_CASE(X) KEY_CASE(C) KEY_CASE(V) KEY_CASE(B) KEY_CASE(N) KEY_CASE(M)
    NUM_KEY_CASE(0) NUM_KEY_CASE(1) NUM_KEY_CASE(2)
    NUM_KEY_CASE(3) NUM_KEY_CASE(4) NUM_KEY_CASE(5)
    NUM_KEY_CASE(6) NUM_KEY_CASE(7) NUM_KEY_CASE(8)
    NUM_KEY_CASE(9)
    KEY_CASE(SPACE) KEY_CASE(BACKSPACE)
    KEY_CASE(LEFT) KEY_CASE(RIGHT) KEY_CASE(UP) KEY_CASE(DOWN)
    KEY_CASE(ESCAPE) KEY_CASE(SEMICOLON) KEY_CASE(APOSTROPHE)
    KEY_CASE(COMMA) KEY_CASE(PERIOD)
    KEY_CASE(SLASH) KEY_CASE(BACKSLASH)
    KEY_CASE(MINUS)
    KEY_CASE(CAPS_LOCK)
    KEY_CASE(TAB)
    KEY_CASE(ENTER)
    KEY_CASE2(RIGHT_BRACKET, RBRACKET)
    KEY_CASE2(EQUAL, EQUALS)
    KEY_CASE2(GRAVE_ACCENT, GRAVE)
    case GLFW_KEY_LEFT_SHIFT:
      UpdateLeftRightKeyState(action, KeyCode::LSHIFT, KeyCode::RSHIFT, KeyCode::SHIFT);
      break;
    case GLFW_KEY_RIGHT_SHIFT:
      UpdateLeftRightKeyState(action, KeyCode::RSHIFT, KeyCode::LSHIFT, KeyCode::SHIFT);
      break;
    case GLFW_KEY_LEFT_CONTROL:
      UpdateLeftRightKeyState(action, KeyCode::LCTRL, KeyCode::RCTRL, KeyCode::CTRL);
      break;
    case GLFW_KEY_RIGHT_CONTROL:
      UpdateLeftRightKeyState(action, KeyCode::RCTRL, KeyCode::LCTRL, KeyCode::CTRL);
      break;
    case GLFW_KEY_LEFT_ALT:
      UpdateLeftRightKeyState(action, KeyCode::LALT, KeyCode::RALT, KeyCode::ALT);
      break;
    case GLFW_KEY_RIGHT_ALT:
      UpdateLeftRightKeyState(action, KeyCode::RALT, KeyCode::LALT, KeyCode::ALT);
      break;
  }
}

// void Input::OnChar(GLFWwindow* window, unsigned codePoint) {
//   window = window;
// }

