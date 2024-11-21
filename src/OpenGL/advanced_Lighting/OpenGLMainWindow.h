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

#ifndef CMAKE_OPEN_SRC_ADVANCED_LIGHTING_OPENGLMAINWINDOW_H_
#define CMAKE_OPEN_SRC_ADVANCED_LIGHTING_OPENGLMAINWINDOW_H_

#include "Experimental/OpenGLCameraWindow.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "Buffers.h"
#include "VertexArray.h"
#include "ImGuiMainWindow.h"

class OpenGLMainWindow: public OpenGLCameraWindow{
 public:
  OpenGLMainWindow(int width, int height, const char* title,
                   GLFWmonitor* monitor, GLFWwindow* share);
  
  ~OpenGLMainWindow() override;

 protected:
  void ProcessInput(GLFWwindow* window) override;
  void InitializeGL() override;
  void ResizeGL(int width, int height) override;
  void PaintGL() override;
  
 private:
  Shader* shader_;
  VertexArray plane_vao_;
  Buffers plane_vbo_;
  TextureLoader* texture_loader_,*texture_loader_gamma_corrected_;
  bool blinn_;
  bool blinn_key_pressed_;
  int light_value_;
  ImGuiMainWindow* imGui_main_window_;
  
  GLuint depth_map_fbo_;
};

#endif  //CMAKE_OPEN_SRC_ADVANCED_LIGHTING_OPENGLMAINWINDOW_H_
