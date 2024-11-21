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

#include "Experimental/SkyBox.h"
#include "FilePathSystem.h"
#include "LoadImage.h"
SkyBox::SkyBox(const std::vector<std::string>& faces_path,
               bool gamma_correction, float gamma_value) {
  if (faces_path.empty()) {
    return;
  }
  
  sky_box_shader_ = new Shader(
      FilePathSystem::GetInstance().GetResourcesPath("glsl/sky_box.vert"),
      FilePathSystem::GetInstance().GetResourcesPath("glsl/sky_box.frag"));

  float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  sky_box_vao_.Bind();
  sky_box_vbo_.Bind();
  sky_box_vbo_.SetData(&skyboxVertices, sizeof(skyboxVertices), GL_STATIC_DRAW);
  sky_box_vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  sky_box_vbo_.UnBind();
  sky_box_vao_.UnBind();

  sky_box_texture_ =
      new TextureLoader(TextureLoader::Type::kCubeMap, faces_path,
                        GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                        GL_LINEAR, GL_LINEAR, gamma_correction, gamma_value);
  sky_box_shader_->Use();
  sky_box_shader_->SetInt("skybox", 0);
  sky_box_shader_->UnUse();
}
void SkyBox::Bind(glm::mat4 projection, glm::mat4 view) {
  glDepthFunc(GL_LEQUAL);
  sky_box_shader_->Use();
  view = glm::mat4(glm::mat3(view));
  sky_box_shader_->SetMat4("projection", projection);
  sky_box_shader_->SetMat4("view", view);

  sky_box_vao_.Bind();
  sky_box_texture_->Bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  sky_box_vao_.UnBind();
  sky_box_shader_->UnUse();
  glDepthFunc(GL_LESS);
}
SkyBox::~SkyBox() {
  if (nullptr != sky_box_shader_)
    delete sky_box_shader_;
  if (nullptr != sky_box_texture_)
    delete sky_box_texture_;
}
