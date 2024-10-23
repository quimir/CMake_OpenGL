#version 330

in VertexOut {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
} fs_in;

out vec4 frag_color;

uniform sampler2D floor_texture;

uniform vec3 light_positions[4];
uniform vec3 light_colors[4];
uniform vec3 view_pos;
// illumination intensity
uniform int light_value;

uniform bool gamma;
uniform float gamma_value;

vec3 BlinnPhong(vec3 normal, vec3 frag_pos, vec3 light_pos, vec3 light_color)
{
  vec3 light_dir = normalize(light_pos - frag_pos);
  float diff = max(dot(light_dir, normal),0.0);
  vec3 diffuse = diff * light_color;

  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = 0.0f;
  vec3 halfway_dir = normalize(light_dir + view_dir);
  spec = pow(max(dot(normal, halfway_dir), 0.0), light_value);
  vec3 specular = spec * light_color;

  float max_distance = 1.5;
  float distance = length(light_pos - frag_pos);
  float attenuation = 1.0 / (gamma ? distance * distance : distance);

  diffuse *= attenuation;
  specular *= attenuation;

  return diffuse + specular;
}

void main() {
  vec3 color = texture(floor_texture, fs_in.tex_coords).rgb;
  vec3 lighting = vec3(0.0);
  for (int i = 0; i < 4; i++) {
	lighting += BlinnPhong(normalize(fs_in.normal), fs_in.frag_pos, light_positions[i], light_colors[i]);
  }
  color*=lighting;
  if(gamma)
  {
    color=pow(color,vec3(1.0/gamma_value));
  }
  frag_color=vec4(color,1.0);
}