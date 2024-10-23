#version 330
out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D depth_map;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
  float z = depth * 2.0 - 1.0; // back to NDC
  return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
  float depth_value=texture(depth_map,tex_coords).r;
  frag_color=vec4(vec3(LinearizeDepth(depth_value)/far_plane),1.0);
}