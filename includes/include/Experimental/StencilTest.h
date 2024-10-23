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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_STENCILTEST_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_STENCILTEST_H_

#include "include/OpenGLStateManager.h"
#include "include/Shader.h"

class StencilTest {
 public:
  StencilTest(GLenum depth_func_mode, GLenum func, GLint ref, GLuint mask,
              GLenum sfail, GLenum dpfail, GLenum dppass);

  void SetView(const std::string& uniform_view_name, glm::mat4 view);

  void SetProjection(const std::string& uniform_projection_name,
                     glm::mat4 projection);

  void SetModel(const std::string& uniform_model_name, glm::mat4 model);
  
  void SetFragColorValue(glm::vec4 frag_color_value);
  
  void UseShader();
  
  void UnUseShader();

 private:
  Shader shader_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_STENCILTEST_H_
