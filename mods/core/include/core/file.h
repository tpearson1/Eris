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

#ifndef _CORE__FILE_H
#define _CORE__FILE_H

#include <vector>
#include <string>
#include <stdexcept>

struct FileAccessException : public std::runtime_error {
  FileAccessException(const std::string &error) : std::runtime_error(error) {}
};

/*
 * Static class for file manipulation
 */
class File {
  /*
   * As class is static, we don't want to be able to create File objects
   */
  File() = delete;

public:
  /*
   * Checks whether a file exists
   * @param path The file path to test
   * @returns true if the file exists
   */
  static bool Exists(const std::string &path);

  /*
   * Reads a file into a string
   * @param path The path of the file
   * @param out The string buffer to read the file into
   */
  static void Read(const std::string &path, std::string &out);
};

/*
 * Static class for directory queries
 */
class Directory {
  /*
   * As class is static, we don't want to be able to create Directory objects
   */
  Directory();
  ~Directory();
  Directory(const Directory &other);
  Directory &operator=(const Directory &other);

  /*
   * Helper function for getting files inside the directory recursively
   * @param dir The path of the directory
   * @param out Buffer for all the file paths found
   */
  static void _GetFilesRecursive(const std::string &dir, std::vector<std::string> &out);

public:
  /*
   * Checks whether a directory exists
   * @param path The path of the directory to test
   * @returns true if the directory exists
   */
  static bool Exists(const std::string &path);

  /*
   * Gets all the files inside the directory recursively
   * @param dir The path of the directory
   * @param out Buffer for all the file paths found
   */
  static void GetFilesRecursive(const std::string &dir, std::vector<std::string> &out);

  /*
   * Gets all the files immediately inside the directory
   * @param dir The path of the directory
   * @param out Buffer for all the files found
   */
  static void GetFiles(const std::string &dir, std::vector<std::string> &out);

  /*
   * Gets all folders immediately inside the directory
   * @param dir The path of the directory
   * @param out Buffer for all the directories found
   */
  static void GetFolders(const std::string &dir, std::vector<std::string> &out);
};

#endif // _CORE__FILE_H
