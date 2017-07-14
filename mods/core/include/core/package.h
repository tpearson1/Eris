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
#include <memory>
#include <core/mapping.h>

/*
 * Class for loading and manipulating a package
 */
class Package {
  Package(const Package &other) = delete;
  Package &operator=(const Package &other) = delete;

  Package(const std::string &_name) : name(_name), path("mods/" + _name + '/') {}

  mutable bool compilation_tried = false;

public:
  static Package *Create(const std::string &name);

  struct LoadOptions {
    bool compile = true;
    bool quiet = false;
  };

  struct Data {
    std::string author, version;
    std::string linkOptions;
    bool playable = false;
    bool hasHeaders = true, usesCPP = true, compile = true;

    LoadOptions loadOptions;

    std::vector<std::string> dependencies;

    Data(const LoadOptions &options) : loadOptions(options) {}
  };

  static std::unordered_map<std::string, std::unique_ptr<Package>> loaded;

  const std::string &name;
  std::string author, version, path;
  bool playable = false, usesCPP = true, compile = true, hasHeaders = true;
  std::vector<Package *> dependencies;
  std::string linkOptions;

  /*
   * The result of a package compilation
   */
  struct CompilationResult {
    enum {
      SUCCESS,
      UNNECESSARY,
      DEPENDENCY_FAILURE,
      FAILURE
    } value;

    CompilationResult(decltype(value) result) : value(result) {}

    bool Successful() {
      return value == SUCCESS || value == UNNECESSARY;
    }

    operator bool() {
      return Successful();
    }
  };

  struct CompilationOptions {
    bool quiet = false;
  };

  /*
   * Compiles the package, also compiling dependencies if necessary
   * @returns the exit status of the compilation
   */
  Package::CompilationResult Compile(const CompilationOptions &options) const;

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
   * Loads the package
   * @returns whether the operation succeeded
   */
  bool Load(const Data &data);

  struct RunOptions {
    std::vector<std::string> args;
    bool quiet = false;
  };

  static std::vector<std::string> args;

  /*
   * Starts running the package if it's playable
   * @returns whether the package succeeded
   */
  bool Run(const RunOptions &options);

  struct TestOptions {
    bool compileTests = true;
    bool recursive = false;
    bool quiet = false;
  };

  /*
   * Runs tests for the package
   * @returns whether testing / compiling tests were succesful
   */
  bool Test(const TestOptions &options);
};

template <>
struct JSONImpl<Package::Data> {
  static void Read(Package::Data &out, const JSON::Value &value, const JSON::ReadData &data);
  static void Write(const Package::Data &value, JSON::Writer &writer);
};

#endif // _CORE__PACKAGE_H
