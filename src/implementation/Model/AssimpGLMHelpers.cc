
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

#include "include/Model/AssimpGLMHelpers.h"

using namespace model;

std::once_flag AssimpGLMHelpers::initialized_;
AssimpGLMHelpers* AssimpGLMHelpers::instance_ = nullptr;
AssimpGLMHelpers& AssimpGLMHelpers::GetInstance() {
  if (instance_ == nullptr) {
    std::call_once(initialized_, [] { instance_ = new AssimpGLMHelpers(); });
  }
  return *instance_;
}
glm::mat4 AssimpGLMHelpers::ConvertMatrix4ToGLMFormat(const aiMatrix4x4& form) {
  glm::mat4 to;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      to[i][j] = form[j][i];  // i and j are swapped because rows of Assimp are
                              // columns of GLM.
    }
  }
  return to;
}
glm::vec3 AssimpGLMHelpers::Assimp3DToGLMVec3(const aiVector3D& vec) {
  return {vec.x, vec.y, vec.z};
}
glm::quat AssimpGLMHelpers::AssimpQuaternionToGLMQuaternion(
    const aiQuaternion& p_quaternion) {
  return {p_quaternion.w, p_quaternion.x, p_quaternion.y, p_quaternion.z};
}
glm::vec2 AssimpGLMHelpers::Assimp2DToGLMVec2(const aiVector2D& vec) {
  return {vec.x, vec.y};
}
glm::mat3 AssimpGLMHelpers::ConvertMatrix3ToGLMFormat(const aiMatrix3x3& form) {
  glm::mat3 to;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      to[i][j] = form[j][i];  // i and j are swapped because rows of Assimp are
                              // columns of GLM.
    }
  }

  return to;
}
AssimpGLMHelpers::~AssimpGLMHelpers() {
  delete instance_;
}
