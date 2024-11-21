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

#include "FrameBufferWindow.h"
#include "FilePathSystem.h"
#include "OpenGLMessage.h"

FrameBufferWindow::FrameBufferWindow(int width, int height, const char* title,
                                     GLFWmonitor* monitor, GLFWwindow* share)
    : OpenGLCameraWindow(width, height, title, monitor, share) {
  glfwSetWindowUserPointer(window_, this);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
}
void FrameBufferWindow::InitializeGL() {
  glEnable(GL_DEPTH_TEST);
  //HideMouse();

  cube_shader_ = new Shader(FilePathSystem::GetInstance().GetExecutablePath(
                                "frame_buffer_window.vert"),
                            FilePathSystem::GetInstance().GetExecutablePath(
                                "frame_buffer_window.frag"));
  screen_shader_ = new Shader(FilePathSystem::GetInstance().GetExecutablePath(
                                  "frame_buffer_window_screen.vert"),
                              FilePathSystem::GetInstance().GetExecutablePath(
                                  "frame_buffer_window_screen.frag"));

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
      // positions          // texture Coords
      5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
      0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,

      5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
      0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f};
  float quadVertices[] = {
      // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
      -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
      0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

      -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
      1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

  cube_vao_.Bind();
  cube_vbo_.Bind();
  cube_vbo_.SetData(&cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);
  cube_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  cube_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                      (void*)(3 * sizeof(float)));

  screen_vao_.Bind();
  screen_vbo_.Bind();
  screen_vbo_.SetData(&planeVertices, sizeof(planeVertices), GL_STATIC_DRAW);
  screen_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  screen_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));

  quad_vao_.Bind();
  quad_vbo_.Bind();
  quad_vbo_.SetData(&quadVertices, sizeof(quadVertices), GL_STATIC_DRAW);
  quad_vao_.AddBuffer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  quad_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                      (void*)(2 * sizeof(float)));

  cube_texture_loader_ = new TextureLoader(
      TextureLoader::Type::kTexture2D, FilePathSystem::GetInstance().GetPath(
                                           "resources/textures/container.jpg"));
  floor_texture_loader_ = new TextureLoader(
      TextureLoader::Type::kTexture2D,
      FilePathSystem::GetInstance().GetPath("resources/textures/metal.png"));

  cube_shader_->Use();
  cube_shader_->SetInt("texture1", 0);
  cube_shader_->UnUse();

  screen_shader_->Use();
  screen_shader_->SetInt("screenTexture", 0);
  screen_shader_->UnUse();
}

void FrameBufferWindow::PaintGL() {
  frame_buffer_->BindFrameBuffer();
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cube_shader_->Use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 projection =
      camera_.GetProjectionMatrix(this->GetWidth(), this->GetHeight());
  cube_shader_->SetMat4("view", view);
  cube_shader_->SetMat4("projection", projection);

  cube_vao_.Bind();
  cube_texture_loader_->Bind();
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  cube_shader_->SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  cube_shader_->SetMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  cube_vao_.UnBind();

  screen_vao_.Bind();
  floor_texture_loader_->Bind();
  cube_shader_->SetMat4("model", glm::mat4(1.0f));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  screen_vao_.UnBind();

  frame_buffer_->UnBindFrameBuffer();
  glDisable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  screen_shader_->Use();
  quad_vao_.Bind();
  frame_buffer_->BindTextureColor();
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
void FrameBufferWindow::ResizeGL(int width, int height) {
  OpenGLWindow::ResizeGL(width, height);
}
