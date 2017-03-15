in vec2 UV;
out vec3 color;

struct Material {
  sampler2D diffuse;
};

uniform Material material;

void main() {
  color = texture(material.diffuse, UV).rgb;
}
