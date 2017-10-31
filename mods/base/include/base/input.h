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

#ifndef _BASE__INPUT_H
#define _BASE__INPUT_H

#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <base/gl.h>
#include <base/registration.h>

#include <math/vec.h>

enum class InputEvent {
  Press = GLFW_PRESS,
  Release = GLFW_RELEASE
};

struct KeyState {
  bool down = false;

  using CallbackSignature = void(InputEvent);
  using Callbacks = CallbackList<CallbackSignature, true>;
  using Callback = typename Callbacks::Function;
  Callbacks callbacks;

  using RegistrationType = Registration<Callbacks>;
};

enum class KeyCode {
  Q, W, E, R, T, Y, U, I, O, P,
  A, S, D, F, G, H, J, K, L,
  Z, X, C, V, B, N, M,
  Num0, Num1, Num2, Num3,
  Num4, Num5, Num6, Num7,
  Num8, Num9,
  Shift, LShift, RShift,
  Ctrl, LCtrl, RCtrl,
  Alt, LAlt, RAlt,
  Space, Backspace,
  Left, Right, Up, Down,
  Escape, LBracket, RBracket,
  Semicolon, Apostrophe,
  Comma, Period, Slash, Backslash,
  Minus, Equals, CapsLock, Tab,
  Grave, Enter,

  Any
};

enum class MouseMode : int {
  Normal = GLFW_CURSOR_NORMAL,
  Hidden = GLFW_CURSOR_HIDDEN,
  Disabled = GLFW_CURSOR_DISABLED
};

enum class MouseButton : int {
  Left = GLFW_MOUSE_BUTTON_LEFT,
  Right = GLFW_MOUSE_BUTTON_RIGHT,
  Middle = GLFW_MOUSE_BUTTON_MIDDLE
};

class Input {
  GLFWwindow *window;

  void OnKey(int key, int scanCode, int action);
  /* void OnChar(unsigned codePoint); */
  void OnMouseButton(int button, int action);
  void OnMouseMove(double xPos, double yPos);
  void OnMouseScroll(double xOffset, double yOffset);

  std::unordered_map<KeyCode, KeyState> keyStates;

  using MouseButtonCallbacks = CallbackList<void(InputEvent), true>;
  using MouseButtonCallback = MouseButtonCallbacks::Function;

  std::unordered_map<MouseButton, MouseButtonCallbacks> mouseButtonCallbacks;
  CallbackList<void(Vec2), true> mouseMoveCallbacks, mouseScrollCallbacks;

  struct GLFWcursor *cursor = nullptr;

  void UpdateKeyState(int action, KeyCode key);
  void UpdateLeftRightKeyState(int action, KeyCode pressed, KeyCode other, KeyCode both);

  template <typename FuncSig>
  using MouseRegistration = Registration<CallbackList<FuncSig, true>>;

public:
  Input(GLFWwindow *w);
  ~Input();

  Input(Input &) = delete;

  bool IsKeyDown(KeyCode code)
    { return keyStates[code].down; }

  int GetKey(int key)
    { return glfwGetKey(window, key); }

  void GetKeyName(int key, std::string &out)
    { out = glfwGetKeyName(key, 0); }

  using MouseButtonRegistration = MouseRegistration<void(InputEvent)>;
  using MouseMoveRegistration = MouseRegistration<void(Vec2)>;
  using MouseScrollRegistration = MouseRegistration<void(Vec2)>;

  KeyState::RegistrationType RegisterKeyCallback(KeyCode key, KeyState::Callback callback) {
    return KeyState::RegistrationType{callback, keyStates[key].callbacks};
  }

  void SetCursor(const std::string &path);

  int GetMouseButton(int button)
    { return glfwGetMouseButton(window, button); }

  Vec2 GetMousePosition();

  void SetMousePosition(Vec2 pos)
    { glfwSetCursorPos(window, pos.x, pos.y); }

  void SetMouseMode(MouseMode mode)
    { glfwSetInputMode(window, GLFW_CURSOR, (int)mode); }

  MouseButtonRegistration RegisterMouseButtonCallback(MouseButtonCallback callback, MouseButton button) {
    return {callback, mouseButtonCallbacks[button]};
  }

  MouseMoveRegistration RegisterMouseMoveCallback(MouseMoveRegistration::Callback callback) {
    return {callback, mouseMoveCallbacks};
  }

  MouseScrollRegistration RegisterMouseScrollCallback(MouseScrollRegistration::Callback callback) {
    return {callback, mouseScrollCallbacks};
  }
};

#endif // _BASE__INPUT_H
