#include <buffer.h>

void ElementBuffer::Generate() {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint),
               data.data(), GL_STATIC_DRAW);
}
