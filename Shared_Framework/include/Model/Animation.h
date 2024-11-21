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

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "assimp/scene.h"
#include "glm/glm.hpp"

#include "Bone.h"
#include "BoneInfo.h"
#include "Model.h"

#include "Core/MacroDefinition.h"

namespace model {
/**
 * The Animation class represents an animation loaded from an external file. 
 * It contains information about bones, their transformations, and the duration 
 * of the animation. The class provides methods to retrieve information about 
 * bones and their transformations.
 */
class Animation {
 public:
  /**
   * Structure to hold data about an Assimp node used in the animation 
   * hierarchy.
   */
  struct AssimpNodeData {
    // The transformation matrix of the node.
    glm::mat4 transformation;
    // The name of the node.
    std::string name;
    // The number of children the node has.
    glm::uint32 children_count;
    // A vector of child node data.
    std::vector<AssimpNodeData> children;
  };

  /**
   * Constructor for the Animation class. Loads the animation from the 
   * specified file path and associates it with a Model object.
   * @param animation_path The file path of the animation file.
   * @param model The Model object associated with the animation.
   * @note This constructor is not thread-safe. It should be called in a 
   * thread-safe context, such as the main thread or after acquiring a lock.
   */
  Animation(const std::string& animation_path, Model* model);

  /**
   * Destructor for the Animation class. Frees all allocated resources.
   * @note This destructor is not thread-safe. It should be called in a 
   * thread-safe context, such as the main thread or after acquiring a lock.
   */
  ~Animation() = default;

  /**
   * Finds a bone in the animation by its name.
   * @param name The name of the bone to find.
   * @return A pointer to the bone if found, nullptr otherwise.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  Bone* FindBone(const std::string& name);

  /**
   * Retrieves the duration of the animation.
   * @return The duration of the animation in seconds.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  glm::float64 GetDuration() const;

  /**
   * Sets the duration of the animation.
   * @param duration The new duration of the animation in seconds.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  void SetDuration(glm::float64 duration);

  /**
   * Retrieves the ticks per second of the animation.
   * @return The ticks per second of the animation.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  glm::float64 GetTicksPerSecond() const;

  /**
   * Sets the ticks per second of the animation.
   * @param ticks_per_second The new ticks per second of the animation.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  void SetTicksPerSecond(glm::float64 ticks_per_second);

  /**
   * Retrieves the root node data of the animation hierarchy.
   * @return A const reference to the root node data.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  const AssimpNodeData& GetRootNode() const;

  /**
   * Sets the root node data of the animation hierarchy.
   * @param root_node The new root node data.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  void SetRootNode(const AssimpNodeData& root_node);

  /**
   * Retrieves the bone information map.
   * @return A const reference to the bone information map.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  const std::map<std::string, BoneInfo>& GetBoneInfoMap() const;

  /**
   * Sets the bone information map.
   * @param bone_info_map The new bone information map.
   * @note This method is thread-safe and can be called from multiple threads.
   */
  void SetBoneInfoMap(const std::map<std::string, BoneInfo>& bone_info_map);

  const std::vector<Bone>& GetBones() const;

 private:
  /**
   * Reads the missing bones from the animation and the associated Model.
   * @param animation The Assimp animation object.
   * @param model The associated Model object.
   * @note This method is not thread-safe and should be called in a thread-safe
   * context.
   */
  void ReadMissingBones(const aiAnimation* animation, Model* model);

  /**
   * Reads the hierarchy data from the Assimp node and populates the AssimpNodeData.
   * @param dest The AssimpNodeData to populate.
   * @param src The Assimp node to read from.
   * @note This method is not thread-safe and should be called in a thread-safe
   * context.
   */
  void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

 private:
  // The duration of the animation in seconds.
  glm::float64 duration_ = 0.0;
  //The ticks per second of the animation.
  glm::float64 ticks_per_second_ = 0.0;
  // The bones in the animation.
  std::vector<Bone> bones_;
  // The root node data of the animation hierarchy.
  AssimpNodeData root_node_;
  // The bone information map.
  std::map<std::string, BoneInfo> bone_info_map_;
};
}  // namespace model

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_ANIMATION_H_
