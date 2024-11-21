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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_SKYBOX_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_SKYBOX_H_

#include <vector>
#include <string>
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Buffers.h"
#include "../TextureLoader.h"
#include "Core/MacroDefinition.h"

class SkyBox {
 public:
  explicit SkyBox(const std::vector<std::string>& faces_path,
                  bool gamma_correction, float gamma_value);

  void Bind(glm::mat4 projection, glm::mat4 view);
  
  ~SkyBox();
 private:
  Shader* sky_box_shader_;
  TextureLoader* sky_box_texture_;
  VertexArray sky_box_vao_;
  Buffers sky_box_vbo_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_SKYBOX_H_
