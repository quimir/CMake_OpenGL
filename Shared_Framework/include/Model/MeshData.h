/*******************************************************************************
 * Copyright 2024 QuiMir
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_MESHDATA_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_MESHDATA_H_

#include <string>

#include "glm/glm.hpp"

namespace model {
namespace meshdata {
constexpr glm::int32 kMaxBoneInfluence = 4;

struct Vertex {
  // Position
  glm::vec3 position;
  // Normal
  glm::vec3 normal;
  // TexCoords
  glm::vec2 tex_coords;
  // Tangent
  glm::vec3 tangent;
  // Bitangent
  glm::vec3 bitangent;
  // Bone indexes which will influence this vertex
  glm::int32 bone_ids[kMaxBoneInfluence];
  // Weights from each bone
  glm::float32 weights[kMaxBoneInfluence];
};

struct Texture {
  // Texture ID in OpenGL
  glm::uint32 id;
  // Texture type
  std::string type;
  // Texture path
  std::string path;
};
}  // namespace meshdata
}  // namespace model

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_MESHDATA_H_
