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

#ifndef CMAKE_OPEN_SRC_ADVANCED_LIGHTING_CASCADEDSHADOWMAPPING_H_
#define CMAKE_OPEN_SRC_ADVANCED_LIGHTING_CASCADEDSHADOWMAPPING_H_

#include <vector>
#include "include/Shader.h"

class CascadedShadowMapping {
 public:
  std::vector<glm::vec4> GetFrustumCornersWorldSpace(
      const glm::mat4& projection, const glm::mat4& view);
};

#endif  //CMAKE_OPEN_SRC_ADVANCED_LIGHTING_CASCADEDSHADOWMAPPING_H_
