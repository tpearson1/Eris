#version 330 core

#define NUM_LIGHTS 12

out vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec2 UV;

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 specular;
  vec3 diffuse;

  float linear;
  float quadratic;
};

uniform vec3 cameraPosition;
uniform PointLight[NUM_LIGHTS] pointLights;
uniform sampler2D textureSampler;
uniform float shininess;

void main() {
  vec3 tex = vec3(texture(textureSampler, UV));

  vec3 ambient = pointLights[0].ambient * tex;

  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(pointLights[0].position - fragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = pointLights[0].diffuse * diff * tex;

  vec3 cameraDir = normalize(cameraPosition - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(cameraPosition, reflectDir), 0.0), shininess);
  vec3 specular = pointLights[0].specular * spec;

  float distance = length(pointLights[0].position - fragPos);
  float attenuation = 1.0f / (1.0f + pointLights[0].linear * distance +
                      pointLights[0].quadratic * (distance * distance));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  color = vec4(ambient + diffuse + specular, 1.0f);
}
