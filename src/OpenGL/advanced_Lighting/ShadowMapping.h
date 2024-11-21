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

#ifndef CMAKE_OPEN_SRC_ADVANCED_LIGHTING_SHADOWMAPPING_H_
#define CMAKE_OPEN_SRC_ADVANCED_LIGHTING_SHADOWMAPPING_H_

#include "Shader.h"
#include "VertexArray.h"
#include "Buffers.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "Core/Rect.h"

class ShadowMapping {
 public:
  ShadowMapping(GLint window_width, GLint window_height, GLint shadow_width,
                GLint shadow_height);

  Shader& GetShadowMappingShader();

  Shader& GetSimpleDepthShader();

  Shader& GetDebugDepthQuadShader();

  VertexArray& GetPlaneVao();

  Buffers& GetPlaneVbo();

  TextureLoader& GetTextureLoader();

  void Bind(float near_plane, float far_plane, glm::vec3& light_pos,
            Camera& camera);

  void ResetWindow(Rect value);

  bool GetOpenShadow() const;

  void SetOpenShadow(bool new_open_shadow);

  float GetShadowBiasValue() const;
  
  void SetShadowBiasValue(float shadow_bias_value);
  
  float GetMaxLightDistance() const;
  
  void SetMaxLightDistance(float max_light_distance);
  
  float GetMinRadius() const;
  
  void SetMinRadius(float min_radius);
  
  float GetMaxRadius() const;
  
  void SetMaxRadius(float max_radius);
  
  float& RGetShadowBiasValue();
  
  float& RGetMaxLightDistance();
  
  float& RGetMinRadius();
  
  float& RGetMaxRadius();
  
  ~ShadowMapping();

 private:
  void RenderCube();

  void RenderScene(const Shader& shader);

  void RenderQuad();

 private:
  GLint shadow_width_, shadow_height_;
  GLint window_width_, window_height_;
  Shader *shadow_mapping_shader_, *simple_depth_shader_,
      *debug_depth_quad_shader_;
  VertexArray plane_vao_;
  Buffers plane_vbo_;
  TextureLoader* texture_loader_;
  GLuint depth_map_fbo_, depth_map_;

  bool open_shadow_;

  float shadow_bias_value_, max_light_distance_, min_radius_, max_radius_;
};

#endif  //CMAKE_OPEN_SRC_ADVANCED_LIGHTING_SHADOWMAPPING_H_
