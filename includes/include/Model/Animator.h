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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_ANIMATOR_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_ANIMATOR_H_

#include <map>
#include <vector>

#include "assimp/scene.h"
#include "glm/glm.hpp"

#include "Animation.h"

/**
 * The Animator class is responsible for updating and resetting the animation 
 * state of an Animation object. It calculates bone transformations based on 
 * the animation data and provides access to the final bone matrices.
 * 
 * Usage example:
 * @code
 * Model* model_ = new Model(FilePathSystem::GetInstance().GetPath(
 * "resources/objects/vampire/dancing_vampire.dae"));
 * Animation* animation_ =new Animation(FilePathSystem::GetInstance().GetPath(
 * "resources/objects/vampire/dancing_vampire.dae"),model_);
 * Animator* animator_=new Animator(animation_);
 * 
 * animator_->UpdateAnimation(this->GetRenderTimer().ElapsedSeconds() * 10.0f);
 * @endcode
 * 
 * @note This class is thread-safe. It uses internal mutexes to protect access 
 * to shared resources.
 */
class Animator {
 public:
  /**
   * Constructor for the Animator class. Initializes the animator with an
   * Animation object.
   * @param animation The Animation object to animate.
   */
  explicit Animator(Animation* animation);

  /**
   * Updates the animation state based on the specified delta time.
   * @param delete_time The time difference since the last update.
   */
  void UpdateAnimation(glm::float64 delete_time);

  /**
   * Resets the animation state to the beginning of the current Animation.
   * @param p_animation The new Animation object to animate.
   */
  void ResetAnimation(Animation* p_animation);

  /**
   * Calculates the bone transformations for the specified node data and parent
   * transform.
   * @param node_data The AssimpNodeData representing the node to calculate the
   * transform for.
   * @param parent_transform The parent transform matrix.
   */
  void CalculateBoneTransform(const Animation::AssimpNodeData& node_data,
                              glm::mat4 parent_transform);

  /**
   * Retrieves the final bone matrices calculated by the animator.
   * @return A const reference to the vector of final bone matrices.
   */
  const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

  ~Animator();

 private:
  /**
   * Initializes the animator with the specified Animation object.
   * @param animation The Animation object to animate.
   */
  void SetupAnimator(Animation* animation);

 private:
  // The final bone matrices calculated by the animator.
  std::vector<glm::mat4> final_bone_matrices_;

  // The current Animation object being animated.
  Animation* current_animation_;
  // The current time in the animation.
  glm::float64 current_time_;
  // The delta time since the last update.
  glm::float64 delta_time_;

  /**
   * A recursive lock that prevents data from being accessed simultaneously in 
   * multiple threads.
   */
  std::recursive_mutex bone_matrices_mutex_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_ANIMATOR_H_
