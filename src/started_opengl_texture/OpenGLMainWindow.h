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

//
// Created by Acer on 2024/7/3.
//

#ifndef CMAKE_OPEN_SRC_STARTED_OPENGL_TEXTURE_OPENGLMAINWINDOW_H_
#define CMAKE_OPEN_SRC_STARTED_OPENGL_TEXTURE_OPENGLMAINWINDOW_H_

#include "include/OpenGLWindow.h"
#include "include/Shader.h"
#include "include/FilePathSystem.h"
#include "include/Buffer.h"
#include "include/VertexArray.h"

class OpenGLMainWindow :public OpenGLWindow {
 public:
  OpenGLMainWindow(int width, int height, const char *title);
 protected:
  void InitializeGL() override;
  void ResizeGL(int width, int height) override;
  void PaintGL() override;

 private:
  glm::float64 delta_time_;
  glm::float64 last_frame_;

  Shader shader_;
  
  VertexArray vao_;
  
  Buffer vbo_,ebo_;

  GLuint VAO_, VBO_, EBO_;

  GLuint texture_1, texture_2;
};

#endif //CMAKE_OPEN_SRC_STARTED_OPENGL_TEXTURE_OPENGLMAINWINDOW_H_
