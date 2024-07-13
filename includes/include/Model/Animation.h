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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_ANIMATION_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_ANIMATION_H_

#include <vector>
#include <map>
#include <functional>
#include <string>

#include "glm/glm.hpp"
#include "assimp/scene.h"

#include "Bone.h"
#include "BoneInfo.h"
#include "Model.h"

class Animation {
 public:
  struct AssimpNodeData {
	glm::mat4 transformation;
	std::string name;
	glm::uint32 children_count;
	std::vector<AssimpNodeData> children;
  };

  Animation(const std::string &animation_path, Model *model);

  ~Animation() = default;

  Bone *FindBone(const std::string &name);

  glm::float64 GetDuration() const;

  void SetDuration(glm::float64 duration);

  glm::float64 GetTicksPerSecond() const;

  void SetTicksPerSecond(glm::float64 ticks_per_second);

  const AssimpNodeData &GetRootNode() const;

  void SetRootNode(const AssimpNodeData &root_node);

  const std::map<std::string, BoneInfo> &GetBoneInfoMap() const;

  void SetBoneInfoMap(const std::map<std::string, BoneInfo> &bone_info_map);

 private:
  Animation() = default;

  void ReadMissingBones(const aiAnimation *animation, Model *model);

  void ReadHierarchyData(AssimpNodeData &dest, const aiNode *src);

 private:
  glm::float64 duration_;
  glm::float64 ticks_per_second_;
  std::vector<Bone> bones_;
  AssimpNodeData root_node_;
  std::map<std::string, BoneInfo> bone_info_map_;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_ANIMATION_H_
