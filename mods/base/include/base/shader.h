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

#ifndef _BASE__SHADER_H
#define _BASE__SHADER_H

#include <base/gl.h>
#include <cassert>
#include <math/mat.h>
#include <math/vec.h>
#include <unordered_map>

/*
 * Class for loading and using a GLSL shader
 */
class Shader {
  /*
   * OpenGL's ID for the shader
   */
  GLuint id = 0;

  /*
   * The current shader. May not be accurate if someone directly called
   * glUseProgram and not through this class
   */
  static Shader *current;

public:
  using Definitions = std::unordered_map<std::string, std::string>;

  struct Settings {
    std::string vertexFilePath, fragmentFilePath;
    Shader::Definitions definitions;
  };

  struct Uniform {
#ifdef NDEBUG
    Uniform() = default;
    Uniform(GLint loc) : location(loc) {}
#define SHADER_UNIFORM_SET_ASSERTS
#else
    Uniform() : valid(false) {}
    Uniform(GLint loc, const Shader *s)
        : valid(true), owner(s), location(loc) {}

#define SHADER_UNIFORM_SET_ASSERTS \
assert(valid);\
assert(owner == Shader::Current());
#endif // ifdef NDEBUG

    void Set(GLfloat v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform1f(location, v);
    }

    void Set(Vec2 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform2f(location, v.x, v.y);
    }

    void Set(Vec3 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform3f(location, v.x, v.y, v.z);
    }

    void Set(Vec4 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform4f(location, v.x, v.y, v.z, v.w);
    }

    void Set(GLint v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform1i(location, v);
    }

    void Set(IVec2 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform2i(location, v.x, v.y);
    }

    void Set(IVec3 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform3i(location, v.x, v.y, v.z);
    }

    void Set(IVec4 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform4i(location, v.x, v.y, v.z, v.w);
    }

    void Set(GLuint v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform1ui(location, v);
    }

    void Set(UVec2 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform2ui(location, v.x, v.y);
    }

    void Set(UVec3 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform3ui(location, v.x, v.y, v.z);
    }

    void Set(UVec4 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform4ui(location, v.x, v.y, v.z, v.w);
    }

    void Set1(GLsizei count, const GLfloat *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform1fv(location, count, v);
    }

    void Set2(GLsizei count, const GLfloat *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform2fv(location, count, v);
    }

    void Set3(GLsizei count, const GLfloat *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform3fv(location, count, v);
    }

    void Set4(GLsizei count, const GLfloat *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform4fv(location, count, v);
    }

    void Set1(GLsizei count, const GLint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform1iv(location, count, v);
    }

    void Set2(GLsizei count, const GLint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform2iv(location, count, v);
    }

    void Set3(GLsizei count, const GLint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform3iv(location, count, v);
    }

    void Set4(GLsizei count, const GLint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform4iv(location, count, v);
    }

    void Set1(GLsizei count, const GLuint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform1uiv(location, count, v);
    }

    void Set2(GLsizei count, const GLuint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform2uiv(location, count, v);
    }

    void Set3(GLsizei count, const GLuint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform3uiv(location, count, v);
    }

    void Set4(GLsizei count, const GLuint *v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniform4uiv(location, count, v);
    }

    void SetMatrix2(GLsizei count, GLboolean transpose, Mat2 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix2fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix3(GLsizei count, GLboolean transpose, Mat3 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix3fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix4(GLsizei count, GLboolean transpose, Mat4 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix4fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix2x3(GLsizei count, GLboolean transpose, Mat2x3 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix2x3fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix3x2(GLsizei count, GLboolean transpose, Mat3x2 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix3x2fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix2x4(GLsizei count, GLboolean transpose, Mat2x4 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix2x4fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix4x2(GLsizei count, GLboolean transpose, Mat4x2 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix4x2fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix3x4(GLsizei count, GLboolean transpose, Mat3x4 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix3x4fv(location, count, transpose, &v[0][0]);
    }

    void SetMatrix4x3(GLsizei count, GLboolean transpose, Mat4x3 v) {
      SHADER_UNIFORM_SET_ASSERTS
      glUniformMatrix4x3fv(location, count, transpose, &v[0][0]);
    }

  private:
#ifndef NDEBUG
    bool valid;
    const Shader *owner;
#endif
    GLint location;

    friend class Shader;
  };

  GLuint ID() const { return id; }

  Shader() {}
  ~Shader() {
    if (id) glDeleteProgram(id);
  }

  bool Load(const Settings &settings);

  /*
   * Gets a uniform variable from the shader
   * @param name The name of the variable
   * @returns a handle to the uniform
   */
  Uniform GetUniform(const std::string &name) {
    auto loc = glGetUniformLocation(id, name.c_str());
    // If the location is -1, the uniform name is not a uniform variable name in
    // the shader.
    // Structs must be set for each field, and arrays must have each
    // value set individually.
    assert(loc != -1);
#ifdef NDEBUG
    return loc;
#else
    return {loc, this};
#endif
  }

  /*
   * Makes this shader active
   */
  void Use() {
    current = this;
    glUseProgram(id);
  }

  bool IsCurrent() const { return current == this; }
  static Shader *Current() { return current; }
};

template <>
struct JSONImpl<Shader::Settings> {
  static void Read(Shader::Settings &out, const JSON::Value &value,
                   const JSON::ReadData &data);
  static void Write(const Shader::Settings &value, JSON::Writer &writer);
};

#endif // _BASE__SHADER_H
