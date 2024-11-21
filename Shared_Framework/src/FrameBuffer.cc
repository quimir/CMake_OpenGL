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

#include "FrameBuffer.h"
#include "LoggerSystem.h"
#include "OpenGLException.h"
#include "OpenGLStateManager.h"
#include "ImGui/OpenGLLogMessage.h"

FrameBuffer::FrameBuffer(GLint width, GLint height, GLenum frame_buffer_type,
                         GLenum texture_color_buffer_type,
                         GLenum rbo_depth_stencil_type)
    : window_width_(width),
      window_height_(height),
      frame_buffer_(0),
      texture_color_buffer_(0),
      rbo_depth_stencil_(0),
      frame_buffer_type_(frame_buffer_type),
      texture_color_buffer_type_(texture_color_buffer_type),
      rbo_depth_stencil_type_(rbo_depth_stencil_type) {
  try {
    Initialize(this->window_width_, this->window_height_);
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(
        std::string("An error occurred while creating the frame buffer. The "
                    "cause is: ") +
        e.what());
#ifdef _DEBUG
    std::cerr << e.what() << std::endl;
#endif
  }
}
FrameBuffer::~FrameBuffer() {
  Cleanup();
}
void FrameBuffer::BindFrameBuffer() const {
  glBindFramebuffer(frame_buffer_type_, this->frame_buffer_);
}
void FrameBuffer::UnBindFrameBuffer() const {
  glBindFramebuffer(frame_buffer_type_, 0);
}
void FrameBuffer::Initialize(GLint width, GLint height) {
  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    throw OpenGLException(
        LoggerSystem::Level::kWarning,
        "OpenGL was not built successfully when building the frame buffer "
        "object. Please rebuild OpenGL and try again or check the log file for "
        "an OpenGL build error.");
  }

  if (width < 0 || height < 0) {
    throw OpenGLException(
        LoggerSystem::Level::kWarning,
        "Do not allow the input screen length or width less than 0, will exit "
        "the frame buffer system.");
  }

  // Generate frame buffer object
  glGenFramebuffers(1, &frame_buffer_);
  glBindFramebuffer(frame_buffer_type_, frame_buffer_);

  // Generate color texture buffer
  glGenTextures(1, &texture_color_buffer_);
  glBindTexture(texture_color_buffer_type_, texture_color_buffer_);
  glTexImage2D(texture_color_buffer_type_, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_S,
                  GL_CLAMP_TO_EDGE);
  glTexParameteri(texture_color_buffer_type_, GL_TEXTURE_WRAP_T,
                  GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(frame_buffer_type_, GL_COLOR_ATTACHMENT0,
                         texture_color_buffer_type_, texture_color_buffer_, 0);

  // Generate depth template render buffer
  glGenRenderbuffers(1, &rbo_depth_stencil_);
  glBindRenderbuffer(rbo_depth_stencil_type_, rbo_depth_stencil_);
  glRenderbufferStorage(rbo_depth_stencil_type_, GL_DEPTH24_STENCIL8, width,
                        height);
  glFramebufferRenderbuffer(frame_buffer_type_, GL_DEPTH_STENCIL_ATTACHMENT,
                            rbo_depth_stencil_type_, rbo_depth_stencil_);

  // now that we actually created the framebuffer and added all attachments we
  // want to check if it is actually complete now
  if (glCheckFramebufferStatus(frame_buffer_type_) != GL_FRAMEBUFFER_COMPLETE) {
    throw OpenGLException(LoggerSystem::Level::kWarning,
                          "Framebuffer is not complete!");
  }

  glBindFramebuffer(frame_buffer_type_, 0);
}
void FrameBuffer::Cleanup() {
  glDeleteFramebuffers(1, &frame_buffer_);
  glDeleteTextures(1, &texture_color_buffer_);
  glDeleteRenderbuffers(1, &rbo_depth_stencil_);
}
void FrameBuffer::Resize(GLint width, GLint height) {
  Initialize(width, height);
}
GLenum FrameBuffer::GetFrameBufferType() const {
  return frame_buffer_type_;
}
void FrameBuffer::SetFrameBufferType(GLuint frame_buffer_type) {
  frame_buffer_type_ = frame_buffer_type;
  ClearColorAndDepthBit();
  Cleanup();
  Initialize(this->window_width_, this->window_height_);
}
GLenum FrameBuffer::GetTextureColorBufferType() const {
  return texture_color_buffer_type_;
}
void FrameBuffer::SetTextureColorBufferType(GLenum texture_color_buffer_type) {
  texture_color_buffer_type_ = texture_color_buffer_type;
}
GLenum FrameBuffer::GetRboDepthStencilType() const {
  return rbo_depth_stencil_type_;
}
void FrameBuffer::SetRboDepthStencilType(GLenum rbo_depth_stencil_type) {
  rbo_depth_stencil_type_ = rbo_depth_stencil_type;
}
void FrameBuffer::BindTextureColor() const {
  glBindTexture(texture_color_buffer_type_, texture_color_buffer_);
}
void FrameBuffer::UnBindTextureColor() const {
  glBindTexture(texture_color_buffer_type_, 0);
}
void FrameBuffer::ClearColorAndDepthBit() const {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void FrameBuffer::Reset(GLint width, GLint height, GLenum frame_buffer_type,
                        GLenum texture_color_buffer_type,
                        GLenum depth_stencil_type) {
  try {
    Cleanup();
    this->window_width_ = width;
    this->window_height_ = height;
    this->frame_buffer_type_ = frame_buffer_type;
    this->texture_color_buffer_type_ = texture_color_buffer_type;
    this->rbo_depth_stencil_type_ = depth_stencil_type;
    this->frame_buffer_ = 0;
    this->texture_color_buffer_ = 0;
    this->rbo_depth_stencil_ = 0;
    Initialize(this->window_width_, this->window_height_);
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(
        std::string("An error occurred while resetting the frame buffer. The "
                    "cause is: ") +
        e.what());

    exit(0);
  }
}
FrameBuffer::FrameBuffer(GLint width, GLint height, GLenum frame_buffer_type,
                         GLenum texture_color_buffer_type,
                         GLint texture_color_buffer_level,
                         GLint texture_color_buffer_internalformat,
                         GLboolean texture_color_buffer_fixed_samplelocations,
                         GLenum rbo_depth_stencil_type,
                         GLenum rbo_depth_stencil_type_internalformat)
    : window_width_(width),
      window_height_(height),
      frame_buffer_type_(frame_buffer_type),
      texture_color_buffer_type_(texture_color_buffer_type),
      rbo_depth_stencil_type_(rbo_depth_stencil_type) {
  try {
    Initialize(width, height, frame_buffer_type, texture_color_buffer_type,
               texture_color_buffer_level, texture_color_buffer_internalformat,
               texture_color_buffer_fixed_samplelocations,
               rbo_depth_stencil_type, rbo_depth_stencil_type_internalformat);
  } catch (Exception& e) {
    OpenGLLogMessage::GetInstance().AddLog(
        std::string("An error occurred while creating the frame buffer. The "
                    "cause is: ") +
        e.what());
    exit(0);
  }
}
void FrameBuffer::Initialize(
    GLint width, GLint height, GLenum frame_buffer_type,
    GLenum texture_color_buffer_type, GLint texture_color_buffer_level,
    GLint texture_color_buffer_internalformat,
    GLboolean texture_color_buffer_fixed_samplelocations,
    GLenum rbo_depth_stencil_type,
    GLenum rbo_depth_stencil_type_internalformat) {
  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    throw OpenGLException(
        LoggerSystem::Level::kWarning,
        "OpenGL was not built successfully when building the frame buffer "
        "object. Please rebuild OpenGL and try again or check the log file for "
        "an OpenGL build error.");
  }

  if (width < 0 || height < 0) {
    throw OpenGLException(
        LoggerSystem::Level::kWarning,
        "Do not allow the input screen length or width less than 0, will exit "
        "the frame buffer system.");
  }

  glGenFramebuffers(1, &frame_buffer_);
  glBindFramebuffer(frame_buffer_type, frame_buffer_);

  glGenTextures(1, &texture_color_buffer_);
  glBindTexture(texture_color_buffer_type, texture_color_buffer_);
  glTexImage2DMultisample(texture_color_buffer_type, texture_color_buffer_level,
                          texture_color_buffer_internalformat, width, height,
                          texture_color_buffer_fixed_samplelocations);
  glBindTexture(texture_color_buffer_type, 0);
  glFramebufferTexture2D(frame_buffer_type, GL_COLOR_ATTACHMENT0,
                         texture_color_buffer_type, texture_color_buffer_, 0);

  glGenRenderbuffers(1, &rbo_depth_stencil_);
  glBindRenderbuffer(rbo_depth_stencil_type, rbo_depth_stencil_);
  glRenderbufferStorageMultisample(
      rbo_depth_stencil_type, texture_color_buffer_level,
      rbo_depth_stencil_type_internalformat, width, height);
  glBindRenderbuffer(rbo_depth_stencil_type, 0);
  glFramebufferRenderbuffer(frame_buffer_type, GL_DEPTH_STENCIL_ATTACHMENT,
                            rbo_depth_stencil_type, rbo_depth_stencil_);

  if (glCheckFramebufferStatus(frame_buffer_type_) != GL_FRAMEBUFFER_COMPLETE) {
    throw OpenGLException(LoggerSystem::Level::kWarning,
                          "Framebuffer is not complete!");
  }

  glBindFramebuffer(frame_buffer_type_, 0);
}
