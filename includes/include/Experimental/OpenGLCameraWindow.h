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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_OPENGLCAMERAWINDOW_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_OPENGLCAMERAWINDOW_H_

#include "../Camera.h"
#include "../OpenGLWindow.h"

class OpenGLCameraWindow : public OpenGLWindow {
 public:
  OpenGLCameraWindow(int width, int height, const char* title,
                     GLFWmonitor* monitor, GLFWwindow* share);

  static void MouseCallBack(GLFWwindow* window, double x_pos, double y_pos);

  static void ScrollCallBack(GLFWwindow* window, double x_offset,
                             double y_offset);

 protected:
  void ProcessInput(GLFWwindow* window) override;

 protected:
  static Camera camera_;
  
  static bool open_mouse_;
 private:
  static bool first_mouse;
  static float last_x_;
  static float last_y_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_EXPERIMENTAL_OPENGLCAMERAWINDOW_H_
