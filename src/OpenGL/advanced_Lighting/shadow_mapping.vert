#version 330
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out VertexOut {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  vec4 frag_pos_light_space;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 light_space_matrix;
uniform mat4 model;

void main() {
  vs_out.frag_pos = vec3(model * vec4(a_pos, 1.0));
  vs_out.normal = transpose(inverse(mat3(model))) * a_normal;
  vs_out.tex_coords = a_tex_coords;
  vs_out.frag_pos_light_space = light_space_matrix * vec4(vs_out.frag_pos, 1.0);
  gl_Position = projection * view * vec4(vs_out.frag_pos, 1.0);
}