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
#include "include/LoadImage.h"
#include "include/OpenGLStateManager.h"

glm::vec4 clear_col(0.1f, 0.1f, 0.1f, 0.1f);

GLenum depth_mode = GL_LESS;

void OpenGLMainWindow::InitializeGL() {
  imgui_dashboard_ = new ImGuiDashboard(window_, GetWidth(), GetHeight());
  OpenGLStateManager::GetInstance().EnableDepthTest();
  OpenGLStateManager::GetInstance().SetDepthFunc(depth_mode);

  shader_ = new Shader("depth_setting.vert", "depth_setting.frag");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float cubeVertices[] = {
      // positions          // texture Coords
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
  float planeVertices[] = {
      // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
      5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
      0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,

      5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
      0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f};

  cube_vao_.Bind();
  cube_vbo_.Bind();
  cube_vbo_.SetData(&cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);
  cube_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  cube_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                      (void*)(3 * sizeof(float)));
  cube_vao_.UnBind();
  cube_vbo_.UnBind();

  plane_vao_.Bind();
  plane_vbo_.Bind();
  plane_vbo_.SetData(&planeVertices, sizeof(planeVertices), GL_STATIC_DRAW);
  plane_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  plane_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                       (void*)(3 * sizeof(float)));
  plane_vao_.UnBind();
  plane_vbo_.UnBind();

  cube_texture_ = LoadImage::GetInstance().LoadTextureAuto2D(
      FilePathSystem::GetInstance().GetPath("resources/textures/marble.jpg"));
  floor_texture_ = LoadImage::GetInstance().LoadTextureAuto2D(
      FilePathSystem::GetInstance().GetPath("resources/textures/metal.png"));

  shader_->Use();
  shader_->SetInt("texture1", 0);
  shader_->UnUse();
}
void OpenGLMainWindow::ResizeGL(int width, int height) {
  imgui_dashboard_->ResizeWidget(width, height);
  OpenGLWindow::ResizeGL(width, height);
}
void OpenGLMainWindow::PaintGL() {
  OpenGLStateManager::GetInstance().SetDepthFunc(depth_mode);
  glClearColor(clear_col.r, clear_col.g, clear_col.b, clear_col.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_->Use();
  glm::mat4 model(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 projection = camera_.GetProjectionMatrix(GetWidth(), GetHeight());
  shader_->SetMat4("view", view);
  shader_->SetMat4("projection", projection);
  shader_->SetFloat("near", camera_.GetNearPlane());
  shader_->SetFloat("far", camera_.GetFarPlane());

  cube_vao_.Bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cube_texture_);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  shader_->SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  shader_->SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  cube_vao_.UnBind();

  plane_vao_.Bind();
  glBindTexture(GL_TEXTURE_2D, floor_texture_);
  shader_->SetMat4("model", glm::mat4(1.0f));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  plane_vao_.UnBind();
  shader_->UnUse();

  this->imgui_dashboard_->SetRenderTimer(GetRenderTimer());
  static bool show_dashboard = true;
  static bool is_shader_far = false;
  static bool is_shader_far_bool = false;
  imgui_dashboard_->BeginFrame();
  this->imgui_dashboard_->ShowToolsPanel(show_dashboard, clear_col, camera_,
                                         depth_mode, is_shader_far);
  if (is_shader_far) {
    if (!is_shader_far_bool) {
      shader_->Cleanup();
      shader_ = new Shader("depth_setting.vert", "depth_setting_far.frag");
      is_shader_far_bool = true;
    }
  } else {
    if (is_shader_far_bool) {
      shader_->Cleanup();
      shader_ = new Shader("depth_setting.vert", "depth_setting.frag");
      is_shader_far_bool = false;
    }
  }
  imgui_dashboard_->EndFrame();
}
OpenGLMainWindow::OpenGLMainWindow(int width, int height, const char* title,
                                   GLFWmonitor* monitor, GLFWwindow* share)
    : OpenGLCameraWindow(width, height, title, monitor, share) {
  glfwSetWindowUserPointer(window_, this);
  //HideMouse();
  DisplayMouse();
}
