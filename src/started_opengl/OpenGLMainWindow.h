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

#ifndef CMAKE_OPEN_SRC_STARTED_OPENGL_OPENGLMAINWINDOW_H_
#define CMAKE_OPEN_SRC_STARTED_OPENGL_OPENGLMAINWINDOW_H_

#include "include/Buffer.h"
#include "include/Camera.h"
#include "include/Imgui/ImGuiDashboard.h"
#include "include/Imgui/ImGuiWidget.h"
#include "include/OpenGLWindow.h"
#include "include/Shader.h"
#include "include/VertexArray.h"

class OpenGLMainWindow:public OpenGLWindow{
 protected:
 public:
  OpenGLMainWindow(int width, int height, const char* title,
                   GLFWmonitor* monitor= nullptr, GLFWwindow* share= nullptr);

 protected:
  void InitializeGL() override;
  void ResizeGL(int width, int height) override;
  void PaintGL() override;
  
  void ProcessInput(GLFWwindow* window) override;
  
  static void MouseCallback(GLFWwindow * window,double x_pos_in,double y_pos_in);
  
  static void ScrollCallback(GLFWwindow* window,double x_offset,double y_offset);

 private:
  Shader shader_;
  VertexArray vertex_array_;
  Buffer buffer_;
  GLuint texture_1_,texture_2_;
  
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 model;
  
  static Camera camera_;

  ImGuiDashboard* imgui_dashboard_;
};

#endif  //CMAKE_OPEN_SRC_STARTED_OPENGL_OPENGLMAINWINDOW_H_
