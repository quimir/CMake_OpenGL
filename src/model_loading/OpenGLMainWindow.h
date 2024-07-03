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

#ifndef CMAKE_OPEN_SRC_MODEL_LOADING_OPENGLMAINWINDOW_H_
#define CMAKE_OPEN_SRC_MODEL_LOADING_OPENGLMAINWINDOW_H_

#include "include/OpenGLWindow.h"
#include "include/Camera.h"

class OpenGLMainWindow: protected OpenGLWindow{
 public:
  OpenGLMainWindow(int width,int height,const char* title);
 protected:
  void InitializeGL() override;
  
  void ResizeGL(int width, int height) override;
  
  void PaintGL() override;
  
  void ProcessInput(GLFWwindow *window) override;
  
 private:
  static void MouseCallback(GLFWwindow* window,GLdouble x_pos,GLdouble y_pos);
  
  static void ScrollCallback(GLFWwindow *glfw_window, GLdouble x_offset, GLdouble y_offset);
  
  static void MouseButtonCallback(GLFWwindow* window,int button,int action,int mods);
  
 private:
  static Camera camera_;
  
  glm::float64 delta_time_;
  glm::float64 last_frame_;
  
  bool paused;
  
  static bool first_mouse_;
  static GLdouble last_x_;
  static GLdouble last_y_;
};

#endif //CMAKE_OPEN_SRC_MODEL_LOADING_OPENGLMAINWINDOW_H_
