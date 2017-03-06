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

#include <library.h>
#include <iostream>
#if defined(__linux__)
#include <dlfcn.h>
#else
#error Class 'Library' only supported on linux
#endif
#include <statics.h>

void Library::Close() {
  if (open)
    dlclose(handle);
}

bool Library::Open(const std::string &libraryPath) {
  path = ::buildPath + libraryPath;
  handle = dlopen(path.c_str(), RTLD_NOW);
  if (!handle) {
    std::cerr << "> Couldn't load shared library at " << path << ": " << dlerror() << '\n';
    return false;
  }
  open = true;
  return true;
}

void *Library::GetSymbol(const std::string &name, bool quiet) {
  void *sym = dlsym(handle, name.c_str());
  if (!sym && !quiet)
    std::cerr << "> Unable to get symbol '" << name << "' from shared library at '" << path << "'\n"
              "Error: " << dlerror() << '\n';
  return sym;
}
