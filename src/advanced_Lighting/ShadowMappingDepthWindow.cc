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

#include "ShadowMappingDepthWindow.h"
#include "include/FilePathSystem.h"

constexpr GLuint kShadowWidth = 1024;
constexpr GLuint kShadowHeight = 1024;
void ShadowMappingDepthWindow::InitializeGL() {
  glEnable(GL_DEPTH_TEST);

  shader_ = new Shader("shadow_mapping.vert", "shadow_mapping.frag");
  simple_depth_shader_ =
      new Shader("shadow_mapping_depth.vert", "shadow_mapping_depth.frag");
  debug_depth_quad_shader_ = new Shader("debug_quad.vert", "debug_quad.frag");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float planeVertices[] = {
      // positions            // normals         // texcoords
      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      -25.0f, -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f,

      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f,
      25.0f,  -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f};

  plane_vao_.Bind();
  plane_vbo_.Bind();
  plane_vbo_.SetData(planeVertices, sizeof(planeVertices), GL_STATIC_DRAW);
  plane_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(nullptr));
  plane_vao_.AddBuffer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(3 * sizeof(float)));
  plane_vao_.AddBuffer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(6 * sizeof(float)));
  plane_vbo_.UnBind();
  plane_vao_.UnBind();

  wood_texture_ = new TextureLoader(
      TextureLoader::Type::kTexture2D,
      FilePathSystem::GetInstance().GetPath("resources/textures/wood.png"));

  glGenFramebuffers(1, &depth_map_fbo_);
  glGenTextures(1, &depth_map_);
  glBindTexture(GL_TEXTURE_2D, depth_map_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, kShadowWidth,
               kShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float border_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
  glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo_);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depth_map_, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  shader_->Use();
  shader_->SetInt("diffuse_texture", 0);
  shader_->SetInt("shadow_map", 1);
  debug_depth_quad_shader_->Use();
  debug_depth_quad_shader_->SetInt("depth_map", 0);
}
void ShadowMappingDepthWindow::ResizeGL(int width, int height) {
  OpenGLWindow::ResizeGL(width, height);
  imgui_main_window_.ReSizeWidget(width, height);
}
void ShadowMappingDepthWindow::PaintGL() {
  static glm::vec3 light_pos(-2.0f, 4.0f, 1.0f);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float near_plane = 1.0f, far_plane = 7.5f;
  glm::mat4 light_projection, light_view;
  glm::mat4 light_space_matrix;
  light_projection =
      glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  light_view =
      glm::lookAt(light_pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
  light_space_matrix = light_projection * light_view;

  simple_depth_shader_->Use();
  simple_depth_shader_->SetMat4("light_space_matrix", light_space_matrix);

  glViewport(0, 0, kShadowWidth, kShadowHeight);
  glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo_);
  glClear(GL_DEPTH_BUFFER_BIT);
  wood_texture_->Bind();
  RenderScene(*simple_depth_shader_);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, GetWidth(), GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_->Use();
  shader_->SetMat4("projection",
                   camera_.GetProjectionMatrix(GetWidth(), GetHeight()));
  shader_->SetMat4("view", camera_.GetViewMatrix());
  shader_->SetVec3("view_pos", camera_.GetPosition());
  shader_->SetVec3("light_pos", light_pos);
  shader_->SetMat4("light_space_matrix", light_space_matrix);
  static float shadow_bias_value = 0.05f;
  shader_->SetFloat("bias_value", shadow_bias_value);
  static float max_light_distance = 1000.0;
  shader_->SetFloat("max_light_distance", max_light_distance);
  static float min_radius = 1.0;
  shader_->SetFloat("min_radius", min_radius);
  static float max_radius = 5.0;
  shader_->SetFloat("max_radius", max_radius);
  wood_texture_->Bind(GL_TEXTURE0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depth_map_);
  glCullFace(GL_FRONT);
  RenderScene(*shader_);
  glCullFace(GL_BACK);

  debug_depth_quad_shader_->Use();
  debug_depth_quad_shader_->SetFloat("near_plane", near_plane);
  debug_depth_quad_shader_->SetFloat("far_plane", far_plane);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depth_map_);

  imgui_main_window_.BeginFrame();
  imgui_main_window_.ShowComputerSettingWindow(GetOpenglVersion(),
                                               primary_monitor_);
  static bool is_open = true;
  imgui_main_window_.ShowDashboardWindow(is_open, GetRenderTimer());
  imgui_main_window_.ShowCameraSettingWindow(camera_);
  imgui_main_window_.ShowShadow(shadow_bias_value, max_light_distance,
                                min_radius, max_radius);
  imgui_main_window_.EndFrame();
  //RenderQuad();
}
void ShadowMappingDepthWindow::RenderCube() {
  static VertexArray cube_vao;
  static Buffers cube_vbo;

  if (cube_vao.IsEmpty()) {
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
                                                             // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f    // bottom-left
    };

    cube_vao.Bind();
    cube_vbo.Bind();
    cube_vbo.SetData(&vertices, sizeof(vertices), GL_STATIC_DRAW);
    cube_vao.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(nullptr));
    cube_vao.AddBuffer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(3 * sizeof(float)));
    cube_vao.AddBuffer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(6 * sizeof(float)));
    cube_vbo.UnBind();
    cube_vao.UnBind();
  }
  cube_vao.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  cube_vao.UnBind();
}
void ShadowMappingDepthWindow::RenderScene(const Shader& shader) {
  auto model = glm::mat4(1.0f);
  shader.SetMat4("model", model);
  plane_vao_.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);

  model =
      glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f)),
                 glm::vec3(0.5f));
  shader.SetMat4("model", model);
  RenderCube();

  model =
      glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 1.0f)),
                 glm::vec3(0.5f));
  shader.SetMat4("model", model);
  RenderCube();

  model = glm::scale(
      glm::rotate(
          (glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 2.0f))),
          glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
      glm::vec3(0.25f));
  shader.SetMat4("model", model);
  RenderCube();
}
void ShadowMappingDepthWindow::RenderQuad() {
  static VertexArray quad_vao;
  static Buffers quad_vbo;

  if (quad_vao.IsEmpty()) {
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    quad_vao.Bind();
    quad_vbo.Bind();
    quad_vbo.SetData(&quadVertices, sizeof(quadVertices), GL_STATIC_DRAW);
    quad_vao.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                       (void*)(nullptr));
    quad_vao.AddBuffer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                       (void*)(3 * sizeof(float)));
    quad_vbo.UnBind();
    quad_vao.UnBind();
  }
  quad_vao.Bind();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  quad_vao.UnBind();
}
ShadowMappingDepthWindow::ShadowMappingDepthWindow(int width, int height,
                                                   const char* title,
                                                   GLFWmonitor* monitor,
                                                   GLFWwindow* share)
    : OpenGLCameraWindow(width, height, title, monitor, share),
      imgui_main_window_(window_, width, height) {
  glfwSetWindowUserPointer(window_, this);
}
