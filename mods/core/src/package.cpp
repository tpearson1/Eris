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
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <statics.h>
#include <library.h>
#include <file.h>
#include <termcolor.h>
#include <saveload.h>

std::vector<Package *> Package::all;
Package *Package::active = nullptr;

bool Package::ReadPackageJSON(bool compileDependencies, bool quiet) {
  std::string jsonPath = path + "package.json";

  std::string jsonText;
  File::Read(jsonPath, jsonText);

  rapidjson::Document json;
  json.Parse<JSON_FLAGS>(jsonText.c_str());

  // Error checking
  CHECK(json.IsObject(), "There is no root object")

  CHECK(json.HasMember("author"), "'author' member must be present")
  CHECK(json["author"].IsString(), "'author' member must be of type string")

  CHECK(json.HasMember("version"), "'version' member must be present")
  CHECK(json["version"].IsString(), "'version' member must be of type string")

  CHECK(json.HasMember("playable"), "'playable' member must be present")
  CHECK(json["playable"].IsBool(), "'playable' member must be of type bool")

  CHECK(json.HasMember("uses-c++"), "'uses-c++' member must be present")
  CHECK(json["uses-c++"].IsBool(), "'uses-c++' member must be of type bool")

  if (json.HasMember("header-only")) {
    CHECK(json["uses-c++"] == true, "'uses-c++' member must be set to true if the package is header only")
    headerOnly = true;
  }

  usesCPP = json["uses-c++"].GetBool();
  playable = json["playable"].GetBool();

  PARSE_FAIL_IF(!usesCPP && playable, "'playable' member is set to true and therefore the package must use C++")

  author = json["author"].GetString();
  version = json["version"].GetString();

  if (json.HasMember("link-options")) {
    const rapidjson::Value &linkOpts = json["link-options"];
    CHECK(linkOpts.IsString(), "'link-options' member must be of type string")
    linkOptions = linkOpts.GetString();
  }

  if (json.HasMember("depend")) {
    const rapidjson::Value &depend = json["depend"];
    CHECK(depend.IsArray(), "'depend' member must be of type array")

    dependencies.reserve(depend.Size());
    for (rapidjson::SizeType i = 0; i < depend.Size(); i++) {
      CHECK(depend[i].IsString(), "All members of 'depend' array must be of type string")

      std::string dep = depend[i].GetString();
      CHECK(Directory::Exists("mods/" + dep), "Dependency '" + dep + "' is unsatisfied")

      Package *pkg = new Package(dep);
      if (pkg->Load(compileDependencies, quiet)) // No package will ever be loaded twice
        dependencies.push_back(pkg);
      else {
        for (size_t i = 0; i < all.size(); i++) {
          if (all[i]->name == dep) {
            dependencies.push_back(all[i]);
            delete pkg;
            break;
          }
        }
      }
    }
  }

  return true;
}

void Package::RecursiveInclude(const std::vector<Package *> &deps, std::vector<Package *> &processed, std::stringstream &out) const {
  for (auto &elem : deps) {
    if (std::find(std::begin(processed), std::end(processed), elem) != std::end(processed))
      continue;

    if (elem->usesCPP) {
      out << " -I" << elem->name << "/include";
      processed.push_back(elem);
    }
    RecursiveInclude(elem->dependencies, processed, out);
  }
}

int Package::Compile(bool quiet) const {
  if (name == "core")
    return 0; // The 'core' package is compiled by the game's makefile
  if (headerOnly)
    return 0;
  if (usesCPP) {
    std::stringstream command;
    command << "cd mods;make " << ((quiet) ? "-s " : "") << "NAME=\"" << name << "\"";
    command << " LIBS=\"-shared " << linkOptions;
    for (size_t i = 0; i < dependencies.size(); i++) {
      if (!dependencies[i]->usesCPP)
        continue;

      command << " -Wl,";
      command << "-rpath=\\\\$\\$\\$\\ORIGIN/../../" << dependencies[i]->name << "/lib"
                 " -L" << dependencies[i]->name << "/lib -l" << dependencies[i]->name;
    }

    command << "\" OPTFLAGS=\"-fpic -I" << name << "/include/" << name
            << " -I" << name << "/include"
            " -I" << name << "/src/include";
    std::vector<Package *> processed;
    RecursiveInclude(dependencies, processed, command);

    command << "\" TARGET=\"" << name << "/lib/lib" << name << ".so\"";

    int result = std::system(command.str().c_str());
    if (result)
      std::cerr << "> Package " << name << " failed to build\n";
    return result;
  }
  return 0;
}

