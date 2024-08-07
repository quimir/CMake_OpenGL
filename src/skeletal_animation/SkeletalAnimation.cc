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

#include "SkeletalAnimation.h"
#include "include/FilePathSystem.h"
#include "include/LoadImage.h"
#include "include/LoggerSystem.h"

using namespace std;

float last_x;
float last_y;
bool first_mouse = true;
float delta_time = 0;
float last_frame = 0;
Camera SkeletalAnimation::camera_(glm::vec3(0.0f, 0.0f, 3.0f));
void SkeletalAnimation::InitializeGL() {
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

  vector<string> faces{"right.jpg",  "left.jpg",  "top.jpg",
                       "bottom.jpg", "front.jpg", "back.jpg"};
  glEnable(GL_DEPTH_TEST);

  last_x = GetWidth() / 2.0f;
  last_y = GetHeight() / 2.0f;

  shader_ = new Shader("animation_model.vert", "animation_model.frag");
  model_ = new Model(FilePathSystem::GetInstance().GetPath(
      "resources/objects/vampire/dancing_vampire.dae"));
  animation_ =
      new Animation(FilePathSystem::GetInstance().GetPath(
                        "resources/objects/vampire/dancing_vampire.dae"),
                    model_);
  animator_ = new Animator(animation_);
  cube_map_shader_ = new Shader(
      FilePathSystem::GetInstance().GetResourcesPath("glsl/cube_maps.vert"),
      FilePathSystem::GetInstance().GetResourcesPath("glsl/cube_maps.frag"));

  cube_map_vao_.Bind();
  cube_map_vbo_.Bind();
  cube_map_vbo_.SetData(&cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);
  cube_map_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)0);
  cube_map_vao_.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
  cube_map_vao_.UnBind();
  cube_map_vbo_.UnBind();

  for (auto& face : faces) {
    face.insert(
        0, FilePathSystem::GetInstance().GetPath("resources/textures/skybox/"));
  }
  cube_map_texture_ = LoadImage::GetInstance().LoadTexture2D(
      FilePathSystem::GetInstance().GetPath(
          "resources/textures/container.jpg"));
  cube_map_shader_->Use();
  cube_map_shader_->SetInt("texture1", 0);
  cube_map_shader_->UnUse();

  sky_box_shader_ = new SkyBox(faces);
}
void SkeletalAnimation::ResizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}
void SkeletalAnimation::PaintGL() {
  float current_time = glfwGetTime();
  delta_time = current_time - last_frame;
  last_frame = current_time;
  animator_->UpdateAnimation(this->GetRenderTimer().ElapsedSeconds() * 10.0f);

  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_->Use();

  auto projection = camera_.GetProjectionMatrix(GetWidth(), GetHeight());
  auto view = camera_.GetViewMatrix();
  shader_->SetMat4("projection", projection);
  shader_->SetMat4("view", view);

  auto trans_forms = animator_->GetFinalBoneMatrices();
  for (int i = 0; i < trans_forms.size(); ++i) {
    shader_->SetMat4("final_bones_matrices[" + std::to_string(i) + "]",
                     trans_forms[i]);
  }

  auto model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
  model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
  shader_->SetMat4("model", model);

  model_->Draw(*shader_);
  shader_->UnUse();

  cube_map_shader_->Use();
  cube_map_shader_->SetMat4("projection", projection);
  cube_map_shader_->SetMat4("view", view);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
  cube_map_shader_->SetMat4("model", model);
  cube_map_vao_.Bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cube_map_texture_);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  cube_map_vao_.UnBind();
  cube_map_shader_->UnUse();

  sky_box_shader_->Bind(projection, view);
}
void SkeletalAnimation::ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kForward,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kBackward,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kLeft,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(Camera::CameraMovement::kRight,
                            this->GetRenderTimer().ElapsedSeconds() * 10.0f);
  OpenGLWindow::ProcessInput(window);
}
SkeletalAnimation::SkeletalAnimation(int width, int height, const char* title,
                                     GLFWmonitor* monitor, GLFWwindow* share)
    : OpenGLWindow(width, height, title, monitor, share) {
  glfwSetWindowUserPointer(window_, this);
  glfwSetCursorPosCallback(window_, mouse_callback);
  glfwSetScrollCallback(window_, scroll_callback);
  HideMouse();
  camera_.IsEnabled();
}
void SkeletalAnimation::mouse_callback(GLFWwindow* window, double x_pos,
                                       double y_pos) {
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
void SkeletalAnimation::scroll_callback(GLFWwindow* window, double x_offset,
                                        double y_offset) {
  camera_.ProcessMouseScroll(y_offset);
}
