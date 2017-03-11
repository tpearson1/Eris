#version 330 core

struct Props {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirLight {
  vec3 direction;
  Props p;
};

struct PointLight {
  vec3 location;

  float constant;
  float linear;
  float quadratic;

  Props p;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float innerAngle;
  float outerAngle;

  float constant;
  float linear;
  float quadratic;

  Props p;
};

uniform int numPointLights;
uniform PointLight pointLights;

in vec2 UV;
in vec3 normal;
uniform sampler2D textureSampler;

void main() {
  vec3 norm = normalize(normal);
}
