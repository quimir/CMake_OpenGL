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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_ASSIMPGLMHELPERS_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_ASSIMPGLMHELPERS_H_

#include <mutex>
#include "assimp/scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

/**
 * The AssimpGLMHelpers class provides utility functions for converting 
 * Assimp's matrix and vector types to GLM's matrix and vector types. It is 
 * designed to be a singleton, ensuring that only one instance of the class 
 * is created throughout the application.
 * 
 * Usage example:
 * @code
 * aiMatrix4x4 form;
 * glm::mat4 matrix=Assimp::GetInstance().ConvertMatrix4ToGLMFormat(form);
 * glm::vec3 glm_vec3=.....
 * @endcode
 * 
 * @note This class is thread-safe. The use of an internal mutex ensures that 
 * an instance is generated only once.
 */
class AssimpGLMHelpers {
 public:
  /**
   * Retrieves the unique instance of the AssimpGLMHelpers class. This method 
   * is thread-safe and ensures that only one instance of the class is created.
   * @return A reference to the unique instance of the AssimpGLMHelpers class.
   */
  static AssimpGLMHelpers& GetInstance();

  /**
   * Converts an Assimp matrix4x4 to a GLM matrix4. The conversion involves 
   * swapping the rows and columns of the Assimp matrix to match the GLM matrix
   * format.
   * @param form The Assimp matrix4x4 to convert.
   * @return A GLM matrix4 representing the converted Assimp matrix.
   */
  glm::mat4 ConvertMatrix4ToGLMFormat(const aiMatrix4x4& form);

  /**
   * Converts an Assimp matrix3x3 to a GLM matrix3. The conversion involves 
   * swapping the rows and columns of the Assimp matrix to match the GLM 
   * matrix format.
   * @param form The Assimp matrix3x3 to convert.
   * @return A GLM matrix3 representing the converted Assimp matrix.
   */
  glm::mat3 ConvertMatrix3ToGLMFormat(const aiMatrix3x3& form);

  /**
   * Converts an Assimp vector3D to a GLM vec3.
   * @param vec The Assimp vector3D to convert.
   * @return A GLM vec3 representing the converted Assimp vector.
   */
  glm::vec3 Assimp3DToGLMVec3(const aiVector3D& vec);

  /**
   * Converts an Assimp vector2D to a GLM vec2.
   * @param vec The Assimp vector2D to convert.
   * @return A GLM vec2 representing the converted Assimp vector.
   */
  glm::vec2 Assimp2DToGLMVec2(const aiVector2D& vec);

  /**
   * Converts an Assimp quaternion to a GLM quat.
   * @param p_quaternion The Assimp quaternion to convert.
   * @return A GLM quat representing the converted Assimp quaternion.
   */
  glm::quat AssimpQuaternionToGLMQuaternion(const aiQuaternion& p_quaternion);

  /**
   * Destructor for the AssimpGLMHelpers class. Frees the allocated resources.
   */
  ~AssimpGLMHelpers();

 private:
  AssimpGLMHelpers() = default;

 private:
  /**
   * Static flag to ensure thread-safe initialization of the instance.
   */
  static std::once_flag initialized_;
  /**
   * Pointer to the unique instance of the AssimpGLMHelpers class.
   */
  static AssimpGLMHelpers* instance_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_ASSIMPGLMHELPERS_H_
