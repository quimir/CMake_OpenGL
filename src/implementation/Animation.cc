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

#include <stdexcept>
#include "include/LoggerSystem.h"

#include "include/Animation.h"
glm::float64 Animation::GetDuration() const {
  return this->duration_;
}
void Animation::SetDuration(glm::float64 duration) {
  duration_ = duration;
}
glm::float64 Animation::GetTicksPerSecond() const {
  return ticks_per_second_;
}
void Animation::SetTicksPerSecond(glm::float64 ticks_per_second) {
  ticks_per_second_ = ticks_per_second;
}
const Animation::AssimpNodeData &Animation::GetRootNode() const {
  return root_node_;
}
void Animation::SetRootNode(const Animation::AssimpNodeData &root_node) {
  root_node_ = root_node;
}
const std::map<std::string, BoneInfo> &Animation::GetBoneInfoMap() const {
  return bone_info_map_;
}
void Animation::SetBoneInfoMap(const std::map<std::string, BoneInfo> &bone_info_map) {
  bone_info_map_ = bone_info_map;
}
void Animation::ReadMissingBones(const aiAnimation *animation, Model *model) {
  auto size = animation->mNumChannels;

  auto &bone_info_map = model->GetBoneInfoMap();
  auto bone_count = model->GetBoneCounter();

  //Reading channels(bones engaged in an animation and their keyframes)
  for (int i = 0; i < size; ++i) {
	auto channel = animation->mChannels[i];
	auto bone_name = channel->mNodeName.data;

	if (bone_info_map.find(bone_name) == bone_info_map.end()) {
	  model->SetBoneInfoMapID(bone_name, bone_count);
	  bone_count++;
	}
	this->bones_.emplace_back(channel->mNodeName.data,
							  model->GetBoneInfo(channel->mNodeName.data).id,
							  channel);
  }

  model->SetBoneInfoMap(bone_info_map);
  model->SetBoneCounter(bone_count);

  this->bone_info_map_ = bone_info_map;
}
void Animation::ReadHierarchyData(Animation::AssimpNodeData &dest, const aiNode *src) {
  if (nullptr == src) {
	LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
									"A node skeleton error occurred while loading a node. ");
	return;
  }

  dest.name = src->mName.data;
  dest.transformation =
	  AssimpGLMHelpers::GetInstance().ConvertMatrixToGLMFormat(src->mTransformation);
  dest.children_count = src->mNumChildren;

  for (int i = 0; i < src->mNumChildren; ++i) {
	AssimpNodeData new_data;
	ReadHierarchyData(new_data, src->mChildren[i]);
	dest.children.push_back(new_data);
  }
}
Animation::Animation(const std::string &animation_path, Model *model)
	: duration_(0), ticks_per_second_(0) {
  Assimp::Importer importer;
  auto scene = importer.ReadFile(animation_path, aiProcess_Triangulate);
  if (!scene || !scene->mRootNode) {
	LoggerSystem::GetInstance().Log(LoggerSystem::Level::kInfo,
									"Error: Failed to load animation from "
										+ animation_path);
	return;
  }

  auto animation = scene->mAnimations[0];
  this->duration_ = animation->mDuration;
  this->ticks_per_second_ = animation->mTicksPerSecond;
  auto global_transformation = scene->mRootNode->mTransformation;
  global_transformation = global_transformation.Inverse();
  ReadHierarchyData(this->root_node_, scene->mRootNode);
  ReadMissingBones(animation, model);
}
Bone *Animation::FindBone(const std::string &name) {
  auto iter =
	  std::find_if(this->bones_.begin(), this->bones_.end(), [&](const Bone &bone) {
		return bone.GetName() == name;
	  });

  if (iter == this->bones_.end())
	return nullptr;

  return &(*iter);
}
