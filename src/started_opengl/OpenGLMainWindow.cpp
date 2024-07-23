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
#include "include/FilePathSystem.h"
#include "include/Imgui/ImGuiDashboard.h"
#include "include/Imgui/ImGuiWidget.h"
#include "include/LoadImage.h"

bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

float last_x = 0.0f;
float last_y = 0.0f;

Camera OpenGLMainWindow::camera_(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec4 clear_color(0.2f, 0.3f, 0.3f, 1.0f);

// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

void OpenGLMainWindow::InitializeGL() {
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  this->vertex_array_.Bind();
  this->buffer_.Bind();
  this->buffer_.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

  this->vertex_array_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void*)nullptr);
  this->vertex_array_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void*)(3 * sizeof(float)));

  this->vertex_array_.UnBind();
  this->buffer_.UnBind();

  LoadImage::GetInstance().EnableStbImageFlipYAxis();
  this->texture_1_ = LoadImage::GetInstance().LoadTexture2D(
      FilePathSystem::GetInstance().GetResourcesPath("textures/container.jpg"));
  this->texture_2_ = LoadImage::GetInstance().LoadTexture2D(
      FilePathSystem::GetInstance().GetResourcesPath(
          "textures/awesomeface.png"));

  shader_.Bind();
  shader_.SetInt("texture1", 0);
  shader_.SetInt("texture2", 1);
  shader_.UnBind();
}
void OpenGLMainWindow::ResizeGL(int width, int height) {
  this->projection =
      glm::perspective(glm::radians(camera_.GetZoom()),
                       (float)width / (float)height, 0.1f, 100.0f);
  shader_.Bind();
  shader_.SetMat4("projection", projection);
  this->imgui_dashboard_->ResizeWidget(width, height);
  glViewport(0, 0, width, height);
}
void OpenGLMainWindow::PaintGL() {
  auto current_time = static_cast<float>(glfwGetTime());
  delta_time = current_time - last_frame;
  std::cerr << "Glfw delta time: " << delta_time
            << " render delta time: " << GetRenderTimer().ElapsedSeconds()
            << std::endl;
  last_frame = current_time;
  glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_1_);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture_2_);

  shader_.Bind();

  this->projection = camera_.GetProjectionMatrix(GetWidth(), GetHeight());
  shader_.SetMat4("projection", projection);
  view = camera_.GetViewMatrix();
  shader_.SetMat4("view", view);

  vertex_array_.Bind();
  for (int i = 0; i < 10; i++) {
    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * (float)i * glfwGetTime();
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    shader_.SetMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  imgui_dashboard_->SetRenderTimer(this->GetRenderTimer());
  imgui_dashboard_->BeginFrame();
  static bool show_dashboard = false;
  imgui_dashboard_->ShowToolsPanel(show_dashboard, clear_color, camera_);
  //imgui_dashboard_->Render();
  imgui_dashboard_->EndFrame();
}
OpenGLMainWindow::OpenGLMainWindow(int width, int height, const char* title,
                                   GLFWmonitor* monitor, GLFWwindow* share)
    : OpenGLWindow(width, height, title, monitor, share),
      shader_("camera.vert", "camera.frag", nullptr, nullptr, nullptr) {
  glEnable(GL_DEPTH_TEST);
  glfwSetWindowUserPointer(window_, this);
  glfwSetCursorPosCallback(window_, MouseCallback);
  glfwSetScrollCallback(window_, ScrollCallback);
  //  glfwSwapInterval(1);
  last_x = width / 2.0f;
  last_y = height / 2.0f;
  imgui_dashboard_ = new ImGuiDashboard(window_, width, height);
}
void OpenGLMainWindow::ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kForward, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kBackward,
                            GetRenderTimer().ElapsedSeconds());
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kLeft,
                            GetRenderTimer().ElapsedSeconds());
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kRight,
                            GetRenderTimer().ElapsedSeconds());
  OpenGLWindow::ProcessInput(window);
}

void OpenGLMainWindow::MouseCallback(GLFWwindow* window, double x_pos_in,
                                     double y_pos_in) {
  auto x_pos = static_cast<float>(x_pos_in);
  auto y_pos = static_cast<float>(y_pos_in);

  if (first_mouse) {
    last_x = x_pos;
    last_y = y_pos;
    first_mouse = false;
  }

  float x_offset = x_pos - last_x;
  float y_offset = last_y - y_pos;

  last_x = x_pos;
  last_y = y_pos;

  camera_.ProcessMouseMovement(x_offset, y_offset);
}

void OpenGLMainWindow::ScrollCallback(GLFWwindow* window, double x_offset,
                                      double y_offset) {
  camera_.ProcessMouseScroll(static_cast<float>(y_offset));
}
