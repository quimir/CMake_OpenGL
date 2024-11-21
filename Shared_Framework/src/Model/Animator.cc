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

#include "Model/Animator.h"
#include "LoggerSystem.h"
#include "Model/ModelException.h"
#include "ImGui/OpenGLLogMessage.h"

using namespace model;
const std::vector<glm::mat4>& Animator::GetFinalBoneMatrices() const {
  return final_bone_matrices_;
}
Animator::Animator(Animation* animation) {
  try {
    SetupAnimator(animation);
  } catch (ModelException& e) {
    OpenGLLogMessage::GetInstance().AddLog(
        std::string(
            "There was an error initializing the Animator class because: ") +
        e.what());
  }
}
void Animator::UpdateAnimation(glm::float64 delete_time) {
  this->delta_time_ = delete_time;
  if (this->current_animation_) {
    this->current_time_ +=
        current_animation_->GetTicksPerSecond() * delta_time_;
    this->current_time_ =
        fmod(this->current_time_, current_animation_->GetDuration());
    CalculateBoneTransform(this->current_animation_->GetRootNode(),
                           glm::mat4(1.0f));
  }
}
void Animator::ResetAnimation(Animation* p_animation) {
  SetupAnimator(p_animation);
}
void Animator::CalculateBoneTransform(
    const Animation::AssimpNodeData& node_data, glm::mat4 parent_transform) {
  std::lock_guard<std::recursive_mutex> lock(bone_matrices_mutex_);
  std::string node_name = node_data.name;
  glm::mat4 node_transform = node_data.transformation;

  auto bone = this->current_animation_->FindBone(node_name);
  if (bone) {
    bone->Update(this->current_time_);
    node_transform = bone->GetLocalTransform();
  }

  auto global_transformation = parent_transform * node_transform;
  auto bone_info_map = this->current_animation_->GetBoneInfoMap();
  if (bone_info_map.find(node_name) != bone_info_map.end()) {
    auto index = bone_info_map[node_name].id;
    auto offset = bone_info_map[node_name].offset;
    this->final_bone_matrices_[index] = global_transformation * offset;
  }

  for (int i = 0; i < node_data.children_count; ++i) {
    CalculateBoneTransform(node_data.children[i], global_transformation);
  }
}
void Animator::SetupAnimator(Animation* animation) {
  if (nullptr == animation) {
    throw ModelException(LoggerSystem::Level::kWarning,
                         "The animation class is not initialized, "
                         "so please initialize it and try again.");
  }
  this->current_animation_ = animation;
  auto bone_count = this->current_animation_->GetBones().size();
  this->final_bone_matrices_ = std::vector<glm::mat4>(bone_count);
  this->current_time_ = 0.0f;
  this->delta_time_ = 0.0f;
}
Animator::~Animator() {
  delete current_animation_;
}
