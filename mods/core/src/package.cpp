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

#include <package.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <library.h>
#include <termcolor.h>
#include <statics.h>
#include <file.h>

Package *Package::active;
std::vector<std::string> Package::args;

std::unordered_map<std::string, std::unique_ptr<Package>> Package::loaded;

Package *Package::Create(const std::string &name) {
  auto it = loaded.emplace(name, nullptr).first;
  auto &package = it->second;
  // Cannot use make_unique because constructor private
  package = std::unique_ptr<Package>(new Package(it->first));
  return package.get();
}

static void RecursiveIncludeHelper(const std::vector<Package *> &deps,
                                   std::vector<Package *> &processed,
                                   std::ostringstream &out,
                                   const std::string &prefix) {
  for (auto &elem : deps) {
    if (std::find(std::begin(processed), std::end(processed), elem) != std::end(processed))
      continue;

    if (elem->hasHeaders) {
      out << " -I" << prefix << elem->name << "/include";
      processed.push_back(elem);
    }
    RecursiveIncludeHelper(elem->dependencies, processed, out, prefix);
  }
}

static void RecursiveInclude(const std::vector<Package *> &deps,
                             std::ostringstream &out,
                             const std::string &prefix = "") {
  std::vector<Package *> processed;
  RecursiveIncludeHelper(deps, processed, out, prefix);
}

static void AddLibraryParameter(const std::string &name,
                                std::ostringstream &command,
                                const std::string &prefix = "",
                                const std::string &originSuffix = "../") {
    command << " -Wl,";
    command << R"x(-rpath=\\$\$\$\ORIGIN/)x" << originSuffix << name
               << " -L" << prefix << name << " -l" << name;
}

static void AddLibraryParameterRecursiveHelper(
    const std::vector<Package *> &deps, std::vector<Package *> &processed,
    std::ostringstream &command, const std::string &prefix,
    const std::string &originSuffix) {
  for (auto &elem : deps) {
    if (std::find(std::begin(processed), std::end(processed), elem) != std::end(processed))
      continue;

    if (elem->usesCPP) {
      AddLibraryParameter(elem->name, command, prefix, originSuffix);
      processed.push_back(elem);
    }
    AddLibraryParameterRecursiveHelper(elem->dependencies, processed, command, prefix, originSuffix);
  }
}

static void AddLibraryParameterRecursive(
    const std::vector<Package *> &deps, std::ostringstream &command,
    const std::string &prefix = "", const std::string &originSuffix = "../") {
  std::vector<Package *> processed;
  AddLibraryParameterRecursiveHelper(deps, processed, command, prefix, originSuffix);
}

Package::CompilationResult Package::Compile(const CompilationOptions &options) const {
  if (compilation_tried)
    return CompilationResult::UNNECESSARY;
  compilation_tried = true;

  for (auto &package : dependencies)
    if (!package->Compile(options))
      return CompilationResult::DEPENDENCY_FAILURE;
  if (!usesCPP || !compile)
    return CompilationResult::UNNECESSARY;

  if (!options.quiet)
    std::clog << TermColor::FG_BLUE << "-- Compiling " << name << "@v" << version << " by " << author << TermColor::FG_DEFAULT << '\n';

  std::ostringstream command;
  command << "cd mods;make -s " << (options.quiet ? "SILENT=\".\" " : "") << "MOD=\"" << name << '"';
  command << " LIBS=\"" << linkOptions;
  for (auto &dep : dependencies) {
    if (!dep->usesCPP)
      continue;
    AddLibraryParameter(dep->name, command);
  }

  command << "\" INCLUDE=\"";
  RecursiveInclude(dependencies, command);

  command << "\" TARGET=\"" << name << "/lib" << name << ".so\"";

  int result = WEXITSTATUS(std::system(command.str().c_str()));
  if (result)
    return CompilationResult::FAILURE;
  return CompilationResult::SUCCESS;
}

bool Package::Load(const Data &data) {
  path = "mods/" + name + '/';
  author = data.author;
  version = data.version;
  playable = data.playable;
  usesCPP = data.usesCPP;
  hasHeaders = data.hasHeaders;
  compile = data.compile;
  linkOptions = data.linkOptions;

  auto typeManager = std::make_shared<JSON::TypeManager>();
  try {
    for (auto &depString : data.dependencies) {
      auto it = loaded.find(depString);
      if (it != std::end(loaded)) {
        dependencies.push_back(it->second.get());
        continue;
      }

      JSON::Document doc;
      JSON::GetDataFromFile(doc, "mods/" + depString + "/package.json");

      auto newData = data;
      newData.dependencies.clear();

      JSON::ReadData readData{typeManager};
      JSON::Read(newData, doc, readData);

      auto package = Create(depString);
      package->Load(newData);
      dependencies.push_back(package);
    }
  }
  catch (JSON::ParseException e) {
    return false;
  }

  return true;
}

template <typename FuncSig>
static bool CallFunction(Library &lib, const std::string &name, bool notFoundOk = false) {
  auto func = (FuncSig)lib.GetSymbol(name, notFoundOk);
  if (!func)
    return notFoundOk;
  return func();
}

/*
 * Converts a string like 'this-is-an-example-234' to 'ThisIsAnExample234'
 */
static std::string HyphenatedToPascalCasing(const std::string &input) {
  std::istringstream ss{input};
  std::string section, out;
  while (std::getline(ss, section, '-')) {
    if (section.empty())
      continue;

    section[0] = toupper(section[0]);
    out.append(section);
  }
  return out;
}

