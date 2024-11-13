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

#ifndef CMAKE_OPEN_SRC_ADVANCED_LIGHTING_POINTSHADOW_H_
#define CMAKE_OPEN_SRC_ADVANCED_LIGHTING_POINTSHADOW_H_

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "include/Shader.h"
#include "include/VertexArray.h"
#include "include/Buffers.h"
#include "include/FrameBuffer.h"
#include "include/TextureLoader.h"
#include "include/Camera.h"
#include "include/Core/Rect.h"
#include "include/ShadowFrameBuffer.h"

class PointShadow {
 public:
  explicit PointShadow(GLint window_width, GLint window_height,
                       GLint shadow_width, GLint shadow_height);

  Shader& GetShader();

  Shader& GetSimpleDepthShader();

  TextureLoader& GetTextureLoader();

  GLuint GetDepthMapFbo();

  GLuint GetDepthCubeMap();

  ~PointShadow();

  void Bind(float near_plane, float far_plane, glm::vec3& light_pos,
            Camera& camera);

  bool GetOpenShadow() const;

  void SetOpenShadow(bool value);

  void ResetWindow(Rect value);

  float GetBiasValue() const;

  float& RGetBiasValue();

  void SetBiasValue(float bias_value);

 private:
  void Initialize();

  void RenderScene(Shader& shader);

  void RenderCube();

 private:
  GLint shadow_width_, shadow_height_;

  GLint window_width_, window_height_;

  Shader *point_shadow_shader_, *simple_depth_shader_;

  GLuint depth_map_fbo_, depth_cube_map_;

  TextureLoader* texture_loader_;

  bool open_shadow_;

  float bias_value_;

  ShadowFrameBuffer* shadow_frame_buffer_;
};

#endif  //CMAKE_OPEN_SRC_ADVANCED_LIGHTING_POINTSHADOW_H_
