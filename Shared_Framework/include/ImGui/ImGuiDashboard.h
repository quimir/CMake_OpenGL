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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIDASHBOARD_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIDASHBOARD_H_

#include "ImGuiWidget.h"
#include "../Camera.h"
#include "../OpenGLWindow.h"
#include "../Time/RenderTimer.h"

class ImGuiDashboard : public ImGuiWidget {
 public:
  explicit ImGuiDashboard(GLFWwindow* window, int window_width,
                          int window_height);

  void Render() override;

  void Begin(const char* name, bool* p_open = nullptr,
             ImGuiWindowFlags flags = 0);

  void End();

  void ShowDashboardWin(bool& open, const RenderTimer& render_time);

  void SetRenderTimer(const RenderTimer& render_timer);

  void ShowToolsPanel(bool& show_dashboard, glm::vec4& clear_col,
                      Camera& camera, GLenum& current_depth_func,
                      bool& shader_far);

  GLenum ShowDepthTextMode(GLenum current_depth_func);

  void ShowOpenGLSetting(glm::vec4& opengl_color, GLenum& current_depth_func,
                         bool& shader_far, GLenum& stencil_mode,
                         glm::vec4& stencil_test_frag_color_value);

  void ShowDepthTextMode(GLenum& current_depth_func, bool& shader_far);

  void ShowStencilTest(GLenum& stencil_mode,
                       glm::vec4& stencil_test_frag_color_value);

  void ShowCameraSetting(Camera& camera);

  void ShowWindowSetting(OpenGLWindow::OpenGLVersion opengl_version,
                         GLFWmonitor* monitor);

 private:
  void SetDashboardWin(bool* open, const RenderTimer& render_time);

 private:
  RenderTimer render_timer_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIDASHBOARD_H_
