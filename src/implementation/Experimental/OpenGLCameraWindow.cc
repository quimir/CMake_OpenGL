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

#include "include/Experimental/OpenGLCameraWindow.h"

bool OpenGLCameraWindow::first_mouse = true;
Camera OpenGLCameraWindow::camera_(glm::vec3(0.0f, 0.0f, 3.0f));
float OpenGLCameraWindow::last_x_ = 0;
float OpenGLCameraWindow::last_y_ = 0;

OpenGLCameraWindow::OpenGLCameraWindow(int width, int height, const char* title,
                                       GLFWmonitor* monitor, GLFWwindow* share)
    : OpenGLWindow(width, height, title, monitor, share) {
  glfwSetWindowUserPointer(window_, this);
  glfwSetCursorPosCallback(window_, MouseCallBack);
  glfwSetScrollCallback(window_, ScrollCallBack);
  last_x_ = width / 2;
  last_y_ = height / 2;
}
void OpenGLCameraWindow::MouseCallBack(GLFWwindow* window, double x_pos,
                                       double y_pos) {
  if (first_mouse) {
    last_x_ = x_pos;
    last_y_ = y_pos;
    first_mouse = false;
  }

  float x_offset = x_pos - last_x_;
  float y_offset = last_y_ - y_pos;

  last_x_ = x_pos;
  last_y_ = y_pos;

  camera_.ProcessMouseMovement(x_offset, y_offset);
}
void OpenGLCameraWindow::ScrollCallBack(GLFWwindow* window, double x_offset,
                                        double y_offset) {
  camera_.ProcessMouseScroll(y_offset);
}
void OpenGLCameraWindow::ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kForward,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kBackward,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kLeft,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kRight,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);

  if (glfwGetKey(window,GLFW_KEY_Q)==GLFW_PRESS)
    camera_.RotateYaw(0.1f);

  if (glfwGetKey(window,GLFW_KEY_E)==GLFW_PRESS)
    camera_.RotateYaw(-0.1f);
  
  OpenGLWindow::ProcessInput(window);
}
