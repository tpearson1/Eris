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

#ifndef _RENDER__FRAMEBUFFER_H
#define _RENDER__FRAMEBUFFER_H

#include <base/gl.h>
#include <base/texture.h>

enum class FramebufferTarget : GLenum {
  BOTH = GL_FRAMEBUFFER,
  READ = GL_READ_FRAMEBUFFER,
  WRITE = GL_DRAW_FRAMEBUFFER
};

class Renderbuffer {
  GLuint id = 0;
public:
  GLuint ID() const { return id; }

  void Generate()
    { glGenRenderbuffers(1, &id); }

  ~Renderbuffer()
    { if (id) glDeleteRenderbuffers(1, &id); }

  void Bind()
    { glBindRenderbuffer(GL_RENDERBUFFER, id); }

  void Initialize(GLenum internalFormat, int width = Window::GetActive()->width, int height = Window::GetActive()->height)
    { glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height); }
};

class Framebuffer {
  GLuint id = 0;
public:
  GLuint ID() const { return id; }

  void Generate()
    { glGenFramebuffers(1, &id); }

  ~Framebuffer()
    { if (id) glDeleteFramebuffers(1, &id); }

  void Bind(FramebufferTarget target = FramebufferTarget::BOTH) const
    { glBindFramebuffer((GLenum)target, id); }

  void AttachTexture(const Texture &tex, GLenum type, GLint mipmapLevel = 0, FramebufferTarget target = FramebufferTarget::BOTH)
    { glFramebufferTexture2D((GLenum)target, type, (GLenum)tex.Settings().type, tex.ID(), mipmapLevel); }

  void AttachRenderbuffer(const Renderbuffer &buffer, FramebufferTarget target = FramebufferTarget::BOTH)
    { glFramebufferRenderbuffer((GLenum)target, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer.ID()); }

  void BindDefault()
    { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  bool Complete(FramebufferTarget target = FramebufferTarget::BOTH) const
    { return glCheckFramebufferStatus((GLenum)target) == GL_FRAMEBUFFER_COMPLETE; }
};

#endif // _RENDER__FRAMEBUFFER_H
