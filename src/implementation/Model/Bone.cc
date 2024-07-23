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

#include <utility>

#include "include/LoggerSystem.h"
#include "include/Model/Bone.h"

const glm::mat4& Bone::GetLocalTransform() const {
  return local_transform_;
}
void Bone::SetLocalTransform(const glm::mat4& local_transform) {
  local_transform_ = local_transform;
}
const std::string& Bone::GetBoneName() const {
  return bone_name_;
}
void Bone::SetBoneName(const std::string& name) {
  bone_name_ = name;
}
glm::int32 Bone::GetId() const {
  return bone_id_;
}
void Bone::SetId(glm::int32 id) {
  bone_id_ = id;
}
Bone::Bone(std::string bone_name, int bone_id, const aiNodeAnim* channel)
    : bone_name_(std::move(bone_name)), bone_id_(bone_id), local_transform_(1.0f) {

  for (int position_index = 0; position_index < channel->mNumPositionKeys;
       ++position_index) {
    KeyPosition data{};
    data.position = AssimpGLMHelpers::GetInstance().Assimp3DToGLMVec3(
        channel->mPositionKeys[position_index].mValue);
    data.time_stamp = channel->mPositionKeys[position_index].mTime;
    this->positions_.push_back(data);
  }

  for (int rotation_index = 0; rotation_index < channel->mNumRotationKeys;
       ++rotation_index) {
    KeyRotation data{};
    data.orientation =
        AssimpGLMHelpers::GetInstance().AssimpQuaternionToGLMQuaternion(
            channel->mRotationKeys[rotation_index].mValue);
    data.time_stamp = channel->mRotationKeys[rotation_index].mTime;
    this->rotations_.push_back(data);
  }

  for (int key_index = 0; key_index < channel->mNumScalingKeys; ++key_index) {
    KeyScale data{};
    data.scale = AssimpGLMHelpers::GetInstance().Assimp3DToGLMVec3(
        channel->mScalingKeys[key_index].mValue);
    data.time_stamp = channel->mScalingKeys[key_index].mTime;
    this->scales_.push_back(data);
  }
}
void Bone::Update(glm::float64 animation_time) {
  glm::mat4 translation = InterpolatePosition(animation_time);
  glm::mat4 rotation = InterpolateRotation(animation_time);
  glm::mat4 scale = InterpolateScale(animation_time);
  this->local_transform_ = translation * rotation * scale;
}
glm::int32 Bone::GetPositionsIndex(glm::float64 animation_time) {
  for (int index = 0; index < this->positions_.size() - 1; ++index) {
    if (animation_time < this->positions_[index + 1].time_stamp)
      return index;
  }

  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                  "The position index of the animation point "
                                  "is not stored, the file is from: " +
                                      bone_name_);
  throw std::runtime_error(
      "The position index of the animation point "
      "is not stored, the file is from: " +
      bone_name_);
}
glm::int32 Bone::GetRotationIndex(glm::float64 animation_time) {
  for (int index = 0; index < this->rotations_.size() - 1; ++index) {
    if (animation_time < this->rotations_[index + 1].time_stamp)
      return index;
  }

  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                  "The rotation index of the animation point "
                                  "is not stored, the file is from: " +
                                      bone_name_);
  throw std::runtime_error(
      "The rotation index of the animation point "
      "is not stored, the file is from: " +
      bone_name_);
}
glm::int32 Bone::GetScaleIndex(glm::float64 animation_time) {
  for (int index = 0; index < this->scales_.size() - 1; ++index) {
    if (animation_time < this->scales_[index + 1].time_stamp)
      return index;
  }

  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                  "The scale index of the animation point is "
                                  "not stored, the file is from: " +
                                      bone_name_);
  throw std::runtime_error(
      "The scale index of the animation point is "
      "not stored, the file is from: " +
      bone_name_);
}
glm::float64 Bone::GetScaleFactor(glm::float64 last_time_stamp,
                                  glm::float64 next_time_stamp,
                                  glm::float64 animation_time) {
  auto mid_way_length = animation_time - last_time_stamp;
  auto frames_diff = next_time_stamp - last_time_stamp;
  auto scale_factor = mid_way_length / frames_diff;
  return scale_factor;
}
glm::mat4 Bone::InterpolatePosition(glm::float64 animation_time) {
  if (1 == this->positions_.size())
    return glm::translate(glm::mat4(1.0f), this->positions_[0].position);

  auto last_time_index = GetPositionsIndex(animation_time);
  auto next_time_index = last_time_index + 1;
  auto scale_factor = GetScaleFactor(
      this->positions_[last_time_index].time_stamp,
      this->positions_[next_time_index].time_stamp, animation_time);

  auto final_position =
      glm::mix(this->positions_[last_time_index].position,
               this->positions_[next_time_index].position, scale_factor);
  return glm::translate(glm::mat4(1.0f), final_position);
}
glm::mat4 Bone::InterpolateRotation(glm::float64 animation_time) {
  if (1 == this->rotations_.size()) {
    auto rotation = glm::normalize(this->rotations_[0].orientation);
    return glm::toMat4(rotation);
  }

  auto last_time_index = GetRotationIndex(animation_time);
  auto next_time_index = last_time_index + 1;
  auto scale_factor = GetScaleFactor(
      this->rotations_[last_time_index].time_stamp,
      this->rotations_[next_time_index].time_stamp, animation_time);

  auto final_rotation =
      glm::slerp(this->rotations_[last_time_index].orientation,
                 this->rotations_[next_time_index].orientation,
                 static_cast<glm::float32>(scale_factor));

  return glm::toMat4(final_rotation);
}
glm::mat4 Bone::InterpolateScale(glm::float64 animation_time) {
  if (1 == this->scales_.size())
    return glm::scale(glm::mat4(1.0f), this->scales_[0].scale);

  auto last_time_index = GetScaleIndex(animation_time);
  auto next_time_index = last_time_index + 1;

  auto scale_factor =
      GetScaleFactor(this->scales_[last_time_index].time_stamp,
                     this->scales_[next_time_index].time_stamp, animation_time);

  auto final_scale =
      glm::mix(this->scales_[last_time_index].scale,
               this->scales_[next_time_index].scale, scale_factor);

  return glm::scale(glm::mat4(1.0f), final_scale);
}
