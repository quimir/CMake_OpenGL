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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIWINDOW_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIWINDOW_H_

#include "ImGuiWidget.h"
#include "include/Time/RenderTimer.h"
#include "include/OpenGLWindow.h"
#include "include/Camera.h"

class ImGuiWindow : public ImGuiWidget {
 public:
  ImGuiWindow(GLFWwindow* window, int window_width, int window_height);

  void ShowDashboardWindow(bool& is_open, const RenderTimer& render_timer);

  void ShowComputerSettingWindow(
      const OpenGLWindow::OpenGLVersion& opengl_version, int& VSYNC_value,
      GLFWmonitor* monitor);
  
  void ShowCameraSettingWindow(Camera& camera,bool& open_mouse);
  
  void ShowOpenGLErrorLog();
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIWINDOW_H_
