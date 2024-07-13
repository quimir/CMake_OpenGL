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

#include "assimp/scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

class AssimpGLMHelpers {
 public:
  static AssimpGLMHelpers &GetInstance();

  glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4 &form);

  glm::vec3 GetGLMVec(const aiVector3D &vec);

  glm::quat GetGLMQuaternion(const aiQuaternion &p_quaternion);

  ~AssimpGLMHelpers() = default;

 private:
  AssimpGLMHelpers() = default;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_ASSIMPGLMHELPERS_H_
