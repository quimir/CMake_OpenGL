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
#include "include/Camera.h"
#include "include/Time/RenderTimer.h"

class ImGuiDashboard : public ImGuiWidget {
 public:
  explicit ImGuiDashboard(GLFWwindow* window, int window_width,
                          int window_height);

  void Render() override;

  void ShowDashboardWin(bool* open);

  void SetRenderTimer(const RenderTimer& render_timer);

  void ShowToolsPanel(bool& show_dashboard, glm::vec4& clear_col,
                      Camera& camera, GLenum& current_depth_func,
                      bool& shader_far);

  GLenum ShowDepthTextMode(GLenum current_depth_func);

 private:
  RenderTimer render_timer_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIDASHBOARD_H_
