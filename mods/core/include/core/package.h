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

#ifndef _CORE__PACKAGE_H
#define _CORE__PACKAGE_H

#include <string>
#include <vector>

/*
 * Class for loading and manipulating a package
 */
class Package {
  Package(const Package &other) = delete;
  Package &operator=(const Package &other) = delete;

  /*
   * Reads the JSON data associated with the file
   * @param compileDependencies Whether or not the package's dependencies should be compiled
   * @returns whether the operation succeeded
   */
  bool ReadPackageJSON(bool compileDependencies = true, bool quiet = false);

  /*
   * Helper function for the Compile function
   * @param deps The packages that are depended by the package being loaded
   * @param out The stringstream for the string generated
   */
  void RecursiveInclude(const std::vector<Package *> &deps, std::vector<Package *> &processed, std::stringstream &out) const;

  /*
   * Compiles the package, also compiling dependencies if necessary
   * @returns the exit status of the compilation
   */
  int Compile(bool quiet = false) const;

  /*
   * Whether the package is playable and whether it contains C++ code
   */
  bool playable = false, usesCPP = true, headerOnly = false;

  /*
   * All the packages currently loaded
   */
  static std::vector<Package *> all;

  /*
   * This package's dependencies
   */
  std::vector<Package *> dependencies;

  /*
   * Package data; the package's path, the name of the package, it's author,
   * version and any link options
   */
  std::string path, name, author, version, linkOptions;

public:
  /*
   * The active package
   */
  static Package *active;

  /*
   * Struct with options for what to do once the package ends execution
   */
  struct RestartOptions {
    /*
     * Whether or not the package should restart
     */
    bool should = false;

    /*
     * Message to print out if the packages exits with a non-zero return code
     */
    std::string errorMessage = "";

    /*
     * If this is true, when the package restarts it will have the newArgs
     * arguments instead of the arguments originally passed
     */
    bool useNewArgs = false;

    /*
     * The new set of arguments that should be passed to the package when it
     * restarts
     */
    std::vector<std::string> newArgs;

    RestartOptions() {}
    RestartOptions(bool _should, const std::string &errorMsg, bool _useNewArgs = false, const std::vector<std::string> &_newArgs = {})
      : should(_should), errorMessage(errorMsg), useNewArgs(_useNewArgs), newArgs(_newArgs) {}
  };

  /*
   * Options for whether or not the package should restart once it finishes
   */
  RestartOptions restartOptions;

  /*
   * Arguments to the package
   */
  std::vector<std::string> args;

  const std::vector<Package *> &Dependencies() const { return dependencies; }
  std::string Name() const { return name; }
  std::string Path() const { return path; }
  bool Playable() const { return playable; }

  Package() {}
  Package(const std::string &pkgName) : name(pkgName) {}

  /*
   * Loads the package
   * @param compile Whether or not the package should try to compile
   * @returns whether the operation succeeded
   */
  bool Load(bool compile = true, bool quiet = false);

  /*
   * Starts running the package if it's playable
   * @returns whether the package succeeded
   */
  bool Start(bool quiet = false, bool runTests = false);

  /*
   * Cleans up memory
   */
  static void Cleanup();
};

#endif // _CORE__PACKAGE_H
