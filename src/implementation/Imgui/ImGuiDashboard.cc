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

#include "include/Imgui/ImGuiDashboard.h"
#include <unordered_map>
#include "glm/glm.hpp"
#include "include/Time/RenderTimer.h"

constexpr float kDistance = 10.0f;

ImGuiDashboard::ImGuiDashboard(GLFWwindow* window, int window_width,
                               int window_height)
    : ImGuiWidget(window, window_width, window_height), render_timer_() {}
void ImGuiDashboard::Render() {
  static bool dash_board_window = true;
  ShowDashboardWin(&dash_board_window);
}
void ImGuiDashboard::ShowDashboardWin(bool* open) {
  if (!open || !(*open)) {
    return;  // If the open pointer is null or the value it points to is false, return early
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
  if (ImGui::Begin("OpenGL Dashboard", open, window_flags)) {
    ImGui::Text(" --- Dashboard --- ");
    ImGui::Separator();
    ImGui::Text(
        "Demo : 09_Add_GUI_System_Dear_ImGui\n"
        "Window Size : %d x %d\n"
        "(right-click to change position)",
        this->GetWidth(), this->GetHeight());
    ImGui::Separator();

    ImGui::Text("FPS : %.1f (ds : %.3f ms/frame)", render_timer_.GetFPS(),
                render_timer_.GetRenderDelay());
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
      if (open && ImGui::MenuItem("Close"))
        *open = false;
      ImGui::EndPopup();
    }
  }
  ImGui::End();
}
void ImGuiDashboard::SetRenderTimer(const RenderTimer& render_timer) {
  render_timer_ = render_timer;
}
void ImGuiDashboard::ShowToolsPanel(bool& show_dashboard, glm::vec4& clear_col,
                                    Camera& camera, GLenum& current_depth_func,
                                    bool& shader_far) {
  ImGui::Begin("OpenGL Control Panel");
  if (ImGui::CollapsingHeader("Informations")) {
    ImGui::Text("IsAnyItemActive : %s",
                ImGui::IsAnyItemActive() ? "true" : "false");
    ImGui::Checkbox("ShowDashBoard", &show_dashboard);
  }

  if (ImGui::CollapsingHeader("Draw State")) {
    ImGui::ColorEdit4("ClearColor", &clear_col.x, ImGuiColorEditFlags_None);
    current_depth_func = ShowDepthTextMode(current_depth_func);
    ImGui::Checkbox("Shader far", &shader_far);
  }
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
    ImGui::Unindent();
  }

  ImGui::End();
  if (show_dashboard) {
    ShowDashboardWin(&show_dashboard);
  }
}
GLenum ImGuiDashboard::ShowDepthTextMode(GLenum current_depth_func) {
  static std::unordered_map<GLenum, int> depth_func_map = {
      {GL_NEVER, 0},   {GL_LESS, 1},     {GL_EQUAL, 2},  {GL_LEQUAL, 3},
      {GL_GREATER, 4}, {GL_NOTEQUAL, 5}, {GL_GEQUAL, 6}, {GL_ALWAYS, 7}};

  static GLenum depth_funcs[] = {GL_NEVER,   GL_LESS,     GL_EQUAL,  GL_LEQUAL,
                                 GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS};

  static const char* depth_func_items[] = {
      "GL_NEVER",   "GL_LESS",     "GL_EQUAL",  "GL_LEQUAL",
      "GL_GREATER", "GL_NOTEQUAL", "GL_GEQUAL", "GL_ALWAYS"};

  int current_item = depth_func_map[current_depth_func];

  if (ImGui::BeginCombo("Depth Func", depth_func_items[current_item])) {
    for (int i = 0; i < IM_ARRAYSIZE(depth_func_items); i++) {
      bool is_selected = (current_item == i);
      if (ImGui::Selectable(depth_func_items[i], is_selected)) {
        current_item = i;
        current_depth_func = depth_funcs[i];
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  return current_depth_func;
}