/*
 * Converts a string like 'this-is-an-example-234' to 'ThisIsAnExample234'
 */
std::string HyphenatedToPascalCasing(const std::string &input) {
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

bool CallFuncReturningBool(Library &lib, const std::string &name, bool notFoundOk = false) {
  auto func = (bool (*)())lib.GetSymbol(name, notFoundOk);
  if (!func)
    return notFoundOk;
  return func();
}

bool CallFuncOnDependenciesPrefixed(const std::vector<Package *> &deps, std::vector<Package *> &processed, Library &lib, const std::string &funcName) {
  for (auto &elem : deps) {
    if (std::find(std::begin(processed), std::end(processed), elem) != std::end(processed))
      continue;
    
    if (!CallFuncOnDependenciesPrefixed(elem->Dependencies(), processed, lib, funcName))
      return false;
    auto fullName = HyphenatedToPascalCasing(elem->Name()) + funcName;
    processed.push_back(elem);
    if (!CallFuncReturningBool(lib, fullName, true)) {
      std::cerr << "> Function '" << fullName << "' returned false\n";
      return false;
    }
  }
  return true;
}

bool Package::Load(bool compile, bool quiet) {
  for (size_t i = 0; i < all.size(); i++)
    if (all[i]->name == name)
      return false;

  path = "mods/" + name + "/";
  if (!ReadPackageJSON(compile, quiet))
  return false;
  if (!quiet)
    std::clog << TermColor::FG_BLUE << "---- LOADING PACKAGE " << name << "@v" << version << " by " << author << " ----" << TermColor::FG_DEFAULT << '\n';

  if (compile && Compile(quiet)) // Only compile if we should
    exit(1);

  all.push_back(this);
  return true;
}

bool Package::Start(bool quiet, bool runTests) {
  if (!playable || !usesCPP)
    return false;
  std::string libPath = path + "lib/lib" + name + ".so";
  if (!quiet)
    std::clog << TermColor::FG_GREEN << "-- RUNNING PACKAGE " << name << " --" << TermColor::FG_DEFAULT << '\n';
  int ret;
  do {
    restartOptions = RestartOptions();
    Library lib;
    if (!lib.Open(libPath))
      return false;

    active = this;
    std::vector<Package *> temp; 
    if (!CallFuncOnDependenciesPrefixed({this}, temp, lib, "_Initialize"))
      return false;

    temp.clear();

    if (runTests) {
      if (CallFuncOnDependenciesPrefixed({this}, temp, lib, "_RunTests"))
        std::cout << "Testing successful\n";
      else if (!quiet)
        return false;
    }
    else {
      if (!CallFuncReturningBool(lib, HyphenatedToPascalCasing(name) + "_Run", false)) {
        ret = false;
        if (restartOptions.errorMessage.empty())
          std::cerr << "> Package '" << name << "' terminated with error\n";
        else
          std::cerr << "> Package '" << name << "' terminated with error: '" << restartOptions.errorMessage << "'\n";
      }
      if (restartOptions.useNewArgs)
        Package::args = restartOptions.newArgs;
    }

    lib.Close();
  } while (restartOptions.should);
  if (!quiet)
    std::clog << TermColor::FG_GREEN << "-- FINISHED RUNNING PACKAGE " << name << " --" << TermColor::FG_DEFAULT << '\n';
  return ret;
}

void Package::Cleanup() {
  for (size_t i = 0; i < all.size(); i++)
    delete all[i];
  all.clear();
}
