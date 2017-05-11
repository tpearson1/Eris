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
#include <core/package.h>
#include <core/library.h>
#include <core/statics.h>

/*
 * Program entry point.
 * Loads a package and tries to run it
 */
int main(int argc, char const *argv[]) {
  std::string usage = "Usage: game [options] [package]";
  if (argc < 2) {
    std::cerr << usage << '\n';
    return -1;
  }

  struct { bool help = false, compile = false, noCompile = false, quiet = false, runTests = false; } options;
  int i;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      // There is an option passed
      std::string option{argv[i]};
      option.erase(0, 2);

      if (option == "help")
        options.help = true;
      else if (option == "compile")
        options.compile = true;
      else if (option == "no-compile") {
        options.noCompile = true;
        // Implies 'quiet' option
        options.quiet = true;
      }
      else if (option == "quiet")
        options.quiet = true;
      else if (option == "runtests")
        options.runTests = true;
      else {
        std::cerr << "> Unrecognized argument '" << option << "'\n" << usage << '\n';
        return -1;
      }
    }
    else
      break;
  }

  if (options.compile && options.noCompile) {
    std::cerr << "> Arguments 'compile' and 'no-compile' cannot be used together\n";
    return -1;
  }

  if (argc < i) {
    if (options.compile) {
      std::cerr << "> Argument 'compile' requires a package to be specified\n";
      return -1;
    }
    if (options.noCompile) {
      std::cerr << "> Argument 'no-compile' requires a package to be specified\n";
      return -1;
    }
  }

  if (options.help) {
    std::cout << usage << '\n'; // TODO: Better help text
    return 0;
  }

  ::SetupStatics(argv[0]);
  const auto &packageName = argv[i];
  auto *package = new Package(packageName); // Garbage collected by Package::Cleanup
  if (!package->Load(!options.noCompile, options.quiet)) {
    std::cerr << "> Unable to load package '" << packageName << "'\n";
    return -1;
  }

  if (!options.compile) {
    if (!package->Playable()) {
      std::cerr << "> Cannot play a package that isn't playable (package '" << packageName << "')\n";
      return -1;
    }

    // Set the package arguments
    for (i += 1; i < argc; i++)
      package->args.push_back(argv[i]);

    int result = package->Start(options.quiet, options.runTests);
    if (result) {
      std::cerr << "> Package '" << packageName << "' failed\n";
      return -1;
    }
  }

  Package::Cleanup();

  return 0;
}
