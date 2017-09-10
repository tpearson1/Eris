layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 3) in mat4 offset;

out vec2 UV;
uniform mat4 VP;

void main() {
  gl_Position = VP * offset * vec4(position, 1);
  UV = vertexUV;
}
