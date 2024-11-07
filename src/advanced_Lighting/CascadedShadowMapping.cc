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

#include "CascadedShadowMapping.h"

using namespace std;
std::vector<glm::vec4> CascadedShadowMapping::GetFrustumCornersWorldSpace(
    const glm::mat4& projection, const glm::mat4& view) {
  const auto inv = glm::inverse(projection * view);

  vector<glm::vec4> frustum_corners;

  for (unsigned int x = 0; x < 2; ++x) {
    for (unsigned int y = 0; y < 2; ++y) {
      for (unsigned int z = 0; z < 2; ++z) {
        // back NDC
        glm::vec4 pt =
            inv * glm::vec4(2.0f * (float)x - 1.0f, 2.0f * (float)y - 1.0f,
                            2.0f * (float)z - 1.0f, 1.0f);
        frustum_corners.push_back(pt / pt.w);
      }
    }
  }
  return frustum_corners;
}
