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

#include <shader.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <core/file.h>
#include <core/statics.h>

const Shader *Shader::current;

void JSONImpl<Shader::Settings>::Write(const Shader::Settings &value, JSON::Writer &writer) {
  auto obj = JSON::ObjectEncloser{writer};
  JSON::WritePair("vertex", value.vertexFilePath, writer);
  JSON::WritePair("vertex", value.fragmentFilePath, writer);
  JSON::WritePair("definitions", value.definitions, writer);
}

void JSONImpl<Shader::Settings>::Read(Shader::Settings &out, const JSON::Value &value, const JSON::ReadData &data) {
  auto t = Trace::Pusher{data.trace, "Shader::Settings"};
  const auto &object = JSON::GetObject(value, data);

  JSON::GetMember(out.vertexFilePath, "vertex", object, data);
  JSON::GetMember(out.fragmentFilePath, "fragment", object, data);
  JSON::TryGetMember(out.definitions, "definitions", object, {}, data);
}

static void CompileShader(GLuint shaderID, const std::string &source) {
  GLint result = GL_FALSE;
  int infoLogLength;

  // Compile the shader
  const char *c_str = source.c_str();
  glShaderSource(shaderID, 1, &c_str, NULL);
  glCompileShader(shaderID);

  // Check compilation
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
  if (infoLogLength > 0) {
    std::vector<char> errorMessage(infoLogLength + 1);
    glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
    if (errorMessage[0])
      std::cerr << "> " << &errorMessage[0] << "|\n";
  }
}

bool Shader::Load(const Settings &settings) {
  if (id)
    glDeleteProgram(id);

  // Create the shaders
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the shader code from the files
  std::string vertexShaderCode, fragmentShaderCode;
  File::Read(settings.vertexFilePath, vertexShaderCode);
  File::Read(settings.fragmentFilePath, fragmentShaderCode);
  if (vertexShaderCode.empty() || fragmentShaderCode.empty()) {
    std::cerr << "Unable to load shader files\n";
    return false;
  }

  auto versionText = std::string("#version ") + GL::shaderVersion + '\n';

  std::string defs;
  for (auto &definition : settings.definitions)
    defs += "#define " + definition.first + ' ' + definition.second + '\n';

  vertexShaderCode = versionText + defs + vertexShaderCode;
  fragmentShaderCode = versionText + defs + fragmentShaderCode;

  // Compile the shaders
  CompileShader(vertexShaderID, vertexShaderCode);
  CompileShader(fragmentShaderID, fragmentShaderCode);

  // Link the program
  id = glCreateProgram();
  glAttachShader(id, vertexShaderID);
  glAttachShader(id, fragmentShaderID);
  glLinkProgram(id);

  // Check the program
  GLint result = GL_FALSE;
  int infoLogLength;
  glGetProgramiv(id, GL_LINK_STATUS, &result);
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
  if (infoLogLength > 0) {
    std::vector<char> programErrorMessage(infoLogLength + 1);
    glGetProgramInfoLog(id, infoLogLength, NULL, &programErrorMessage[0]);
    if (programErrorMessage[0])
      std::cerr << "> " << &programErrorMessage[0] << "|\n";
  }

  glDetachShader(id, vertexShaderID);
  glDetachShader(id, vertexShaderID);

  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

  return true;
}

