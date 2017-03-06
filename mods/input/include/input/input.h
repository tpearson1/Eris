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

#ifndef _INPUT__INPUT_H
#define _INPUT__INPUT_H

#include <string>
#include <unordered_map>
#include <list>
#include <functional>
#include <base/window.h>

enum class InputEvent {
  PRESS = GLFW_PRESS,
  RELEASE = GLFW_RELEASE
};

struct KeyState {
  bool down = false;

  using Callback = std::function<void(InputEvent)>;
  std::list<Callback> callbacks;
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
  static void OnKey(struct GLFWwindow *window, int key, int scanCode, int action, int mods);
  // static void OnChar(struct GLFWwindow *window, unsigned codePoint);
  static void OnMouseButton(GLFWwindow *window, int button, int action, int mods);
  static void OnMouseMove(GLFWwindow *window, double xPos, double yPos);
  static void OnMouseScroll(GLFWwindow *window, double xOffset, double yOffset);
  
  static std::unordered_map<KeyCode, KeyState> keyStates;

  using MouseButtonCallback = std::function<void(int, int)>;
  using MouseMoveCallback = std::function<void(double, double)>;
  using MouseScrollCallback = std::function<void(double, double)>;

  static std::unordered_map<MouseButton, std::list<MouseButtonCallback>> mouseButtonCallbacks;
  static std::list<MouseMoveCallback> mouseMoveCallbacks;
  static std::list<MouseScrollCallback> mouseScrollCallbacks;

  static struct GLFWcursor *cursor;

  static void UpdateKeyState(int action, KeyCode key);
  static void UpdateLeftRightKeyState(int action, KeyCode pressed, KeyCode other, KeyCode both);

public:
  static void Setup();
  static void Cleanup();

  static bool IsKeyDown(KeyCode code)
    { return keyStates[code].down; }

  static int GetKey(int key)
    { return glfwGetKey(Window::inst->window, key); }

  static void GetKeyName(int key, std::string &out)
    { out = glfwGetKeyName(key, 0); }

  static auto RegisterKeyCallback(KeyCode key, KeyState::Callback callback) {
    auto &l = keyStates[key];
    l.callbacks.push_front(callback);
    return l.callbacks.begin();
  }

  static void UnregisterKeyCallback(KeyCode key, std::list<KeyState::Callback>::iterator it)
    { keyStates[key].callbacks.erase(it); }

  static void SetCursor(const std::string &path);

  static int GetMouseButton(int button)
    { return glfwGetMouseButton(Window::inst->window, button); }

  static void GetMousePosition(double &outX, double &outY)
    { glfwGetCursorPos(Window::inst->window, &outX, &outY); }
  static void SetMousePosition(int x, int y)
    { glfwSetCursorPos(Window::inst->window, x, y); }

  static void SetMouseMode(MouseMode mode)
    { glfwSetInputMode(Window::inst->window, GLFW_CURSOR, (int)mode); }

  static auto RegisterMouseButtonCallback(MouseButtonCallback callback, MouseButton button) {
    auto &l = mouseButtonCallbacks[button];
    l.push_front(callback);
    return l.begin();
  }
  static auto RegisterMouseMoveCallback(MouseMoveCallback callback)
    { mouseMoveCallbacks.push_front(callback); return mouseMoveCallbacks.begin(); }
  static auto RegisterMouseScrollCallback(MouseScrollCallback callback)
    { mouseScrollCallbacks.push_front(callback); return mouseScrollCallbacks.begin(); }
  
  static void UnregisterMouseButtonCallback(std::list<MouseButtonCallback>::iterator it, MouseButton button)
    { mouseButtonCallbacks[button].erase(it); }
  static void UnregisterMouseMoveCallback(std::list<MouseMoveCallback>::iterator it)
    { mouseMoveCallbacks.erase(it); }
  static void UnregisterMouseScrollCallback(std::list<MouseScrollCallback>::iterator it)
    { mouseScrollCallbacks.erase(it); }
};

#endif // _INPUT__INPUT_H