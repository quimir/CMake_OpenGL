#version 420
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out VertexOut {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
}vs_out;

out vec2 tex_coords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform bool reverse_normals;

void main()
{
  vs_out.frag_pos = vec3(model * vec4(a_pos, 1.0f));
  if (reverse_normals)
  {
	vs_out.normal = transpose(inverse(mat3(model))) * (-1.0f * a_normal);
  }
  else
  {
	vs_out.normal = transpose(inverse(mat3(model))) * a_normal;
  }
  vs_out.tex_coords = a_tex_coords;
  gl_Position = projection * view * model * vec4(a_pos, 1.0);
}