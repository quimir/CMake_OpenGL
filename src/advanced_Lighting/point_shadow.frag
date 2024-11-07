#version 420
out vec4 frag_color;

in VertexOut {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
} fs_in;

uniform sampler2D diffuse_texture;
uniform samplerCube depth_map;

uniform vec3 light_pos;
uniform vec3 view_pos;

uniform float bias_value;

uniform float far_plane;
uniform bool open_shadow;

float ShadowCalculation(vec3 frag_pos)
{
  vec3 frag_to_light = frag_pos - light_pos;
  
  float current_depth = length(frag_to_light);

  // Calculate bias to avoid shadow acne
  float bias = max(bias_value * (1.0 - dot(normalize(fs_in.normal), normalize(light_pos - fs_in.frag_pos))), bias_value / 10);

  float shadow = 0.0;
  float samples = 20;
  float offset = 0.1;
  float view_distance = length(view_pos - frag_pos);
  float disk_radius = (1.0 + (view_distance / far_plane)) / 25.0;

  vec3 sample_offset_directions[20] = vec3[]
  (
  vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
  vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
  vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
  vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
  vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
  );

  for (int i = 0; i < samples; ++i) {
	float closest_depth = texture(depth_map, frag_to_light + sample_offset_directions[i] * disk_radius).r;
	closest_depth *= far_plane;
	if (current_depth - bias > closest_depth)
	{
	  shadow += 1.0;
	}
  }

  shadow /= float(samples);

  return shadow;
}

void main()
{
  vec3 color = texture(diffuse_texture, fs_in.tex_coords).rgb;
  vec3 normal = normalize(fs_in.normal);
  vec3 light_color = vec3(1.0);
  // ambient
  vec3 ambient = 0.15 * light_color;
  // diffuse
  vec3 light_dir = normalize(light_pos - fs_in.frag_pos);
  float diff = max(dot(light_dir, normal), 0.0);
  vec3 diffuse = diff * light_color;
  // specular
  vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
  vec3 halfway_dir = normalize(light_dir + view_dir);
  float spec = 0.0;
  spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
  vec3 specular = spec * light_color;

  // calculate shadow
  float shadow = open_shadow ? ShadowCalculation(fs_in.frag_pos) : 0.0;
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

  frag_color = vec4(lighting, 1.0);
}