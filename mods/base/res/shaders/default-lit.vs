#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

out vec3 fragPos
out vec2 UV;
out vec3 normal;
uniform mat4 MVP;
uniform mat4 M;

void main() {
  gl_Position = MVP * vec4(position, 1);
  fragPos = vec3(M * vec4(position, 1));
  UV = vertexUV;
  normal = mat3(transpose(inverse(model))) * normal;
}
