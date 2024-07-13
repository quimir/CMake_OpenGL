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
#include "include/Time/RenderTimer.h"

constexpr float kDistance = 10.0f;

ImGuiDashboard::ImGuiDashboard(GLFWwindow* window, int window_width,
                               int window_height)
    : ImGuiWidget(window, window_width, window_height),
      render_timer_(RenderTimer::GetInstance()),
      camera_(Camera()) {}
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
void ImGuiDashboard::SetCamera(const Camera& camera) {
  camera_ = camera;
}
void ImGuiDashboard::ShowToolsPanel(bool show_dashboard) {
  ImGui::Begin("OpenGL Control Panel");
  if (ImGui::CollapsingHeader("Informations")) {
    ImGui::Text("IsAnyItemActive : %s", ImGui::IsAnyItemActive() ? "true" : "false");
    ImGui::Checkbox("ShowDashBoard", &show_dashboard);
  }
}
