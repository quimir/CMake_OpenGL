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
glm::vec4 stencil_test_frag_color_value(0.04, 0.28, 0.26, 1.0);

GLenum depth_mode = GL_LESS;
GLenum stencil_mode = GL_NOTEQUAL;

std::vector<glm::vec3> vegetation{
    glm::vec3(-1.5f, 0.0f, -0.48f), glm::vec3(1.5f, 0.0f, 0.51f),
    glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3(0.5f, 0.0f, -0.6f)};

void OpenGLMainWindow::InitializeGL() {
  imgui_dashboard_ = new ImGuiDashboard(window_, GetWidth(), GetHeight());
  OpenGLStateManager::GetInstance().EnableDepthTest();
  OpenGLStateManager::GetInstance().SetDepthFunc(depth_mode);
  OpenGLStateManager::GetInstance().EnableStencilTest();
  OpenGLStateManager::GetInstance().SetStencilFunc(stencil_mode, 1, 0xFF);
  OpenGLStateManager::GetInstance().SetStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  shader_ = new Shader("depth_setting.vert", "depth_setting.frag");
  stencil_testing_shader_ = new Shader(
      FilePathSystem::GetInstance().GetPath("resources/glsl/stencil_test.vert"),
      FilePathSystem::GetInstance().GetPath(
          "resources/glsl/stencil_test.frag"));
  blending_shader_ = new Shader("depth_setting.vert", "blending.frag");

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
  float transparentVertices[] = {
      // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
      0.0f, 0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -0.5f, 0.0f,
      0.0f, 1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

      0.0f, 0.5f, 0.0f, 0.0f,  0.0f, 1.0f, -0.5f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.5f,  0.0f, 1.0f, 0.0f};

  cube_vao_.Bind();
  cube_vbo_.Bind();
  cube_vbo_.SetData(&cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);
  cube_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                      (void*)nullptr);
  cube_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                      (void*)(3 * sizeof(float)));
  cube_vao_.UnBind();
  cube_vbo_.UnBind();

  plane_vao_.Bind();
  plane_vbo_.Bind();
  plane_vbo_.SetData(&planeVertices, sizeof(planeVertices), GL_STATIC_DRAW);
  plane_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                       (void*)nullptr);
  plane_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                       (void*)(3 * sizeof(float)));
  plane_vao_.UnBind();
  plane_vbo_.UnBind();

  blending_vao_.Bind();
  blending_vbo_.Bind();
  blending_vbo_.SetData(&transparentVertices, sizeof(transparentVertices),
                        GL_STATIC_DRAW);
  blending_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)nullptr);
  blending_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
  blending_vbo_.UnBind();
  blending_vao_.UnBind();

  cube_texture_ = LoadImage::GetInstance().LoadTextureAuto2D(
      FilePathSystem::GetInstance().GetPath("resources/textures/marble.jpg"));
  floor_texture_ = LoadImage::GetInstance().LoadTextureAuto2D(
      FilePathSystem::GetInstance().GetPath("resources/textures/metal.png"));
  blending_texture_loader_ = new TextureLoader(
      TextureLoader::Type::kTexture2D,
      FilePathSystem::GetInstance().GetPath("resources/textures/grass.png"),
      GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

  shader_->Use();
  shader_->SetInt("texture1", 0);
  shader_->UnUse();

  blending_shader_->Use();
  blending_shader_->SetInt("texture1", 0);
}
void OpenGLMainWindow::ResizeGL(int width, int height) {
  imgui_dashboard_->ReSizeWidget(width, height);
  OpenGLWindow::ResizeGL(width, height);
}
void OpenGLMainWindow::PaintGL() {
  OpenGLStateManager::GetInstance().SetDepthFunc(depth_mode);
  OpenGLStateManager::GetInstance().SetColorClear(clear_col);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  shader_->Use();
  glm::mat4 model(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 projection = camera_.GetProjectionMatrix(GetWidth(), GetHeight());
  shader_->SetMat4("view", view);
  shader_->SetMat4("projection", projection);
  shader_->SetFloat("near", camera_.GetNearPlane());
  shader_->SetFloat("far", camera_.GetFarPlane());
  shader_->UnUse();

  stencil_testing_shader_->Use();
  stencil_testing_shader_->SetMat4("view", view);
  stencil_testing_shader_->SetMat4("projection", projection);
  stencil_testing_shader_->SetVec4("frag_color_value",
                                   stencil_test_frag_color_value);
  stencil_testing_shader_->UnUse();

  glStencilMask(0x00);
  shader_->Use();
  plane_vao_.Bind();
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, floor_texture_);
  shader_->SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  plane_vao_.UnBind();

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);
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
  shader_->UnUse();

  OpenGLStateManager::GetInstance().SetStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  OpenGLStateManager::GetInstance().SetStencilMask(0x00);
  OpenGLStateManager::GetInstance().DisableDepthTest();
  stencil_testing_shader_->Use();
  float scale = 1.1f;
  cube_vao_.Bind();
  glBindTexture(GL_TEXTURE_2D, cube_texture_);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  model = glm::scale(model, glm::vec3(scale, scale, scale));
  stencil_testing_shader_->SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  model = glm::scale(model, glm::vec3(scale, scale, scale));
  stencil_testing_shader_->SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  cube_vao_.UnBind();
  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 0, 0xFF);
  glEnable(GL_DEPTH_TEST);
  stencil_testing_shader_->UnUse();

  blending_vao_.Bind();
  blending_texture_loader_->Bind();

  blending_shader_->Use();
  blending_shader_->SetMat4("projection", projection);
  blending_shader_->SetMat4("view", view);

  for (int i = 0; i < vegetation.size(); ++i) {
    model = glm::mat4(1.0f);
    model = glm::translate(model, vegetation[i]);
    blending_shader_->SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  static bool show_dashboard = true;
  static bool is_shader_far = false;
  static bool is_shader_far_bool = false;

  imgui_dashboard_->BeginFrame();
  this->imgui_dashboard_->Begin("Advanced opengl");
  imgui_dashboard_->ShowWindowSetting(OpenGLWindow::GetOpenglVersion(),
                                      primary_monitor_);
  this->imgui_dashboard_->ShowDashboardWin(show_dashboard, GetRenderTimer());
  this->imgui_dashboard_->ShowOpenGLSetting(clear_col, depth_mode,
                                            is_shader_far, stencil_mode,
                                            stencil_test_frag_color_value);
  this->imgui_dashboard_->ShowCameraSetting(camera_);
  this->imgui_dashboard_->End();
  if (is_shader_far) {
    if (!is_shader_far_bool) {
      shader_->ResetShader("depth_setting.vert", "depth_setting_far.frag");
      is_shader_far_bool = true;
    }
  } else {
    if (is_shader_far_bool) {
      shader_->ResetShader("depth_setting.vert", "depth_setting.frag");
      is_shader_far_bool = false;
    }
  }

  imgui_dashboard_->EndFrame();
}
OpenGLMainWindow::OpenGLMainWindow(int width, int height, const char* title,
                                   GLFWmonitor* monitor, GLFWwindow* share)
    : OpenGLCameraWindow(width, height, title, monitor, share) {
  glfwSetWindowUserPointer(window_, this);
  DisplayMouse();
}
