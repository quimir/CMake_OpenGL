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

#include "include/ShadowFrameBuffer.h"
#include "include/OpenGLException.h"
#include "include/ImGui/OpenGLLogMessage.h"

ShadowFrameBuffer::ShadowFrameBuffer(ShadowType shadow_type, GLint width,
                                     GLint height, int cascade_count)
    : FrameBuffer(width, height, GL_FRAMEBUFFER, GL_TEXTURE_2D,
                  GL_RENDERBUFFER),
      shadow_type_(shadow_type),
      cascade_count_(cascade_count) {
  try {
    Initialize();
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(e.what());
#ifdef _DEBUG
    std::cerr << e.what() << std::endl;
#endif
  }
}
void ShadowFrameBuffer::Initialize() {
  BindFrameBuffer();

  glFramebufferTexture2D(frame_buffer_type_, GL_COLOR_ATTACHMENT0,
                         texture_color_buffer_type_, texture_color_buffer_, 0);
  glFramebufferRenderbuffer(frame_buffer_type_, GL_DEPTH_STENCIL_ATTACHMENT,
                            rbo_depth_stencil_type_, rbo_depth_stencil_);

  if (frame_buffer_ != 0) {
    glDeleteFramebuffers(1, &frame_buffer_);
  }
  if (texture_color_buffer_ != 0) {
    glDeleteTextures(1, &texture_color_buffer_);
  }
  if (rbo_depth_stencil_ != 0) {
    glDeleteRenderbuffers(1, &rbo_depth_stencil_);
  }

  glGenFramebuffers(1, &frame_buffer_);
  glBindFramebuffer(frame_buffer_type_, frame_buffer_);

  if (shadow_type_ == ShadowType::kPoint) {
    texture_color_buffer_type_ = GL_TEXTURE_CUBE_MAP;
    glGenTextures(1, &texture_color_buffer_);
    glBindTexture(texture_color_buffer_type_, texture_color_buffer_);
    for (int i = 0; i < 6; ++i) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                   window_width_, window_height_, 0, GL_DEPTH_COMPONENT,
                   GL_FLOAT, nullptr);
    }
    glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_R,
                    GL_CLAMP_TO_EDGE);

    BindFrameBuffer();
    glFramebufferTexture(frame_buffer_type_, GL_DEPTH_ATTACHMENT,
                         texture_color_buffer_, 0);
  } else {
    glGenTextures(1, &texture_color_buffer_);

    if (shadow_type_ == ShadowType::kCSM) {
      texture_color_buffer_type_ = GL_TEXTURE_2D_ARRAY;
      glBindTexture(texture_color_buffer_type_, texture_color_buffer_);
      glTexImage3D(texture_color_buffer_type_, 0, GL_DEPTH_COMPONENT,
                   window_width_, window_height_, cascade_count_, 0,
                   GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MAG_FILTER,
                      GL_NEAREST);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_EDGE);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_EDGE);

      glFramebufferTexture(frame_buffer_type_, GL_DEPTH_ATTACHMENT,
                           texture_color_buffer_, 0);
    } else {
      texture_color_buffer_type_ = GL_TEXTURE_2D;
      glBindTexture(texture_color_buffer_type_, texture_color_buffer_);
      glTexImage2D(texture_color_buffer_type_, 0, GL_DEPTH_COMPONENT,
                   window_width_, window_height_, 0, GL_DEPTH_COMPONENT,
                   GL_FLOAT, nullptr);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MAG_FILTER,
                      GL_NEAREST);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_EDGE);
      glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_EDGE);

      glFramebufferTexture2D(frame_buffer_type_, GL_DEPTH_ATTACHMENT,
                             texture_color_buffer_type_, texture_color_buffer_,
                             0);
    }
  }

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw OpenGLException(LoggerSystem::Level::kWarning,
                          "Shadow framebuffer is not complete!");
  }

  UnBindFrameBuffer();
}
int ShadowFrameBuffer::GetCascadeCount() const {
  return cascade_count_;
}
void ShadowFrameBuffer::SetCascadeCount(int cascade_count) {
  cascade_count_ = cascade_count;
}
