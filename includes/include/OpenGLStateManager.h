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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLSTATEMANAGER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLSTATEMANAGER_H_

#include <climits>
#include <mutex>
#include "glm/glm.hpp"
#include "glad/glad.h"

class OpenGLStateManager {
 public:
  void EnableDepthTest();

  void DisableDepthTest();

  void SetDepthFunc(GLenum depth_func = UINT_MAX);

  bool IsEnableOpenGL();

  static OpenGLStateManager& GetInstance();
  
  void EnableStencilTest();
  
  void DisableStencilTest();
  
  void SetStencilFunc(GLenum func, GLint ref, GLuint mask);
  
  void SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
  
  void SetStencilMask(GLuint mask);
  
  void SetColorClear(glm::vec4 color_value);
  
  void SetColorClear(float red,float green,float blue,float alpha);

 private:
  OpenGLStateManager() = default;

 private:
  bool stencil_test_enabled_;
  bool depth_test_enabled_;
  bool color_enabled_;
  static std::once_flag initialized_;
  static OpenGLStateManager* instance_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLSTATEMANAGER_H_
