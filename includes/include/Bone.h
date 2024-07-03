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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_BONE_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_BONE_H_

#include <vector>
#include <list>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "include/AssimpGLMHelpers.h"

class Bone {
 public:
  struct KeyPosition {
	glm::vec3 position;
	glm::float64 time_stamp;
  };

  struct KeyRotation {
	glm::quat orientation;
	glm::float64 time_stamp;
  };

  struct KeyScale {
	glm::vec3 scale;
	glm::float64 time_stamp;
  };

 public:
  Bone(std::string name, int id, const aiNodeAnim *channel);

  void Update(glm::float64 animation_time);

  const glm::mat4 &GetLocalTransform() const;

  void SetLocalTransform(const glm::mat4 &local_transform);

  const std::string &GetName() const;

  void SetName(const std::string &name);

  glm::int32 GetId() const;

  void SetId(glm::int32 id);

  glm::int32 GetPositionsIndex(glm::float64 animation_time);

  glm::int32 GetRotationIndex(glm::float64 animation_time);

  glm::int32 GetScaleIndex(glm::float64 animation_time);

 private:
  glm::float64 GetScaleFactor(glm::float64 last_time_stamp,
							  glm::float64 next_time_stamp,
							  glm::float64 animation_time);

  glm::mat4 InterpolatePosition(glm::float64 animation_time);

  glm::mat4 InterpolateRotation(glm::float64 animation_time);

  glm::mat4 InterpolateScale(glm::float64 animation_time);

 private:
  std::vector<KeyPosition> positions_;
  std::vector<KeyRotation> rotations_;
  std::vector<KeyScale> scales_;

  glm::uint32 num_positions_;
  glm::uint32 num_rotations_;
  glm::uint32 num_scales_;

  glm::mat4 local_transform_;
  std::string name_;
  glm::int32 id_;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_BONE_H_
