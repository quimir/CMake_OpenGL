#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textures;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in ivec4 bone_ids;
layout (location = 6) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int kMaxBones = 100;
const int kMaxBoneInfluence = 4;
uniform mat4 final_bones_matrices[kMaxBones];

out vec2 tex_coords;

void main() {
  vec4 total_position = vec4(0.0f);
  for (int i = 0; i < kMaxBoneInfluence; i++)
  {
	if (bone_ids[i] == -1)
	continue;
	if (bone_ids[i] >= kMaxBones)
	{
	  total_position = vec4(position, 1.0f);
	  break;
	}

	vec4 local_position = final_bones_matrices[bone_ids[i]] * vec4(position, 1.0f);
	total_position += local_position * weights[i];
	vec3 local_normal = mat3(final_bones_matrices[bone_ids[i]]) * normal;
  }

  mat4 view_model = view * model;
  gl_Position = projection * view_model * total_position;
  tex_coords = textures;
}
