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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_BONEINFO_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_BONEINFO_H_

#include "glm/glm.hpp"

namespace model {
struct BoneInfo {
  // Id is index in final bone matrices
  int id;

  // offset matrix transforms vertex from model space to bone space
  glm::mat4 offset;
};
}  // namespace model

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_BONEINFO_H_
