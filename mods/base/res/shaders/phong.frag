#version 330 core

#define NUM_LIGHTS 12

out vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec2 UV;

struct PointLight {
  vec3 position;
  vec3 color;
};

uniform vec3 cameraPosition;
uniform PointLight[NUM_LIGHTS] pointLights;
uniform sampler2D textureSampler;

void main() {
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * pointLights[0].color;

  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(pointLights[0].position - fragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * pointLights[0].color;

  float specularStrength = 0.5f;
  vec3 cameraDir = normalize(cameraPosition - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(cameraPosition, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * pointLights[0].color;

  vec3 result = (ambient + diffuse + specular) * texture(textureSampler, UV).rgb; 
  color = vec4(result, 1.0f);
}
