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
#include "include/LoadImage.h"
#include "include/OpenGLMessage.h"
OpenGLMainWindow::OpenGLMainWindow(int width, int height, const char* title)
    : OpenGLWindow(width, height, title, nullptr, nullptr),
      delta_time_(0),
      last_frame_(0),
      shader_("texture.vert", "texture.frag", nullptr, nullptr, nullptr) {
  glfwSetWindowUserPointer(this->window_, this);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
}
void OpenGLMainWindow::InitializeGL() {
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
  };
  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  this->vao_.Bind();
  this->vbo_.Bind();
  this->ebo_.ReSetType(GL_ELEMENT_ARRAY_BUFFER);
  this->ebo_.Bind();
  this->vbo_.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);
  this->ebo_.SetData(indices, sizeof(indices), GL_STATIC_DRAW);
  this->vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  this->vao_.AddBuffer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(3 * sizeof(float)));
  this->vao_.AddBuffer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(6 * sizeof(float)));

  LoadImage::GetInstance().EnableStbImageFlipYAxis();

  this->texture_1 = LoadImage::GetInstance().LoadTexture2D(
      FilePathSystem::GetInstance().GetPath(
          "resources/textures/container.jpg"));

  this->texture_2 = LoadImage::GetInstance().LoadTexture2D(
      FilePathSystem::GetInstance().GetPath(
          "resources/textures/awesomeface.png"));

  shader_
      .Use();  // don't forget to activate/use the shader before setting uniforms!
  // either set it manually like so:
  shader_.SetInt("texture1", 0);
  // or set it via the texture class
  shader_.SetInt("texture2", 1);
}
void OpenGLMainWindow::ResizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}
void OpenGLMainWindow::PaintGL() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  OpenGLMessage::GetInstance().EnableOpenGLDebugMessage();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture_2);

  shader_.Use();
  this->vao_.Bind();
  //  glBindVertexArray(this->VAO_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
