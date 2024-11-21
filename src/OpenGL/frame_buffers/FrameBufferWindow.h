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

#ifndef CMAKE_OPEN_SRC_FRAME_BUFFERS_FRAMEBUFFERWINDOW_H_
#define CMAKE_OPEN_SRC_FRAME_BUFFERS_FRAMEBUFFERWINDOW_H_

#include "Buffers.h"
#include "Experimental/OpenGLCameraWindow.h"
#include "Shader.h"
#include "VertexArray.h"
#include "TextureLoader.h"

class FrameBufferWindow : public OpenGLCameraWindow {
 public:
  FrameBufferWindow(int width, int height, const char* title,
                    GLFWmonitor* monitor, GLFWwindow* share);

 protected:
  void InitializeGL() override;
  void PaintGL() override;
  void ResizeGL(int width, int height) override;

 private:
  Shader *cube_shader_, *screen_shader_;
  VertexArray cube_vao_, screen_vao_, quad_vao_;
  Buffers cube_vbo_, screen_vbo_, quad_vbo_;
  
  TextureLoader* cube_texture_loader_,*floor_texture_loader_;
};

#endif  //CMAKE_OPEN_SRC_FRAME_BUFFERS_FRAMEBUFFERWINDOW_H_
