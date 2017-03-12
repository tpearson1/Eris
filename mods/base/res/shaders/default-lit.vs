layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

out vec3 fragPos;
out vec2 UV;
out vec3 normal;
uniform mat4 MVP;
uniform mat4 model;

void main() {
  gl_Position = MVP * vec4(position, 1.0f);
  fragPos = vec3(model * vec4(position, 1.0f));
  UV = vertexUV;
  normal = mat3(transpose(inverse(model))) * vertexNormal;
}
