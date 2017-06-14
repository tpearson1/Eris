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

#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <core/package.h>
#include <core/library.h>
#include <core/statics.h>
#include <core/file.h>

static constexpr const auto usage = "Usage: load [options] [package]\nFor more "
                                    "detailed help information try 'load -H'";

struct LoadInfo {
  std::string name;
  bool compile = false;
  bool run = true;
  bool test = false, testRecursive = false;
  bool quiet = false;
  std::vector<std::string> args;

  /*
   * Check that options do not contradict each other
   */
  bool NoCollisions() const;
};

bool LoadInfo::NoCollisions() const {
  if (name.empty()) {
    if (compile) {
      std::cerr << "Argument 'compile' requires a package to be specified\n";
      return false;
    }

    if (run) {
      std::cerr << "A package must be specified\n";
      return false;
    }

    if (test || testRecursive) {
      std::cerr << "A package must be specified if you want to run tests\n";
      return false;
    }
  }

  return true;
}

static int HandlePackage(const LoadInfo &info) {
  if (!info.NoCollisions())
    return -1;

  auto package = Package::Create(info.name);

  Package::LoadOptions lo;
  lo.compile = info.compile;
  lo.quiet = info.quiet;

  Package::Data data{lo};
  {
    auto typeManager = std::make_shared<JSON::TypeManager>();
    JSON::Document doc;
    JSON::GetDataFromFile(doc, "mods/" + info.name + "/package.json");
    JSON::ReadData readData{typeManager};
    JSON::Read(data, doc, readData);
  }

  package->Load(data);
  if (info.compile) {
    Package::CompilationOptions co;
    co.quiet = info.quiet;
    if (package->Compile(co))
      return -1;
  }

  if (info.test || info.testRecursive) {
    Package::TestOptions to;
    to.compileTests = info.compile;
    to.recursive = info.testRecursive;
    to.quiet = info.quiet;
    if (!package->Test(to))
      return -1;
  }

  if (info.run) {
    Package::RunOptions ro;
    ro.args = info.args;
    ro.quiet = info.quiet;
    if (!package->Run(ro))
      return -1;
  }

  return 0;
}

// Returns false when all arguments are handled
static bool HandleArgument(int &out, std::vector<std::string> &revArgs, LoadInfo &info) {
  assert(!revArgs.empty());
  auto arg = revArgs.back();
  revArgs.pop_back();

  if (arg[0] != '-') {
    info.name = arg;
    out = HandlePackage(info);

    // Add package arguments
    std::reverse(std::begin(revArgs), std::end(revArgs));
    info.args = revArgs;

    return false;
  }

  arg.erase(0, 1);

  if (arg == "h" || arg == "-help") {
    std::cout << usage << '\n';
    return false;
  }

  if (arg == "H" || arg == "-help-full") {
    std::string helpFull;
    try {
      File::Read("help.txt", helpFull);
    }
    catch (FileAccessException e) {
      std::cerr << "Error reading help file. File 'help.txt' may not be present\n";
    }
    std::cout << helpFull;
    return false;
  }

  if (arg == "v" || arg == "-version") {
    std::cout << "Version " << ::version << '\n';
    return false;
  }

  if (arg == "n" || arg == "-no-compile") {
    info.compile = false;
    return true;
  }

  if (arg == "c" || arg == "-compile") {
    info.compile = true;
    return true;
  }

  if (arg == "C" || arg == "-only-compile") {
    info.compile = true;
    info.run = false;
    return true;
  }

  if (arg == "q" || arg == "-quiet") {
    info.quiet = true;
    return true;
  }

  if (arg == "t" || arg == "-run-tests") {
    info.run = false;
    info.test = true;
    return true;
  }

  if (arg == "T" || arg == "-run-tests-rec") {
    info.run = false;
    info.testRecursive = true;
    return true;
  }

  std::cout << usage << '\n';
  return false;
}

/*
 * Program entry point.
 * Loads a package and tries to run it.
 */
int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cerr << usage << '\n';
    return -1;
  }

  // Vector with all arguments backwards and without program name
  std::vector<std::string> args;
  for (auto i = argc - 1; i > 0; i--)
    args.push_back(argv[i]);

  ::SetupStatics(argv[0]);

  LoadInfo info;
  int ret = 0;
  while (HandleArgument(ret, args, info) && !ret);

  return ret;
}

