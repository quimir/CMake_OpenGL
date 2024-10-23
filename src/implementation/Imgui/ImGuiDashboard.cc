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
#include "include/Time/RenderTimer.h"

constexpr float kDistance = 10.0f;

std::unordered_map<GLenum, int> depth_func_map = {
    {GL_NEVER, 0},   {GL_LESS, 1},     {GL_EQUAL, 2},  {GL_LEQUAL, 3},
    {GL_GREATER, 4}, {GL_NOTEQUAL, 5}, {GL_GEQUAL, 6}, {GL_ALWAYS, 7}};

GLenum depth_funcs[] = {GL_NEVER,   GL_LESS,     GL_EQUAL,  GL_LEQUAL,
                        GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS};

const char* depth_func_items[] = {"GL_NEVER",  "GL_LESS",    "GL_EQUAL",
                                  "GL_LEQUAL", "GL_GREATER", "GL_NOTEQUAL",
                                  "GL_GEQUAL", "GL_ALWAYS"};

ImGuiDashboard::ImGuiDashboard(GLFWwindow* window, int window_width,
                               int window_height)
    : ImGuiWidget(window, window_width, window_height), render_timer_() {}
void ImGuiDashboard::Render() {
  static bool dash_board_window = true;
}
void ImGuiDashboard::SetDashboardWin(bool* open,
                                     const RenderTimer& render_time) {
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
        "ImGUi Window Setting\n"
        "Window Size : %d x %d\n"
        "(right-click to change position)",
        this->GetWidth(), this->GetHeight());
    ImGui::Separator();

    ImGui::Text("FPS : %.1f (ds : %.3f ms/frame)", render_time.GetFPS(),
                render_time.GetRenderDelay());
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
    SetDashboardWin(&show_dashboard, render_timer_);
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
void ImGuiDashboard::ShowOpenGLSetting(
    glm::vec4& opengl_color, GLenum& current_depth_func, bool& shader_far,
    GLenum& stencil_mode, glm::vec4& stencil_test_frag_color_value) {
  if (ImGui::CollapsingHeader("Draw State")) {
    ImGui::ColorEdit4("ClearColor", &opengl_color.x, ImGuiColorEditFlags_None);
    ShowDepthTextMode(current_depth_func, shader_far);
    ShowStencilTest(stencil_mode, stencil_test_frag_color_value);
  }
}
void ImGuiDashboard::ShowDepthTextMode(GLenum& current_depth_func,
                                       bool& shader_far) {
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
  ImGui::Checkbox("Shader far", &shader_far);
}
void ImGuiDashboard::ShowStencilTest(GLenum& stencil_mode,
                                     glm::vec4& stencil_test_frag_color_value) {
  int current_item = depth_func_map[stencil_mode];

  if (ImGui::BeginCombo("Depth Func", depth_func_items[current_item])) {
    for (int i = 0; i < IM_ARRAYSIZE(depth_func_items); i++) {
      bool is_selected = (current_item == i);
      if (ImGui::Selectable(depth_func_items[i], is_selected)) {
        current_item = i;
        stencil_mode = depth_funcs[i];
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  ImGui::DragFloat4("Stencil test color value",
                    &stencil_test_frag_color_value.x, 0.1f);
}

void ImGuiDashboard::ShowCameraSetting(Camera& camera) {
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
}
void ImGuiDashboard::Begin(const char* name, bool* p_open,
                           ImGuiWindowFlags flags) {
  ImGui::Begin(name, p_open, flags);
}
void ImGuiDashboard::End() {
  ImGui::End();
}
void ImGuiDashboard::ShowDashboardWin(bool& open,
                                      const RenderTimer& render_time) {
  if (ImGui::CollapsingHeader("Informations")) {
    ImGui::Text("IsAnyItemActive : %s",
                ImGui::IsAnyItemActive() ? "true" : "false");
    ImGui::Checkbox("ShowDashBoard", &open);
  }
  if (open) {
    SetDashboardWin(&open, render_time);
  }
}
void ImGuiDashboard::ShowWindowSetting(
    OpenGLWindow::OpenGLVersion opengl_version, GLFWmonitor* monitor) {
  if (ImGui::CollapsingHeader("Window Settings")) {
    ImGui::Text("OpenGL version: %d.%d", opengl_version.major,
                opengl_version.minor);
    ImGui::Text(
        "OpenGL Type: %s",
        OpenGLWindow::OpenGLVersionToString(opengl_version.type).c_str());
    ImGui::Text("OpenGL Render: %s", opengl_version.renderer.c_str());

    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);
    std::vector<std::string> resolutions;
    resolutions.reserve(count);
    for (int i = 0; i < count; ++i) {
      resolutions.push_back(std::to_string(modes[i].width) + "x" +
                            std::to_string(modes[i].height) + " @ " +
                            std::to_string(modes[i].refreshRate) + "Hz");
    }

    static int current_resolution = 0;
    if (current_resolution >= resolutions.size()) {
      current_resolution = 0;
    }

    static int current_mode = 0;
    const char* window_modes[] = {"Windowed", "Fullscreen"};

    if (ImGui::BeginCombo("Window Mode", window_modes[current_mode])) {
      for (int i = 0; i < IM_ARRAYSIZE(window_modes); ++i) {
        bool is_selected = (current_mode == i);
        if (ImGui::Selectable(window_modes[i], is_selected)) {
          current_mode = i;

          if (current_mode == 0) {  // Windowed mode
            glfwSetWindowMonitor(glfwGetCurrentContext(), nullptr, 100, 100,
                                 modes[current_resolution].width,
                                 modes[current_resolution].height, 0);
          } else {  // Fullscreen mode
            glfwSetWindowMonitor(glfwGetCurrentContext(), monitor, 0, 0,
                                 modes[current_resolution].width,
                                 modes[current_resolution].height,
                                 modes[current_resolution].refreshRate);
          }
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Resolution",
                          resolutions[current_resolution].c_str())) {
      for (int i = 0; i < resolutions.size(); ++i) {
        bool is_selected = (current_resolution == i);
        if (ImGui::Selectable(resolutions[i].c_str(), is_selected)) {
          current_resolution = i;

          if (current_mode == 0) {  // Windowed mode
            glfwSetWindowMonitor(glfwGetCurrentContext(), nullptr, 100, 100,
                                 modes[i].width, modes[i].height, 0);
          } else {  // Fullscreen mode
            glfwSetWindowMonitor(glfwGetCurrentContext(), monitor, 0, 0,
                                 modes[i].width, modes[i].height,
                                 modes[i].refreshRate);
          }
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
}
