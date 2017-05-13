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

#include <functional>
#include <list>
#include <base/gl.h>

struct GLFWwindow;

/*
 * Class for creating and manipulating a window
 */
class Window {
  /*
   * Prevent copying
   */
  Window(const Window &other);
  Window &operator=(const Window &other);

  using ResizeCallback = std::function<void(int, int)>;
  std::list<ResizeCallback> resizeCallbacks;

  static void OnResize(GLFWwindow *window, int width, int height);

public:
  /*
   * The window instance
   */
  static Window *inst;

  /*
   * The width and height of the window in pixels
   */
  int width = 640, height = 480;

  /*
   * The GLFW representation of the window
   */
  GLFWwindow *window;

  Window();
  ~Window();

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

  auto RegisterResizeCallback(ResizeCallback callback)
    { resizeCallbacks.push_front(callback); return resizeCallbacks.begin(); }

  void UnregisterResizeCallback(std::list<ResizeCallback>::iterator it)
    { resizeCallbacks.erase(it); }
};

#endif // _BASE__WINDOW_H
