#version 330 

out vec4 frag_color;

in VertexOut {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  vec4 frag_pos_light_space;
} fs_in;

uniform sampler2D diffuse_texture;
uniform sampler2D shadow_map;

uniform vec3 light_pos;
uniform vec3 view_pos;

uniform float max_light_distance;
uniform float min_radius;
uniform float max_radius;

uniform float bias_value;

float ShadowCalculation(vec4 frag_pos_light_space) {
  // Perform perspective divide
  vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
  proj_coords = proj_coords * 0.5 + 0.5;

  float current_depth = proj_coords.z;

  // Beyond light range should not render shadow
  if (current_depth > 1.0)
  return 0.0f;

  // Calculate bias to avoid shadow acne
  float bias = max(bias_value * (1.0 - dot(normalize(fs_in.normal), normalize(light_pos - fs_in.frag_pos))), bias_value / 10);

  // Calculate the distance from fragment to light
  float distance_to_light = length(light_pos - fs_in.frag_pos);

  // Adaptive sampling radius based on distance to light
  float adaptive_radius = mix(min_radius, max_radius, distance_to_light / max_light_distance);
  vec2 texel_size = adaptive_radius / vec2(textureSize(shadow_map, 0));

  // PCF shadow calculation with depth threshold, directional weight, and adaptive radius
  float shadow = 0.0;
  float total_weight = 0.0;

  for (int x = -1; x <= 1; ++x) {
	for (int y = -1; y <= 1; ++y) {
	  vec2 offset = vec2(x, y) * texel_size;
	  float PCF_depth = texture(shadow_map, proj_coords.xy + offset).r;

	  // Calculate the depth difference and apply a threshold to avoid shadowing bright areas
	  float depth_diff = current_depth - bias - PCF_depth;
	  if (depth_diff > 0.005) { // Depth difference threshold
								// Weight based on light direction to prevent shadowing areas directly lit by light
								float weight = max(0.0, dot(normalize(offset), normalize(light_pos.xy)));
								shadow += weight;
								total_weight += weight;
	  }
	}
  }

  // Normalize shadow factor by total weights
  if (total_weight > 0.0) {
	shadow /= total_weight;
  }

  return shadow;
}

void main() {
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
  float shadow = ShadowCalculation(fs_in.frag_pos_light_space);
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

  frag_color = vec4(lighting, 1.0);
}