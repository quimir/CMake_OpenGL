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

#ifndef CMAKE_OPEN_SRC_ADVANCED_OPENGL_OPENGLMAINWINDOW_H_
#define CMAKE_OPEN_SRC_ADVANCED_OPENGL_OPENGLMAINWINDOW_H_

#include "include/Experimental/OpenGLCameraWindow.h"
#include "include/Shader.h"
#include "include/VertexArray.h"
#include "include/Buffers.h"
#include "include/ImGui/ImGuiDashboard.h"

class OpenGLMainWindow : public OpenGLCameraWindow {
 protected:
 public:
  OpenGLMainWindow(int width, int height, const char* title,
                   GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

 protected:
  void InitializeGL() override;
  void ResizeGL(int width, int height) override;
  void PaintGL() override;
  
 private:
  Shader* shader_;
  
  VertexArray cube_vao_,plane_vao_;
  Buffers cube_vbo_,plane_vbo_;
  
  GLuint cube_texture_,floor_texture_;
  
  ImGuiDashboard* imgui_dashboard_;
};

#endif  //CMAKE_OPEN_SRC_ADVANCED_OPENGL_OPENGLMAINWINDOW_H_
