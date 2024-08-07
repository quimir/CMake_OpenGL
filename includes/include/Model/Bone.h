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

#include <list>
#include <vector>

#include "assimp/scene.h"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "AssimpGLMHelpers.h"

/**
 * This class handles the skeletal information within a model and computes the 
 * motion of each bone over time.This class is usually used in conjunction with 
 * the Animation class. This class is only responsible for storing data and 
 * computing data without the risk of memory leaks.
 */
class Bone {
 public:
  // Represents a key position in the animation.
  struct KeyPosition {
    // The 3D position vector of the bone.
    glm::vec3 position;
    // The time stamp of this key position.
    glm::float64 time_stamp;
  };

  // Represents a key rotation in the animation.
  struct KeyRotation {
    // The orientation quaternion of the bone.
    glm::quat orientation;
    // The time stamp of this key rotation.
    glm::float64 time_stamp;
  };
  
  // Represents a key scale in the animation.
  struct KeyScale {
    // The scale vector of the bone.
    glm::vec3 scale;       
    // The time stamp of this key scale.
    glm::float64 time_stamp;  
  };

 public:
  explicit Bone(std::string bone_name, int bone_id, const aiNodeAnim* channel);

  /**
   * Updates the local transform of the bone based on the animation time.
   * @param animation_time The current time of the animation.
   */
  void Update(glm::float64 animation_time);

  /**
   * Gets the local transformation matrix of the bone.
   * @return The local transformation matrix.
   */
  const glm::mat4& GetLocalTransform() const;

  /**
   * Sets the local transformation matrix of the bone.
   * @param local_transform The new local transformation matrix.
   */
  void SetLocalTransform(const glm::mat4& local_transform);

  /**
   * Gets the name of the bone.
   * @return The name of the bone.
   */
  const std::string& GetBoneName() const;

  /**
   * Sets the name of the bone.
   * @param name The new name of the bone.
   */
  void SetBoneName(const std::string& name);

  /**
   * Gets the ID of the bone.
   * @return The ID of the bone.
   */
  glm::int32 GetId() const;

  /**
   * Sets the ID of the bone.
   * @param id The new ID of the bone.
   */
  void SetId(glm::int32 id);

  /**
   * Gets the index of the key position at a specific animation time.
   * @param animation_time The current time of the animation.
   * @return The index of the key position.
   */
  glm::int32 GetPositionsIndex(glm::float64 animation_time);

  /**
   * Gets the index of the key rotation at a specific animation time.
   * @param animation_time The current time of the animation.
   * @return The index of the key rotation.
   */
  glm::int32 GetRotationIndex(glm::float64 animation_time);

  /**
   * Gets the index of the key scale at a specific animation time.
   * @param animation_time The current time of the animation.
   * @return The index of the key scale.
   */
  glm::int32 GetScaleIndex(glm::float64 animation_time);

 private:
  /**
   * Calculates the scale factor for interpolation.
   * @param last_time_stamp The time stamp of the previous key frame.
   * @param next_time_stamp The time stamp of the next key frame.
   * @param animation_time The current time of the animation.
   * @return The scale factor for interpolation.
   */
  glm::float64 GetScaleFactor(glm::float64 last_time_stamp,
                              glm::float64 next_time_stamp,
                              glm::float64 animation_time);

  /**
   * Interpolates the position of the bone based on the animation time.
   * @param animation_time The current time of the animation.
   * @return The interpolated position matrix.
   */
  glm::mat4 InterpolatePosition(glm::float64 animation_time);

  /**
   * Interpolates the rotation of the bone based on the animation time.
   * @param animation_time The current time of the animation.
   * @return The interpolated rotation matrix.
   */
  glm::mat4 InterpolateRotation(glm::float64 animation_time);

  /**
   * Interpolates the scale of the bone based on the animation time.
   * @param animation_time The current time of the animation.
   * @return The interpolated scale matrix.
   */
  glm::mat4 InterpolateScale(glm::float64 animation_time);

 private:
  std::vector<KeyPosition> positions_;
  std::vector<KeyRotation> rotations_;
  std::vector<KeyScale> scales_;

  glm::mat4 local_transform_;
  std::string bone_name_;
  glm::int32 bone_id_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_BONE_H_