bool Package::Run(const RunOptions &options) {
  if (!playable || !usesCPP) {
    std::cout << "Package '" << name << "' can not be played\n";
    return false;
  }

  std::string libPath = path + "lib" + name + ".so";
  if (!options.quiet)
    std::clog << TermColor::FG_GREEN << "-- Running '" << name << '\'' << TermColor::FG_DEFAULT << '\n';
  bool ret = true;
  do {
    restartOptions = RestartOptions();
    Library lib;
    if (!lib.Open(libPath)) {
      std::cerr << "Unable to open library for the package\n";
      return false;
    }

    active = this;

    if (!CallFunction<bool (*)()>(lib, HyphenatedToPascalCasing(name) + "_Run", false)) {
      ret = false;
      if (restartOptions.errorMessage.empty())
        std::cerr << "Package '" << name << "' terminated with error\n";
      else
        std::cerr << "Package '" << name << "' terminated with error: '" << restartOptions.errorMessage << "'\n";
    }
    if (restartOptions.useNewArgs)
      Package::args = restartOptions.newArgs;

    lib.Close();
  } while (restartOptions.should);
  if (!options.quiet)
    std::clog << TermColor::FG_GREEN << "-- Finished Running '" << name << '\'' << TermColor::FG_DEFAULT << '\n';
  return ret;
}

static bool CompileTest(const Package *package, bool quiet) {
  if (!Directory::Exists("mods/" + package->name + "/tests"))
    return true;
  std::ostringstream command;
  command << "cd " << ::buildPath << "/mods/test/res; make -s MOD=\"" << package->name;
  command << "\" LIBS=\"";
  AddLibraryParameter(package->name, command, "../../", "../../");
  AddLibraryParameterRecursive(package->dependencies, command, "../../", "../../");

  command << "\" INCLUDE=\"";
  RecursiveInclude(package->dependencies, command, "../../");
  command << '"';

  if (quiet)
    command << " SILENT=\".\"";

  auto result = WEXITSTATUS(std::system(command.str().c_str()));
  return !result;
}

static bool RunTest(const std::string &packageName, bool quiet) {
  auto file = "mods/" + packageName + "/tests/tests";
  auto testsPresent = File::Exists(file);
  if (!testsPresent) {
    if (!quiet)
      std::cout << TermColor::FG_GREEN << "-- No Tests For '" << packageName << '\'' << TermColor::FG_DEFAULT << '\n';
    return true;
  }

  if (!quiet)
    std::clog << TermColor::FG_GREEN << "-- Running Tests for '" << packageName << '\'' << TermColor::FG_DEFAULT << '\n';

  auto result = File::Exists(file) ? WEXITSTATUS(std::system((::buildPath + file).c_str())) : 0;

  if (!quiet)
    std::clog << TermColor::FG_GREEN << "-- Finished Running Tests for '" << packageName << '\'' << TermColor::FG_DEFAULT << '\n';

  if (result) return false;
  return true;
}

bool Package::Test(const TestOptions &options) {
  if (options.compileTests) {
    if (!options.quiet)
      std::clog << TermColor::FG_GREEN << "-- Compiling Tests" << TermColor::FG_DEFAULT << '\n';

    auto printFinished = [&] {
      if (!options.quiet)
        std::clog << TermColor::FG_GREEN << "-- Finished Compiling Tests" << TermColor::FG_DEFAULT << '\n';
    };

    if (options.recursive) {
      // If we are recursive we can just iterate through all loaded packages
      for (auto &it : loaded) {
        if (!CompileTest(it.second.get(), options.quiet)) {
          printFinished();
          return false;
        }
      }
    }
    else {
      if (!CompileTest(this, options.quiet)) {
        printFinished();
        return false;
      }
    }

    printFinished();
  }

  if (options.recursive) {
    for (auto &it : loaded) {
      if (!RunTest(it.second->name, options.quiet))
        return false;
    }
  }
  else if (!RunTest(name, options.quiet))
    return false;

  return true;
}

void JSONImpl<Package::Data>::Read(Package::Data &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Package::Data"};
  const auto &object = JSON::GetObject(value, data);

  JSON::GetMember(out.author, "author", object, data);
  JSON::GetMember(out.version, "version", object, data);
  JSON::GetMember(out.playable, "playable", object, data);
  JSON::GetMember(out.hasHeaders, "has-headers", object, data);
  JSON::GetMember(out.usesCPP, "uses-c++", object, data);
  JSON::TryGetMember(out.compile, "compile", object, true, data);
  JSON::TryGetMember(out.dependencies, "depend", object, {}, data);

  JSON::ParseFailIf(
    out.playable && !out.usesCPP, data,
    "'playable' member is set to true and therefore the package must use C++");

  out.linkOptions = JSON::TryGetMember<std::string>("link-options", object, "", data);
}

void JSONImpl<Package::Data>::Write(const Package::Data &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
  JSON::WritePair("author", value.author, writer);
  JSON::WritePair("version", value.version, writer);
  JSON::WritePair("playable", value.playable, writer);
  JSON::WritePair("has-headers", value.hasHeaders, writer);
  JSON::WritePair("uses-c++", value.usesCPP, writer);
  JSON::WritePair("compile", value.compile, writer);
  JSON::WritePair("link-options", value.linkOptions, writer);
  JSON::WritePair("depend", value.dependencies, writer);
}

