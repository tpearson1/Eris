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

#ifndef _CORE__LIBRARY_H
#define _CORE__LIBRARY_H

#include <string>

/*
 * Class for loading a library
 */
class Library {
  /*
   * Prevent copy construction and copy assignment
   */
  Library(const Library &other);
  Library &operator=(const Library &other);

  /*
   * Handle pointer to the library
   */
  void *handle;

  /*
   * Path to the library
   */
  std::string path;

  /*
   * Whether or not the library is currently open
   */
  bool open = false;

public:
  Library() {}

  /*
   * Closes the library if it's open
   */
  void Close();

  bool IsOpen() const { return open; }

  /*
   * Opens the library
   * @param libraryPath The path to the library
   * @returns whether opening the library succeeded
   */
  bool Open(const std::string &libraryPath);

  /*
   * Gets a pointer to a symbol in the library
   * @param name The name of the symbol
   * @returns a pointer to the symbol
   */
  void *GetSymbol(const std::string &name, bool quiet = false);
};

#endif // _CORE__LIBRARY_H
