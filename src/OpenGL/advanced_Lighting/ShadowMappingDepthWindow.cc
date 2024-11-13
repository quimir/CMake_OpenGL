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

#include "ShadowMappingDepthWindow.h"
#include "include/FilePathSystem.h"

constexpr GLuint kShadowWidth = 1024;
constexpr GLuint kShadowHeight = 1024;

static glm::vec3 light_pos(0.0f, 0.0f, 0.0f);

void ShadowMappingDepthWindow::InitializeGL() {
  glEnable(GL_DEPTH_TEST);

  point_shadow_ =
      new PointShadow(GetWidth(), GetHeight(), kShadowWidth, kShadowHeight);
  //  shadow_mapping_ =
  //      new ShadowMapping(GetWidth(), GetHeight(), kShadowWidth, kShadowHeight);
}
void ShadowMappingDepthWindow::ResizeGL(int width, int height) {
  OpenGLWindow::ResizeGL(width, height);
  imgui_window_.ReSizeWidget(width, height);
  point_shadow_->ResetWindow(Rect(0, 0, width, height));
}
void ShadowMappingDepthWindow::PaintGL() {
  float near_plane = 1.0f, far_plane = 25.0f;
  point_shadow_->Bind(near_plane, far_plane, light_pos, camera_);

  imgui_window_.BeginFrame();
  imgui_window_.ShowOpenGLErrorLog();
  imgui_window_.ShowComputerSettingWindow(GetOpenglVersion(), VSYNC_value_,
                                          primary_monitor_);
  SetVSYNC(VSYNC_value_);
  static bool is_open = true;
  imgui_window_.ShowDashboardWindow(is_open, GetRenderTimer());
  imgui_window_.ShowCameraSettingWindow(camera_, open_mouse_);
  imgui_window_.EndFrame();
}

ShadowMappingDepthWindow::ShadowMappingDepthWindow(int width, int height,
                                                   const char* title,
                                                   GLFWmonitor* monitor,
                                                   GLFWwindow* share)
    : OpenGLCameraWindow(width, height, title, monitor, share),
      imgui_window_(window_, width, height) {
  glfwSetWindowUserPointer(window_, this);
}
ShadowMappingDepthWindow::~ShadowMappingDepthWindow() {
  delete point_shadow_;
}
