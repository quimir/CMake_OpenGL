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

#include "include/OpenGLStateManager.h"

std::once_flag OpenGLStateManager::initialized_;
OpenGLStateManager* OpenGLStateManager::instance_ = nullptr;
void OpenGLStateManager::EnableDepthTest() {
  if (!depth_test_enabled_) {
    glEnable(GL_DEPTH_TEST);
    depth_test_enabled_ = true;
  }
}
void OpenGLStateManager::DisableDepthTest() {
  if (depth_test_enabled_) {
    glDisable(GL_DEPTH_TEST);
    depth_test_enabled_ = false;
  }
}
void OpenGLStateManager::SetDepthFunc(GLenum depth_func) {
  if (depth_func != UINT_MAX) {
    glDepthFunc(depth_func);
  }
}
bool OpenGLStateManager::IsEnableOpenGL() {
  return !((const char*)(glGetString(GL_VERSION)) == nullptr);
}
OpenGLStateManager& OpenGLStateManager::GetInstance() {
  if (instance_ == nullptr) {
    std::call_once(initialized_,
                   []() { instance_ = new OpenGLStateManager(); });
  }
  return *instance_;
}
void OpenGLStateManager::EnableStencilTest() {
  if (!stencil_test_enabled_) {
    glEnable(GL_STENCIL_TEST);
    stencil_test_enabled_ = true;
  }
}
void OpenGLStateManager::DisableStencilTest() {
  if (stencil_test_enabled_) {
    glDisable(GL_STENCIL_TEST);
    stencil_test_enabled_ = false;
  }
}
void OpenGLStateManager::SetStencilFunc(GLenum func, GLint ref, GLuint mask) {
  glStencilFunc(func, ref, mask);
}
void OpenGLStateManager::SetStencilOp(GLenum sfail, GLenum dpfail,
                                      GLenum dppass) {
  glStencilOp(sfail, dpfail, dppass);
}
void OpenGLStateManager::SetStencilMask(GLuint mask) {
  glStencilMask(mask);
}
void OpenGLStateManager::SetColorClear(glm::vec4 color_value) {
  glClearColor(color_value.r, color_value.g, color_value.b, color_value.a);
  color_enabled_ = true;
}
void OpenGLStateManager::SetColorClear(float red, float green, float blue,
                                       float alpha) {
  glClearColor(red, green, blue, alpha);
  color_enabled_ = true;
}
