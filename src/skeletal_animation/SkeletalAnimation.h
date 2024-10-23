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

#ifndef CMAKE_OPEN_SRC_SKELETAL_ANIMATION_SKELETALANIMATION_H_
#define CMAKE_OPEN_SRC_SKELETAL_ANIMATION_SKELETALANIMATION_H_

#include "include/Buffers.h"
#include "include/Camera.h"
#include "include/Experimental/SkyBox.h"
#include "include/Model/Animator.h"
#include "include/Model/Model.h"
#include "include/OpenGLWindow.h"
#include "include/Shader.h"
#include "include/VertexArray.h"

class SkeletalAnimation : public OpenGLWindow {
 protected:
 public:
  SkeletalAnimation(int width, int height, const char* title,
                    GLFWmonitor* monitor, GLFWwindow* share);

 protected:
  void InitializeGL() override;
  void ResizeGL(int width, int height) override;
  void PaintGL() override;
  void ProcessInput(GLFWwindow* window) override;

 private:
  static void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

  static void scroll_callback(GLFWwindow* window, double x_offset,
                              double y_offset);

 private:
  static Camera camera_;
  Shader *shader_, *cube_map_shader_;
  model::Model* model_;
  model::Animator* animator_;
  model::Animation* animation_;
  GLuint cube_map_texture_, sky_box_texture_;
  VertexArray sky_box_vao_, cube_map_vao_;
  Buffers sky_box_vbo_, cube_map_vbo_;

  SkyBox* sky_box_shader_;
};

#endif  //CMAKE_OPEN_SRC_SKELETAL_ANIMATION_SKELETALANIMATION_H_
