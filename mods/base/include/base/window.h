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

#ifndef _BASE__WINDOW_H
#define _BASE__WINDOW_H

#include <base/gl.h>
#include <functional>
#include <list>
#include <math/vec.h>
#include <unordered_map>

struct GLFWwindow;

/*
 * Class for creating and manipulating a window
 */
class Window {
  static std::unordered_map<GLFWwindow *, Window *> windowMapping;
  static Window *active;

  /*
   * Prevent copying
   */
  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;

  using ResizeCallback = std::function<void(IVec2)>;
  std::list<ResizeCallback> resizeCallbacks;

  static void OnResize(GLFWwindow *window, int width, int height);

  IVec2 size;

  /*
   * The GLFW representation of the window
   */
  GLFWwindow *window;

  static void SetViewportSize(IVec2 size);

public:
  /*
   * The size of the window in pixels
   */
  IVec2 GetSize() const { return size; }

  Window(IVec2 size);
  ~Window() { Close(); }

  static Window *GetActive() { return active; }

  bool IsActive() const { return GetActive() == this; }
  void MakeActive() {
    glfwMakeContextCurrent(window);
    active = this;
  }

  /*
   * Whether or not the window should Close
   * @returns whether the window should close
   */
  bool ShouldClose() const { return glfwWindowShouldClose(window); }

  /*
   * Swaps the window buffer
   */
  void SwapBuffers() const { glfwSwapBuffers(window); }

  /*
   * Closes the window
   */
  void Close();

  auto RegisterResizeCallback(ResizeCallback callback) {
    resizeCallbacks.push_front(callback);
    return resizeCallbacks.begin();
  }

  void UnregisterResizeCallback(std::list<ResizeCallback>::iterator it) {
    resizeCallbacks.erase(it);
  }

  friend class Input;
};

#endif // _BASE__WINDOW_H
