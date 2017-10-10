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

#include <base/callbacklist.h>
#include <base/window.h>
#include <functional>
#include <list>
#include <math/vec.h>
#include <string>
#include <unordered_map>
#include <vector>

enum class InputEvent {
  PRESS = GLFW_PRESS,
  RELEASE = GLFW_RELEASE
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
  NUM0, NUM1, NUM2, NUM3,
  NUM4, NUM5, NUM6, NUM7,
  NUM8, NUM9,
  SHIFT, LSHIFT, RSHIFT,
  CTRL, LCTRL, RCTRL,
  ALT, LALT, RALT,
  SPACE, BACKSPACE,
  LEFT, RIGHT, UP, DOWN,
  ESCAPE, LBRACKET, RBRACKET,
  SEMICOLON, APOSTROPHE,
  COMMA, PERIOD, SLASH, BACKSLASH,
  MINUS, EQUALS, CAPS_LOCK, TAB,
  GRAVE, ENTER,

  ANY
};

enum class MouseMode : int {
  NORMAL = GLFW_CURSOR_NORMAL,
  HIDDEN = GLFW_CURSOR_HIDDEN,
  DISABLED = GLFW_CURSOR_DISABLED
};

enum class MouseButton : int {
  LEFT = GLFW_MOUSE_BUTTON_LEFT,
  RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
  MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

class Input {
  Window *window;

  static void OnKey(struct GLFWwindow *window, int key, int scanCode, int action, int mods);
  // static void OnChar(struct GLFWwindow *window, unsigned codePoint);
  static void OnMouseButton(GLFWwindow *window, int button, int action, int mods);
  static void OnMouseMove(GLFWwindow *window, double xPos, double yPos);
  static void OnMouseScroll(GLFWwindow *window, double xOffset, double yOffset);

  static std::unordered_map<KeyCode, KeyState> keyStates;

  using MouseButtonCallbacks = CallbackList<void(InputEvent, int), true>;
  using MouseButtonCallback = MouseButtonCallbacks::Function;

  static std::unordered_map<MouseButton, MouseButtonCallbacks> mouseButtonCallbacks;
  static CallbackList<void(Vec2), true> mouseMoveCallbacks, mouseScrollCallbacks;

  static struct GLFWcursor *cursor;

  static void UpdateKeyState(int action, KeyCode key);
  static void UpdateLeftRightKeyState(int action, KeyCode pressed, KeyCode other, KeyCode both);

  template <typename FuncSig>
  using MouseRegistration = Registration<CallbackList<FuncSig, true>>;

public:
  Input(Window *w);
  ~Input();

  static bool IsKeyDown(KeyCode code)
    { return keyStates[code].down; }

  static int GetKey(int key)
    { return glfwGetKey(Window::GetActive()->window, key); }

  static void GetKeyName(int key, std::string &out)
    { out = glfwGetKeyName(key, 0); }

  using MouseButtonRegistration = MouseRegistration<void(InputEvent, int)>;
  using MouseMoveRegistration = MouseRegistration<void(Vec2)>;
  using MouseScrollRegistration = MouseRegistration<void(Vec2)>;

  static KeyState::RegistrationType RegisterKeyCallback(KeyCode key, KeyState::Callback callback) {
    return KeyState::RegistrationType{callback, keyStates[key].callbacks};
  }

  static void SetCursor(const std::string &path);

  static int GetMouseButton(int button)
    { return glfwGetMouseButton(Window::GetActive()->window, button); }

  static Vec2 GetMousePosition();

  static void SetMousePosition(Vec2 pos)
    { glfwSetCursorPos(Window::GetActive()->window, pos.x, pos.y); }

  static void SetMouseMode(MouseMode mode)
    { glfwSetInputMode(Window::GetActive()->window, GLFW_CURSOR, (int)mode); }

  static MouseButtonRegistration RegisterMouseButtonCallback(MouseButtonCallback callback, MouseButton button) {
    return {callback, mouseButtonCallbacks[button]};
  }

  static MouseMoveRegistration RegisterMouseMoveCallback(MouseMoveRegistration::Callback callback) {
    return {callback, mouseMoveCallbacks};
  }

  static MouseScrollRegistration RegisterMouseScrollCallback(MouseScrollRegistration::Callback callback) {
    return {callback, mouseScrollCallbacks};
  }
};

#endif // _BASE__INPUT_H
