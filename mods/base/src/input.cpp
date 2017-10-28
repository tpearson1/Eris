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

#include <cstring>
#include <iostream>

#include <image.h>
#include <window.h>

#include <core/statics.h>

static Input &GetInputFromWindow(GLFWwindow *window) {
  return GetWindowFromGLFWwindow(window)->GetInput();
}

void Input::OnMouseButton(int button, int action) {
  auto &callbackList = mouseButtonCallbacks[static_cast<MouseButton>(button)];
  callbackList.CallAll(static_cast<InputEvent>(action));
}

void Input::OnMouseMove(double xPos, double yPos) {
  mouseMoveCallbacks.CallAll(
      Vec2{static_cast<float>(xPos), static_cast<float>(yPos)});
}

void Input::OnMouseScroll(double xOffset, double yOffset) {
  mouseScrollCallbacks.CallAll(
      Vec2{static_cast<float>(xOffset), static_cast<float>(yOffset)});
}

Input::Input(GLFWwindow *w) : window(w) {
  glfwSetCursorPosCallback(window, [](auto win, auto x, auto y) {
    GetInputFromWindow(win).OnMouseMove(x, y);
  });
  glfwSetMouseButtonCallback(
      window, [](auto win, auto button, auto action, auto /* modifiers */) {
        GetInputFromWindow(win).OnMouseButton(button, action);
      });
  glfwSetScrollCallback(window, [](auto win, auto x, auto y) {
    GetInputFromWindow(win).OnMouseScroll(x, y);
  });

  glfwSetKeyCallback(window, [](auto win, auto key, auto scanCode,
                                   auto action, auto /* modifiers */) {
    GetInputFromWindow(win).OnKey(key, scanCode, action);
  });
  /* glfwSetCharCallback(window, [](auto win, auto codePoint) { */
  /*   GetInputFromWindow(win).OnChar(codePoint); */
  /* }); */
}

Input::~Input() {
  if (cursor) glfwDestroyCursor(cursor);
}

void Input::SetCursor(const std::string &path) {
  RawImage r;
  r.Load(path);

  GLFWimage image;
  image.width = r.size.x;
  image.height = r.size.y;
  image.pixels = r.data.data();

  if (cursor) glfwDestroyCursor(cursor);

  cursor = glfwCreateCursor(&image, 0, 0);
  glfwSetCursor(window, cursor);
}

Vec2 Input::GetMousePosition() {
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  return {static_cast<float>(x), static_cast<float>(y)};
}

void Input::UpdateKeyState(int action, KeyCode key) {
  auto event = static_cast<InputEvent>(action);
  auto &keyState = keyStates[key];
  if (event == InputEvent::Press)
    keyState.down = true;
  else if (event == InputEvent::Release)
    keyState.down = false;

  keyState.callbacks.CallAll(event);
}

// Update KeyStates like LShift, RShift and Shift
void Input::UpdateLeftRightKeyState(int action, KeyCode pressed, KeyCode other,
                                    KeyCode both) {
  auto event = static_cast<InputEvent>(action);
  auto &pressedState = keyStates[pressed], &bothState = keyStates[both],
       &otherState = keyStates[other];
  if (event == InputEvent::Press) {
    pressedState.down = true;
    bothState.down = true;

    if (!otherState.down) {
      // Left button down and right button is not, so trigger bothState's
      // callbacks
      bothState.callbacks.CallAll(event);
    }
  } else if (event == InputEvent::Release) {
    pressedState.down = false;

    if (!otherState.down) {
      // Both are released
      bothState.down = false;
      // Call bothState's callbacks to notify notify that its state changed
      bothState.callbacks.CallAll(event);
    }
  }

  pressedState.callbacks.CallAll(event);
}

#define KEY_CASE2(glfwLetter, letter)                                          \
  case GLFW_KEY_##glfwLetter:                                                  \
    UpdateKeyState(action, KeyCode::letter);\
    break;
#define KEY_CASE(letter) KEY_CASE2(letter, letter)

#define NUM_KEY_CASE(number)                                                   \
  case GLFW_KEY_##number:                                                      \
    UpdateKeyState(action, KeyCode::Num##number);                              \
    break;
void Input::OnKey(int key, int /* scanCode */, int action) {
  switch (key) {
    KEY_CASE(Q) KEY_CASE(W) KEY_CASE(E) KEY_CASE(R) KEY_CASE(T) KEY_CASE(Y) KEY_CASE(U) KEY_CASE(I) KEY_CASE(O) KEY_CASE(P)
    KEY_CASE(A) KEY_CASE(S) KEY_CASE(D) KEY_CASE(F) KEY_CASE(G) KEY_CASE(H) KEY_CASE(J) KEY_CASE(K) KEY_CASE(L)
    KEY_CASE(Z) KEY_CASE(X) KEY_CASE(C) KEY_CASE(V) KEY_CASE(B) KEY_CASE(N) KEY_CASE(M)
    NUM_KEY_CASE(0) NUM_KEY_CASE(1) NUM_KEY_CASE(2)
    NUM_KEY_CASE(3) NUM_KEY_CASE(4) NUM_KEY_CASE(5)
    NUM_KEY_CASE(6) NUM_KEY_CASE(7) NUM_KEY_CASE(8)
    NUM_KEY_CASE(9)
    KEY_CASE2(SPACE, Space) KEY_CASE2(BACKSPACE, Backspace)
    KEY_CASE2(LEFT, Left) KEY_CASE2(RIGHT, Right) KEY_CASE2(UP, Up) KEY_CASE2(DOWN, Down)
    KEY_CASE2(ESCAPE, Escape) KEY_CASE2(SEMICOLON, Semicolon) KEY_CASE2(APOSTROPHE, Apostrophe)
    KEY_CASE2(COMMA, Comma) KEY_CASE2(PERIOD, Period)
    KEY_CASE2(SLASH, Slash) KEY_CASE2(BACKSLASH, Backslash)
    KEY_CASE2(MINUS, Minus)
    KEY_CASE2(CAPS_LOCK, CapsLock)
    KEY_CASE2(TAB, Tab)
    KEY_CASE2(ENTER, Enter)
    KEY_CASE2(RIGHT_BRACKET, RBracket)
    KEY_CASE2(EQUAL, Equals)
    KEY_CASE2(GRAVE_ACCENT, Grave)
    case GLFW_KEY_LEFT_SHIFT:
      UpdateLeftRightKeyState(action, KeyCode::LShift, KeyCode::RShift, KeyCode::Shift);
      break;
    case GLFW_KEY_RIGHT_SHIFT:
      UpdateLeftRightKeyState(action, KeyCode::RShift, KeyCode::LShift, KeyCode::Shift);
      break;
    case GLFW_KEY_LEFT_CONTROL:
      UpdateLeftRightKeyState(action, KeyCode::LCtrl, KeyCode::RCtrl, KeyCode::Ctrl);
      break;
    case GLFW_KEY_RIGHT_CONTROL:
      UpdateLeftRightKeyState(action, KeyCode::RCtrl, KeyCode::LCtrl, KeyCode::Ctrl);
      break;
    case GLFW_KEY_LEFT_ALT:
      UpdateLeftRightKeyState(action, KeyCode::LAlt, KeyCode::RAlt, KeyCode::Alt);
      break;
    case GLFW_KEY_RIGHT_ALT:
      UpdateLeftRightKeyState(action, KeyCode::RAlt, KeyCode::LAlt, KeyCode::Alt);
      break;
  }
}

// void Input::OnChar(unsigned codePoint) {
//
// }
