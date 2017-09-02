// Constants MAX_DIR_LIGHTS and MAX_POINT_LIGHTS must be defined

out vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec2 UV;

#if MAX_DIR_LIGHTS != 0
struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
#endif

#if MAX_POINT_LIGHTS != 0
struct PointLight {
  vec3 location;

  vec3 ambient;
  vec3 specular;
  vec3 diffuse;

  float constant;
  float linear;
  float quadratic;
};
#endif

struct Material {
  sampler2D diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;

uniform vec3 cameraLocation;
#if MAX_DIR_LIGHTS != 0
  uniform DirectionalLight[MAX_DIR_LIGHTS] directionalLights;
#endif
#if MAX_POINT_LIGHTS != 0
  uniform PointLight[MAX_POINT_LIGHTS] pointLights;
#endif

uniform int numDirectionalLights, numPointLights;

#if MAX_DIR_LIGHTS != 0
  vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
#endif
#if MAX_POINT_LIGHTS != 0
  vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir);
#endif

void main() {
  vec3 norm = normalize(normal);
  vec3 viewDir = normalize(cameraLocation - fragPos);

  vec3 result = vec3(0.0f, 0.0f, 0.0f);
#if MAX_DIR_LIGHTS != 0
  for (int i = 0; i < numDirectionalLights; i++)
    result += CalculateDirectionalLight(directionalLights[i], norm, viewDir);
#endif
#if MAX_POINT_LIGHTS != 0
  for (int i = 0; i < numPointLights; i++)
      result += CalculatePointLight(pointLights[i], norm, fragPos, viewDir);
#endif

  color = vec4(result, 1.0f);
}

#if MAX_DIR_LIGHTS != 0
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0f);
  // Specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 color = vec3(texture(material.diffuse, UV));
  vec3 ambient = light.ambient * color;
  vec3 diffuse = light.diffuse * diff * color;
  vec3 specular = light.specular * spec;
  return ambient + diffuse + specular;
}
#endif

#if MAX_POINT_LIGHTS != 0
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir) {
  vec3 lightDir = normalize(light.location - pos);
  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0f);
  // Specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  // Attenuation
  float distance = length(light.location - pos);
  float attenuation = 1.0f / (light.constant + light.linear * distance
                              + light.quadratic * (distance * distance));

  vec3 color = vec3(texture(material.diffuse, UV));
  vec3 ambient = light.ambient * color;
  vec3 diffuse = light.diffuse * diff * color;
  vec3 specular = light.specular * spec;

  return (ambient + diffuse + specular) * attenuation;
}
#endif

