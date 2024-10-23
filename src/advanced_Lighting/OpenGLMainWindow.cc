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
#include "thread"
OpenGLMainWindow::OpenGLMainWindow(int width, int height, const char* title,
                                   GLFWmonitor* monitor, GLFWwindow* share)
    : OpenGLCameraWindow(width, height, title, monitor, share),
      blinn_(false),
      blinn_key_pressed_(false),
      light_value_(32.0) {
  glfwSetWindowUserPointer(window_, this);
}
OpenGLMainWindow::~OpenGLMainWindow() = default;
void OpenGLMainWindow::ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinn_key_pressed_) {
    blinn_ = !blinn_;
    blinn_key_pressed_ = true;
  }
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
    blinn_key_pressed_ = false;
  }
  OpenGLCameraWindow::ProcessInput(window);
}
void OpenGLMainWindow::InitializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader_ = new Shader("advanced_lighting.vert", "advanced_lighting.frag");
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float planeVertices[] = {
      // positions            // normals         // texcoords
      10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
      -10.0f, -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,
      -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f,

      10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
      -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f,
      10.0f,  -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f};
  plane_vbo_.Bind();
  plane_vbo_.SetData(planeVertices, sizeof(planeVertices), GL_STATIC_DRAW);
  plane_vao_.Bind();
  plane_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)nullptr);
  plane_vao_.AddBuffer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(3 * sizeof(float)));
  plane_vao_.AddBuffer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(6 * sizeof(float)));
  plane_vao_.UnBind();

  texture_loader_ = new TextureLoader(
      TextureLoader::Type::kTexture2D,
      FilePathSystem::GetInstance().GetResourcesPath("textures/wood.png"));
  texture_loader_gamma_corrected_ = new TextureLoader(
      TextureLoader::Type::kTexture2D,
      FilePathSystem::GetInstance().GetResourcesPath("textures/wood.png"));

  shader_->Use();
  shader_->SetInt("floor_texture", 0);

  imGui_main_window_ =
      new ImGuiMainWindow(this->window_, GetWidth(), GetHeight());
}
void OpenGLMainWindow::ResizeGL(int width, int height) {
  this->imGui_main_window_->ReSizeWidget(width,height);
  OpenGLWindow::ResizeGL(width, height);
}
void OpenGLMainWindow::PaintGL() {
  static glm::vec3 lightPositions[] = {
      glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(3.0f, 0.0f, 0.0f)};
  static glm::vec3 lightColors[] = {glm::vec3(0.25), glm::vec3(0.50),
                                    glm::vec3(0.75), glm::vec3(1.00)};

  glm::vec3 light_pos(0.0f, 0.0f, 0.0f);
  glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_->Use();
  auto projection = camera_.GetProjectionMatrix(
      (float)this->GetRect().GetWidth(), (float)this->GetRect().GetHeight());
  glm::mat4 view = camera_.GetViewMatrix();
  shader_->SetMat4("projection", projection);
  shader_->SetMat4("view", view);
  shader_->SetVec3("light_positions", 4, &lightPositions[0][0]);
  shader_->SetVec3("light_colors", 4, &lightColors[0][0]);
  shader_->SetInt("light_value", light_value_);
  shader_->SetInt("gamma", blinn_);
  static float gamma_value = 2.2f;
  shader_->SetFloat("gamma_value", gamma_value);

  plane_vao_.Bind();
  if (blinn_) {
    texture_loader_->Bind(GL_TEXTURE0);
  } else {
    texture_loader_gamma_corrected_->Bind(GL_TEXTURE0);
  }
  glDrawArrays(GL_TRIANGLES, 0, 6);

  static bool is_open_dash_board = true;
  imGui_main_window_->BeginFrame();
  imGui_main_window_->ShowDashboardWindow(is_open_dash_board, GetRenderTimer());
  imGui_main_window_->ShowComputerSettingWindow(GetOpenglVersion(),
                                                primary_monitor_);
  imGui_main_window_->ShowCameraSettingWindow(camera_);
  imGui_main_window_->ShowLightWindow(light_value_, gamma_value);
  imGui_main_window_->EndFrame();
}
