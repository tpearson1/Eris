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

#include <core/mapping.h>
#include <math/vec.h>
#include <math/mat.h>
#include <base/gl.h>

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
  static const Shader *current;

public:
  using Definitions = SerializableMapping<std::string>;

  struct Settings : public SaveLoad {
    std::string vertexFilePath, fragmentFilePath;
    Shader::Definitions definitions; 

    virtual void SerializeToJSON(Save::Writer &writer) const override;

    virtual bool LoadFromJSON(const rapidjson::Value &data, JSONTypeManager &manager) override;
  };

  static std::string openGLVersion;

  GLuint ID() const { return id; }

  Shader() {}
  ~Shader()
    { if (id) glDeleteProgram(id); }

  bool Load(const Settings &settings);

  /*
   * Gets a uniform variable from the shader
   * @param name The name of the variable
   * @returns a handle to the uniform
   */
  GLint GetUniform(const std::string &name) const { return glGetUniformLocation(id, name.c_str()); }

  /*
   * Makes this shader active
   */
  void Use() const { current = this; glUseProgram(id); }

  bool IsCurrent() const { return current == this; }
  static const Shader &Current() { return *current; }

  /*
   * Sets uniform variables in the shader
   */
  static void SetUniform(GLint location, GLfloat v)
    { glUniform1f(location, v); }
  static void SetUniform(GLint location, Vec2 v)
    { glUniform2f(location, v.x, v.y); }
  static void SetUniform(GLint location, Vec3 v)
    { glUniform3f(location, v.x, v.y, v.z); }
  static void SetUniform(GLint location, Vec4 v)
    { glUniform4f(location, v.x, v.y, v.z, v.w); }
  static void SetUniform(GLint location, GLint v)
    { glUniform1i(location, v); }
  static void SetUniform(GLint location, IVec2 v)
    { glUniform2i(location, v.x, v.y); }
  static void SetUniform(GLint location, IVec3 v)
    { glUniform3i(location, v.x, v.y, v.z); }
  static void SetUniform(GLint location, IVec4 v)
    { glUniform4i(location, v.x, v.y, v.z, v.w); }
  static void SetUniform(GLint location, GLuint v)
    { glUniform1ui(location, v); }
  static void SetUniform(GLint location, UVec2 v)
    { glUniform2ui(location, v.x, v.y); }
  static void SetUniform(GLint location, UVec3 v)
    { glUniform3ui(location, v.x, v.y, v.z); }
  static void SetUniform(GLint location, UVec4 v)
    { glUniform4ui(location, v.x, v.y, v.z, v.w); }
  static void SetUniform1(GLint location, GLsizei count, const GLfloat *v)
    { glUniform1fv(location, count, v); }
  static void SetUniform2(GLint location, GLsizei count, const GLfloat *v)
    { glUniform2fv(location, count, v); }
  static void SetUniform3(GLint location, GLsizei count, const GLfloat *v)
    { glUniform3fv(location, count, v); }
  static void SetUniform4(GLint location, GLsizei count, const GLfloat *v)
    { glUniform4fv(location, count, v); }
  static void SetUniform1(GLint location, GLsizei count, const GLint *v)
    { glUniform1iv(location, count, v); }
  static void SetUniform2(GLint location, GLsizei count, const GLint *v)
    { glUniform2iv(location, count, v); }
  static void SetUniform3(GLint location, GLsizei count, const GLint *v)
    { glUniform3iv(location, count, v); }
  static void SetUniform4(GLint location, GLsizei count, const GLint *v)
    { glUniform4iv(location, count, v); }
  static void SetUniform1(GLint location, GLsizei count, const GLuint *v)
    { glUniform1uiv(location, count, v); }
  static void SetUniform2(GLint location, GLsizei count, const GLuint *v)
    { glUniform2uiv(location, count, v); }
  static void SetUniform3(GLint location, GLsizei count, const GLuint *v)
    { glUniform3uiv(location, count, v); }
  static void SetUniform4(GLint location, GLsizei count, const GLuint *v)
    { glUniform4uiv(location, count, v); }
  static void SetUniformMatrix2(GLint location, GLsizei count, GLboolean transpose, Mat2 v)
    { glUniformMatrix2fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix3(GLint location, GLsizei count, GLboolean transpose, Mat3 v)
    { glUniformMatrix3fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, Mat4 v)
    { glUniformMatrix4fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix2x3(GLint location, GLsizei count, GLboolean transpose, Mat2x3 v)
    { glUniformMatrix2x3fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix3x2(GLint location, GLsizei count, GLboolean transpose, Mat3x2 v)
    { glUniformMatrix3x2fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix2x4(GLint location, GLsizei count, GLboolean transpose, Mat2x4 v)
    { glUniformMatrix2x4fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix4x2(GLint location, GLsizei count, GLboolean transpose, Mat4x2 v)
    { glUniformMatrix4x2fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix3x4(GLint location, GLsizei count, GLboolean transpose, Mat3x4 v)
    { glUniformMatrix3x4fv(location, count, transpose, &v[0][0]); }
  static void SetUniformMatrix4x3(GLint location, GLsizei count, GLboolean transpose, Mat4x3 v)
    { glUniformMatrix4x3fv(location, count, transpose, &v[0][0]); }
};

#endif // _BASE__SHADER_H
