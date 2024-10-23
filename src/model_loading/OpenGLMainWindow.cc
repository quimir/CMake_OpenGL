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

#include "OpenGLMainWindow.h"
#include <thread>
#include "include/FilePathSystem.h"
#include "include/LoadImage.h"
#include "include/OpenGLMessage.h"

bool OpenGLMainWindow::first_mouse_ = true;
GLdouble OpenGLMainWindow::last_x_;
GLdouble OpenGLMainWindow::last_y_;
Camera OpenGLMainWindow::camera_ = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

void OpenGLMainWindow::InitializeGL() {
  /*
   * configure global opengl state
   */
  glEnable(GL_DEPTH_TEST);

  // Tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
  //LoadImage::GetInstance().EnableStbImageFlipYAxis();
  this->shader_ = new Shader("model.vert", "model.frag");
  this->model_ = new model::Model(FilePathSystem::GetInstance().GetPath(
      "resources/objects/cyborg/cyborg.obj"));
}
void OpenGLMainWindow::ResizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}
void OpenGLMainWindow::PaintGL() {
  GLdouble current_frame = glfwGetTime();
  delta_time_ = current_frame - last_frame_;
  last_frame_ = current_frame;

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_->Use();
  glm::mat4 projection = camera_.GetProjectionMatrix(GetWidth(), GetHeight());
  glm::mat4 view = camera_.GetViewMatrix();
  shader_->SetMat4("projection", projection);
  shader_->SetMat4("view", view);

  // render the loaded model
  auto model = glm::mat4(1.0f);
  model = glm::translate(
      model,
      glm::vec3(0.0f, 0.0f,
                0.0f));  // translate it down so it's at the center of the scene
  model = glm::scale(
      model,
      glm::vec3(1.0f, 1.0f,
                1.0f));  // it's a bit too big for our scene, so scale it down
  shader_->SetMat4("model", model);
  model_->Draw(*shader_);
  shader_->UnUse();
}
void OpenGLMainWindow::ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    OpenGLMainWindow::camera_.ProcessKeyboard(Camera::CameraMovement::kForward,
                                              delta_time_);

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    OpenGLMainWindow::camera_.ProcessKeyboard(Camera::CameraMovement::kBackward,
                                              delta_time_);

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    OpenGLMainWindow::camera_.ProcessKeyboard(Camera::CameraMovement::kLeft,
                                              delta_time_);

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    OpenGLMainWindow::camera_.ProcessKeyboard(Camera::CameraMovement::kRight,
                                              delta_time_);

  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
    paused = !paused;
    if (paused) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
      glfwSetCursorPos(window, last_x_, last_y_);  // Reset cursor position
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    // Wait a bit to prevent immediate toggling
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}
OpenGLMainWindow::OpenGLMainWindow(int width, int height, const char* title)
    : OpenGLWindow(width, height, title, nullptr, nullptr),
      delta_time_(0.0f),
      last_frame_(0.0f),
      paused(false) {
  last_x_ = width / 2;
  last_y_ = height / 2;
  // Set callback functions for mouse and scroll
  glfwSetCursorPosCallback(this->window_, MouseCallback);
  glfwSetScrollCallback(this->window_, ScrollCallback);
  //glfwSetMouseButtonCallback(this->window_, MouseButtonCallback);
  //glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetWindowUserPointer(this->window_, this);
  HideMouse();
}
void OpenGLMainWindow::MouseCallback(GLFWwindow* window, double x_pos_in,
                                     double y_pos_in) {
  if (first_mouse_) {
    last_x_ = x_pos_in;
    last_y_ = y_pos_in;
    first_mouse_ = false;
  }

  auto x_offset = x_pos_in - last_x_;
  auto y_offset =
      last_y_ - y_pos_in;  // Reversed since y-coordinates go from bottom to top

  last_x_ = x_pos_in;
  last_y_ = y_pos_in;

  camera_.ProcessMouseMovement(x_offset, y_offset);
}
void OpenGLMainWindow::ScrollCallback(GLFWwindow* glfw_window,
                                      GLdouble x_offset, GLdouble y_offset) {
  auto* open_gl_main_window =
      static_cast<OpenGLMainWindow*>(glfwGetWindowUserPointer(glfw_window));
  if (open_gl_main_window) {
    OpenGLMainWindow::camera_.ProcessMouseScroll(
        static_cast<GLfloat>(y_offset));
  }
}
void OpenGLMainWindow::MouseButtonCallback(GLFWwindow* window, int button,
                                           int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    auto* open_gl_main_window =
        static_cast<OpenGLMainWindow*>(glfwGetWindowUserPointer(window));
    if (open_gl_main_window && open_gl_main_window->paused) {
      open_gl_main_window->paused = false;
      glfwSetCursorPos(window, OpenGLMainWindow::last_x_,
                       OpenGLMainWindow::last_y_);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  }
}
