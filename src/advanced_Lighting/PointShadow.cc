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

#include "PointShadow.h"
#include "include/FilePathSystem.h"
PointShadow::PointShadow(GLint window_width, GLint window_height,
                         GLint shadow_width, GLint shadow_height)
    : window_width_(window_width),
      window_height_(window_height),
      shadow_width_(shadow_width),
      shadow_height_(shadow_height),
      open_shadow_(true),
      bias_value_(0.05) {
  Initialize();
}
void PointShadow::Initialize() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  point_shadow_shader_ = new Shader("point_shadow.vert", "point_shadow.frag");
  simple_depth_shader_ =
      new Shader("point_shadow_depth.vert", "point_shadow_depth.frag",
                 "point_shadow_depth.geom");

  texture_loader_ = new TextureLoader(
      TextureLoader::Type::kTexture2D,
      FilePathSystem::GetInstance().GetPath("resources/textures/wood.png"),
      GL_REPEAT, GL_REPEAT);

  shadow_frame_buffer_ = new ShadowFrameBuffer(
      ShadowFrameBuffer::ShadowType::kPoint, shadow_width_, shadow_height_);

//  glGenFramebuffers(1, &depth_map_fbo_);
//  glGenTextures(1, &depth_cube_map_);
//  glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube_map_);
//  for (unsigned int i = 0; i < 6; ++i) {
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
//                 shadow_width_, shadow_height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
//                 nullptr);
//  }
//  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//  // attach depth texture as FBO's depth buffer
//  glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo_);
//  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cube_map_, 0);
//  glDrawBuffer(GL_NONE);
//  glReadBuffer(GL_NONE);
//  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  point_shadow_shader_->Use();
  point_shadow_shader_->SetInt("diffuse_texture", 0);
  point_shadow_shader_->SetInt("depth_map", 1);
  point_shadow_shader_->UnUse();
}
Shader& PointShadow::GetShader() {
  return *point_shadow_shader_;
}
Shader& PointShadow::GetSimpleDepthShader() {
  return *simple_depth_shader_;
}
TextureLoader& PointShadow::GetTextureLoader() {
  return *texture_loader_;
}
GLuint PointShadow::GetDepthMapFbo() {
  return depth_map_fbo_;
}
GLuint PointShadow::GetDepthCubeMap() {
  return depth_cube_map_;
}
PointShadow::~PointShadow() {
  delete point_shadow_shader_;
  delete simple_depth_shader_;
  delete texture_loader_;
}
void PointShadow::Bind(float near_plane, float far_plane, glm::vec3& light_pos,
                       Camera& camera) {
  //light_pos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 shadow_projection = glm::perspective(
      glm::radians(90.0f), (float)shadow_width_ / (float)shadow_height_,
      near_plane, far_plane);
  std::vector<glm::mat4> shadow_transforms;
  shadow_transforms.push_back(
      shadow_projection * glm::lookAt(light_pos,
                                      light_pos + glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f)));
  shadow_transforms.push_back(
      shadow_projection * glm::lookAt(light_pos,
                                      light_pos + glm::vec3(-1.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f)));
  shadow_transforms.push_back(
      shadow_projection * glm::lookAt(light_pos,
                                      light_pos + glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f)));
  shadow_transforms.push_back(
      shadow_projection * glm::lookAt(light_pos,
                                      light_pos + glm::vec3(0.0f, -1.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, -1.0f)));

  shadow_transforms.push_back(
      shadow_projection * glm::lookAt(light_pos,
                                      light_pos + glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f)));

  shadow_transforms.push_back(
      shadow_projection * glm::lookAt(light_pos,
                                      light_pos + glm::vec3(0.0f, 0.0f, -1.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f)));

  glViewport(0, 0, shadow_width_, shadow_height_);
  shadow_frame_buffer_->BindFrameBuffer();
  //glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo_);
  glClear(GL_DEPTH_BUFFER_BIT);
  simple_depth_shader_->Use();
  for (unsigned int i = 0; i < 6; ++i) {
    simple_depth_shader_->SetMat4("shadow_matrices[" + std::to_string(i) + "]",
                                  shadow_transforms[i]);
  }
  simple_depth_shader_->SetFloat("far_plane", far_plane);
  simple_depth_shader_->SetVec3("light_pos", light_pos);
  RenderScene(*simple_depth_shader_);
  shadow_frame_buffer_->UnBindFrameBuffer();
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
  simple_depth_shader_->UnUse();

  glViewport(0, 0, window_width_, window_height_);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  point_shadow_shader_->Use();
  glm::mat4 projection =
      camera.GetProjectionMatrix(window_width_, window_height_);
  glm::mat4 view = camera.GetViewMatrix();

  point_shadow_shader_->SetMat4("projection", projection);
  point_shadow_shader_->SetMat4("view", view);
  point_shadow_shader_->SetVec3("light_pos", light_pos);
  point_shadow_shader_->SetVec3("view_pos", camera.GetPosition());
  point_shadow_shader_->SetBool("open_shadow", open_shadow_);
  point_shadow_shader_->SetFloat("far_plane", far_plane);
  point_shadow_shader_->SetFloat("bias_value", bias_value_);
  texture_loader_->Bind(GL_TEXTURE0);
  glActiveTexture(GL_TEXTURE1);
  shadow_frame_buffer_->BindTextureColor();
//  glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube_map_);
  RenderScene(*point_shadow_shader_);
}
bool PointShadow::GetOpenShadow() const {
  return open_shadow_;
}
void PointShadow::SetOpenShadow(bool value) {
  if (value == open_shadow_)
    return;

  open_shadow_ = value;
}
void PointShadow::RenderScene(Shader& shader) {
  shader.Use();
  glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
  shader.SetMat4("model", model);
  glDisable(GL_CULL_FACE);
  shader.SetInt("reverse_normals", 1);
  RenderCube();
  shader.SetInt("reverse_normals", 0);
  glEnable(GL_CULL_FACE);

  model =
      glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -3.5f, 0.0f)),
                 glm::vec3(0.5f));
  shader.SetMat4("model", model);
  RenderCube();

  model =
      glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0f)),
                 glm::vec3(0.75f));
  shader.SetMat4("model", model);
  RenderCube();

  model =
      glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -1.0f, 0.0)),
                 glm::vec3(0.5f));
  shader.SetMat4("model", model);
  RenderCube();

  model =
      glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 1.0f, 1.5f)),
                 glm::vec3(0.5f));
  shader.SetMat4("model", model);
  RenderCube();

  model = glm::scale(
      glm::rotate(
          glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.0f, -3.0f)),
          glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
      glm::vec3(0.75f));
  shader.SetMat4("model", model);
  RenderCube();
  shader.UnUse();
}
void PointShadow::RenderCube() {
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
    cube_vbo.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);
    cube_vao.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(nullptr));
    cube_vao.AddBuffer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(3 * sizeof(float)));
    cube_vao.AddBuffer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                       (void*)(6 * sizeof(float)));

    cube_vao.UnBind();
    cube_vbo.UnBind();
  }

  cube_vao.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  cube_vao.UnBind();
}
void PointShadow::ResetWindow(Rect value) {
  window_width_ = value.GetWidth();
  window_height_ = value.GetHeight();
}
float PointShadow::GetBiasValue() const {
  return bias_value_;
}
void PointShadow::SetBiasValue(float bias_value) {
  bias_value_ = bias_value;
}
float& PointShadow::RGetBiasValue() {
  return bias_value_;
}
