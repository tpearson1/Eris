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

#include <file.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#if defined(__linux__) || defined(__CYGWIN__)
#include <dirent.h>
#include <unistd.h>
#else
#error Classes 'File' and 'Directory' only supported on linux and cygwin
#endif
#include <statics.h>

bool File::Exists(const std::string &path) {
  return access((::buildPath + path).c_str(), F_OK) == 0;
}

void File::Read(const std::string &path, std::string &out) {
  std::string fullPath = ::buildPath + path;
  std::ifstream file{fullPath, std::ios::in};
  if (file.is_open()) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();
  }
  else {
    out = "";
    throw FileAccessException("Unable to open file: " + fullPath);
  }
}

void Directory::_GetFilesRecursive(const std::string &dir, std::vector<std::string> &out) {
  std::vector<std::string> tmp;
  std::string d = ::buildPath + dir;
  GetFolders(d, tmp);
  for (size_t i = 0; i < tmp.size(); i++)
    _GetFilesRecursive(d + "/" + tmp[i], out);

  size_t outSize = out.size();
  GetFiles(d, out);
  for (size_t j = outSize; j < out.size(); j++) {
    out[j] = d + "/" + out[j];
  }
}


bool Directory::Exists(const std::string &path) {
  std::string tmp{::buildPath + path};
  if (tmp[tmp.size() - 1] != '/')
    tmp.append("/");

  return access(tmp.c_str(), F_OK) == 0;
}

void Directory::GetFilesRecursive(const std::string &dir, std::vector<std::string> &out) {
  std::string tmp{::buildPath + dir};
  if (tmp[tmp.size() - 1] == '/')
    tmp.append(".");

  _GetFilesRecursive(tmp, out);
}

void Directory::GetFiles(const std::string &dir, std::vector<std::string> &out) {
  dirent *epdf;
  DIR *dpdf = opendir((::buildPath + dir).c_str());
  if (dpdf)
    while ((epdf = readdir(dpdf)))
      if (epdf->d_type == DT_REG)
        out.push_back(epdf->d_name);
  closedir(dpdf);
}

void Directory::GetFolders(const std::string &dir, std::vector<std::string> &out) {
  dirent *epdf;
  DIR *dpdf = opendir((::buildPath + dir).c_str());
  if (dpdf)
    while ((epdf = readdir(dpdf)))
      if (epdf->d_type == DT_DIR) {
        std::string name{epdf->d_name};
        if (name != "." && name != "..")
          out.push_back(epdf->d_name);
      }
  closedir(dpdf);
}
