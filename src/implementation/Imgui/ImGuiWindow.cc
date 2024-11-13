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

#include "include/ImGui/ImGuiWindow.h"
#include "include/ImGui/OpenGLLogMessage.h"
#include "include/OpenGLException.h"

constexpr float kDistance = 10.0f;

using namespace AppUI;

ImGuiWindow::ImGuiWindow(GLFWwindow* window, int window_width,
                         int window_height)
    : ImGuiWidget(window, window_width, window_height) {}
void ImGuiWindow::ShowOpenGLErrorLog() {
  OpenGLLogMessage& logs = OpenGLLogMessage::GetInstance();

  if (!logs.IsHasError())
    return;

  ImGui::OpenPopup("Error");

  ImGui::SetNextWindowPos(
      ImVec2(GetIo()->DisplaySize.x * 0.5f, GetIo()->DisplaySize.y * 0.5f), 0,
      ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Error", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    std::string error_message;
    for (const auto& log : logs.GetLogs()) {
      error_message += log + "\n";
    }

    ImGui::TextWrapped("%s", error_message.c_str());
    ImGui::Separator();

    if (ImGui::Button("Close")) {
      ImGui::CloseCurrentPopup();
      OpenGLLogMessage::GetInstance().Clear();
      exit(0);
    }

    ImGui::EndPopup();
  }
}
void ImGuiWindow::ShowDashboardWindow(bool& is_open,
                                      const RenderTimer& render_timer) {
  if (ImGui::CollapsingHeader("Information's")) {
    ImGui::Checkbox("ShowDashBoard", &is_open);
  }

  if (!is_open) {
    return;
  }

  static int corner = 0;

  if (corner != -1) {
    // 0 : top-left
    // 1 : top-right
    // 2 : bottom-left
    // 3 : bottom-right
    // corner & 1 ==> (top-left || bottom-right)
    // corner & 2 ==> (bottom-left || bottom-right)
    ImVec2 window_pos =
        ImVec2((corner & 1) ? GetIo()->DisplaySize.x - kDistance : kDistance,
               (corner & 2) ? GetIo()->DisplaySize.y - kDistance : kDistance);
    ImVec2 window_pos_pivot =
        ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
  }
  ImGui::SetNextWindowBgAlpha(0.35f);
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
      ImGuiWindowFlags_NoNav;
  if (corner != -1) {
    window_flags |= ImGuiWindowFlags_NoMove;
  }
  if (ImGui::Begin("OpenGL Dashboard", &is_open, window_flags)) {
    ImGui::Text(" --- Dashboard --- ");
    ImGui::Separator();
    ImGui::Text(
        "ImGUi Window Setting\n"
        "Window Size : %d x %d\n"
        "(right-click to change position)",
        this->GetWidth(), this->GetHeight());
    ImGui::Separator();

    ImGui::Text("FPS : %.1f (ds : %.3f ms/frame)", render_timer.GetFPS(),
                render_timer.GetRenderDelay());
    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::MenuItem("Custom", nullptr, corner == -1))
        corner = -1;
      if (ImGui::MenuItem("Top-left", nullptr, corner == 0))
        corner = 0;
      if (ImGui::MenuItem("Top-right", nullptr, corner == 1))
        corner = 1;
      if (ImGui::MenuItem("Bottom-left", nullptr, corner == 2))
        corner = 2;
      if (ImGui::MenuItem("Bottom-right", nullptr, corner == 3))
        corner = 3;
      if (is_open && ImGui::MenuItem("Close"))
        is_open = false;
      ImGui::EndPopup();
    }
  }
  ImGui::End();
}
void ImGuiWindow::ShowComputerSettingWindow(
    const OpenGLWindow::OpenGLVersion& opengl_version, int& VSYNC_value,
    GLFWmonitor* monitor) {
  try {
    if (ImGui::CollapsingHeader("Window Settings")) {
      ImGui::Text("OpenGL version: %d.%d", opengl_version.major,
                  opengl_version.minor);
      ImGui::Text(
          "OpenGL Type: %s",
          OpenGLWindow::OpenGLVersionToString(opengl_version.type).c_str());
      ImGui::Text("OpenGL Render: %s", opengl_version.renderer.c_str());

      int count;
      const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);
      // Resolution memory
      std::vector<std::pair<int, int>> unique_resolutions;
      // Refresh rate storage
      std::vector<int> available_refresh_rates;
      for (int i = 0; i < count; ++i) {
        std::pair<int, int> res = {modes[i].width, modes[i].height};
        if (std::find(unique_resolutions.begin(), unique_resolutions.end(),
                      res) == unique_resolutions.end()) {
          unique_resolutions.push_back(res);
        }
      }

      static int current_resolution = 0;
      if (current_resolution >= unique_resolutions.size()) {
        current_resolution = 0;
      }

      // Update the available refresh rate options based on the current resolution
      available_refresh_rates.clear();
      for (int i = 0; i < count; ++i) {
        if (modes[i].width == unique_resolutions[current_resolution].first &&
            modes[i].height == unique_resolutions[current_resolution].second)
          available_refresh_rates.push_back(modes[i].refreshRate);
      }

      static int current_refresh_rate = 0;
      if (current_refresh_rate >= available_refresh_rates.size()) {
        current_refresh_rate = 0;
      }

      static int current_mode = 0;
      const char* window_modes[] = {"Windowed", "Fullscreen"};

      if (GetOpenGLWindow() == nullptr) {
        throw OpenGLException(LoggerSystem::Level::kError,
                              "Current window context is NULL.");
      }

      // Select Window Mode (Window/Full screen
      if (ImGui::BeginCombo("Window Mode", window_modes[current_mode])) {
        for (int i = 0; i < IM_ARRAYSIZE(window_modes); ++i) {
          bool is_selected = (current_mode == i);
          if (ImGui::Selectable(window_modes[i], is_selected)) {
            current_mode = i;
            const GLFWvidmode* current_mode_info = &modes[current_resolution];
            if (current_mode == 0) {  // Windowed mode
              glfwSetWindowMonitor(
                  GetOpenGLWindow(), nullptr, 100, 100,
                  unique_resolutions[current_resolution].first,
                  unique_resolutions[current_resolution].second, 0);
            } else {  // Fullscreen mode
              glfwSetWindowMonitor(
                  GetOpenGLWindow(), monitor, 0, 0,
                  unique_resolutions[current_resolution].first,
                  unique_resolutions[current_resolution].second,
                  available_refresh_rates[current_refresh_rate]);
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }

      std::vector<std::string> resolutions_str;
      resolutions_str.reserve(unique_resolutions.size());
      for (auto& unique_resolution : unique_resolutions) {
        resolutions_str.push_back(std::to_string(unique_resolution.first) +
                                  "x" +
                                  std::to_string(unique_resolution.second));
      }

      if (ImGui::BeginCombo("Resolution",
                            resolutions_str[current_resolution].c_str())) {
        for (int i = 0; i < unique_resolutions.size(); ++i) {
          bool is_selected = (current_resolution == i);
          if (ImGui::Selectable(resolutions_str[i].c_str(), is_selected)) {
            current_resolution = i;
            available_refresh_rates.clear();
            for (int j = 0; j < count; ++j) {
              if (modes[j].width ==
                      unique_resolutions[current_resolution].first &&
                  modes[j].height ==
                      unique_resolutions[current_resolution].second) {
                available_refresh_rates.push_back(modes[j].refreshRate);
              }
            }

            if (current_mode == 0) {
              glfwSetWindowMonitor(
                  GetOpenGLWindow(), nullptr, 100, 100,
                  unique_resolutions[current_resolution].first,
                  unique_resolutions[current_resolution].second, 0);
            } else {
              glfwSetWindowMonitor(
                  GetOpenGLWindow(), monitor, 0, 0,
                  unique_resolutions[current_resolution].first,
                  unique_resolutions[current_resolution].second,
                  available_refresh_rates[current_refresh_rate]);
            }
          }
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }

      if (!available_refresh_rates.empty()) {
        std::vector<std::string> refresh_rate_str;
        refresh_rate_str.reserve(available_refresh_rates.size());
        for (int available_refresh_rate : available_refresh_rates) {
          refresh_rate_str.push_back(std::to_string(available_refresh_rate) +
                                     "Hz");
        }

        if (ImGui::BeginCombo("Refresh Rate",
                              refresh_rate_str[current_refresh_rate].c_str())) {
          for (int i = 0; i < available_refresh_rates.size(); ++i) {
            bool is_selected = (current_refresh_rate == i);
            if (ImGui::Selectable(refresh_rate_str[i].c_str(), is_selected)) {
              current_refresh_rate = i;

              if (current_mode == 1) {
                glfwSetWindowMonitor(
                    GetOpenGLWindow(), monitor, 0, 0,
                    unique_resolutions[current_resolution].first,
                    unique_resolutions[current_resolution].second,
                    available_refresh_rates[current_refresh_rate]);
              }
            }
            if (is_selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }

      static bool enable_VSYNC = VSYNC_value == 0 ? false : true;
      ImGui::Checkbox("Enable VSYNC", &enable_VSYNC);
      if (enable_VSYNC) {
        VSYNC_value = 1;
        ImGui::DragInt("Set VSYNC", &VSYNC_value, 1.0f, -1, INT_MAX);
      } else {
        VSYNC_value = 0;
      }
    }
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(e.what());
  }
}
void ImGuiWindow::ShowCameraSettingWindow(Camera& camera, bool& open_mouse) {
  if (ImGui::CollapsingHeader("Camera")) {
    if (ImGui::Button("Reset")) {
      camera.ResetCamera();
    }
    bool enable_camera = camera.IsEnabled();
    ImGui::Checkbox("Enabled Camera Control(C)", &enable_camera);
    if (enable_camera)
      camera.Enable();
    else
      camera.DisEnable();

    ImGui::Checkbox("Enable mouse control", &open_mouse);
    ImGui::Separator();
    ImGui::Text("Transform: ");
    ImGui::Separator();
    ImGui::Indent();
    glm::vec3 camera_setting = camera.GetPosition();
    ImGui::DragFloat3("Pos: ", &camera_setting.x, 0.01f, -100.0f, 100.0f);
    camera.SetPosition(camera_setting);
    ImGui::Separator();
    auto camera_view_setting = camera.GetZoom();
    ImGui::DragFloat("Fov: ", &camera_view_setting, 0.1f, 1.0f, 60.0f);
    camera.SetZoom(camera_view_setting);
    camera_view_setting = camera.GetNearPlane();
    ImGui::DragFloat("Near: ", &camera_view_setting, 0.1f, 0.1f, 1000.0f);
    camera.SetNearPlane(camera_view_setting);
    camera_view_setting = camera.GetFarPlane();
    ImGui::DragFloat("Far: ", &camera_view_setting, 0.1f, 0.1f, 1000.0f);
    camera.SetFarPlane(camera_view_setting);
    camera_view_setting = camera.GetYaw();
    ImGui::DragFloat("Yaw: ", &camera_view_setting, 0.1f);
    camera.SetYaw(camera_view_setting);
    camera_view_setting = camera.GetPitch();
    ImGui::DragFloat("Pitch: ", &camera_view_setting, 0.01f);
    camera.SetPitch(camera_view_setting);
    camera_view_setting = camera.GetMovementSpeed();
    ImGui::DragFloat("Movement Speed: ", &camera_view_setting, 0.1f);
    camera.SetMovementSpeed(camera_view_setting);
    camera_view_setting = camera.GetRotateSpeed();
    ImGui::DragFloat("Rotate Speed: ", &camera_view_setting, 0.1f);
    camera.SetRotateSpeed(camera_view_setting);
    ImGui::Unindent();
  }
}
