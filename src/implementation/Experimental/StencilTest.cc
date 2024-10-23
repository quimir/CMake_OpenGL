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

#include "include/Experimental/StencilTest.h"
#include "include/FilePathSystem.h"
#include "include/LoggerSystem.h"
#include "include/OpenGLException.h"

StencilTest::StencilTest(GLenum depth_func_mode, GLenum func, GLint ref,
                         GLuint mask, GLenum sfail, GLenum dpfail,
                         GLenum dppass)
    : shader_(FilePathSystem::GetInstance().GetPath(
                  "resources/glsl/stencil_test.vert"),
              FilePathSystem::GetInstance().GetPath(
                  "resources/glsl/stencil_test.frag")) {
  try {
    if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
      throw Exception(
          LoggerSystem::Level::kError,
          "Serious error! Initialize OpenGL before building shaders!");
    }

    OpenGLStateManager::GetInstance().EnableDepthTest();
    OpenGLStateManager::GetInstance().SetDepthFunc(depth_func_mode);
    OpenGLStateManager::GetInstance().EnableStencilTest();
    OpenGLStateManager::GetInstance().SetStencilFunc(func, ref, mask);
    OpenGLStateManager::GetInstance().SetStencilOp(sfail, dpfail, dppass);
  } catch (Exception& e) {
    std::cerr << e.what() << std::endl;
    exit(0);
  }
}
void StencilTest::SetView(const std::string& uniform_view_name,
                          glm::mat4 view) {
  shader_.SetMat4(uniform_view_name, view);
}
void StencilTest::SetProjection(const std::string& uniform_projection_name,
                                glm::mat4 projection) {
  shader_.SetMat4(uniform_projection_name, projection);
}
void StencilTest::SetModel(const std::string& uniform_model_name,
                           glm::mat4 model) {
  shader_.SetMat4(uniform_model_name, model);
}
void StencilTest::SetFragColorValue(glm::vec4 frag_color_value) {
  shader_.SetVec4("frag_color_value", frag_color_value);
}
void StencilTest::UseShader() {
  shader_.Use();
}
void StencilTest::UnUseShader() {
  shader_.UnUse();
}
