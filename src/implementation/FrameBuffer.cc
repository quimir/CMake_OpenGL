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

#include "include/FrameBuffer.h"
#include "include/LoggerSystem.h"
GLuint FrameBuffer::GetTextureColorBuffer() const {
  return texture_color_buffer_;
}
FrameBuffer::FrameBuffer(GLint width, GLint height)
    : frame_buffer_(0), texture_color_buffer_(0), rbo_depth_stencil_(0) {
  if (glGetString(GL_VERSION) == nullptr) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "OpenGL was not built successfully when building the frame buffer "
        "object. Please rebuild OpenGL and try again or check the log file for "
        "an OpenGL build error.");
    throw std::runtime_error(
        "OpenGL was not built successfully when building the frame buffer "
        "object. Please rebuild OpenGL and try again or check the log file for "
        "an OpenGL build error.");
  }
  Initialize(width, height);
}
FrameBuffer::~FrameBuffer() {
  Cleanup();
}
void FrameBuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer_);
}
void FrameBuffer::UnBind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Initialize(GLint width, GLint height) {
  // Generate frame buffer object
  glGenFramebuffers(1, &frame_buffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

  // Generate color texture buffer
  glGenTextures(1, &texture_color_buffer_);
  glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_color_buffer_, 0);

  // Generate depth template render buffer
  glGenRenderbuffers(1, &rbo_depth_stencil_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo_depth_stencil_);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                    "Framebuffer is not complete!");
    throw std::runtime_error("Framebuffer is not complete!");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Cleanup() {
  glDeleteFramebuffers(1, &frame_buffer_);
  glDeleteTextures(1, &texture_color_buffer_);
  glDeleteRenderbuffers(1, &rbo_depth_stencil_);
}
void FrameBuffer::Resize(GLint width, GLint height) {
  Cleanup();
  Initialize(width, height);
}
GLuint FrameBuffer::GetFrameBuffer() const {
  return frame_buffer_;
}
