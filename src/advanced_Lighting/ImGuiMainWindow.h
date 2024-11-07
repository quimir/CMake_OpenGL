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

#ifndef CMAKE_OPEN_SRC_ADVANCED_LIGHTING_IMGUIMAINWINDOW_H_
#define CMAKE_OPEN_SRC_ADVANCED_LIGHTING_IMGUIMAINWINDOW_H_

#include "include/ImGui/ImGuiWidget.h"
#include "include/OpenGLWindow.h"
#include "include/Time/RenderTimer.h"
#include "include/Camera.h"

class ImGuiMainWindow : public ImGuiWidget {
 public:
  ImGuiMainWindow(GLFWwindow* window, int window_width, int window_height);

  void ShowDashboardWindow(bool& is_open, const RenderTimer& render_timer);

  void ShowComputerSettingWindow(
      const OpenGLWindow::OpenGLVersion& opengl_version, GLFWmonitor* monitor);

  void ShowCameraSettingWindow(Camera& camera, bool& open_mouse);
  
  void ShowLightWindow(int& light_value,float& k_ambient);

  void ShowMappingShadow(float& bias_value, float& max_light_distance,
                  float& min_radius, float& max_radius);
  
  void ShowShadow(bool& open_shadow,bool& shadow_mode);
};

#endif  //CMAKE_OPEN_SRC_ADVANCED_LIGHTING_IMGUIMAINWINDOW_H_
