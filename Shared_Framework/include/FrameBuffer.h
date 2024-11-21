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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_FRAMEBUFFER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_FRAMEBUFFER_H_

#include "glad/glad.h"
#include "Core/MacroDefinition.h"

/**
 * Information used to process the frame buffer. Divided into texture color 
 * buffer, frame buffer, rendering buffer. It is usually attached to 
 * OpenGLWindow and uses RenderToFramebuffer as its frame buffering logic. But 
 * it is also used independently. There is no risk of memory leaks and all 
 * memory is reclaimed via destructors.
 * 
 * Use reference:
 * @code
 * FrameBuffer frame_buffer(800,600);
 * frame_buffer.BindFrameBuffer();
 * 
 * //Build your frame buffer operations here.
 * 
 * frame_buffer.UnBind();
 * @endcode
 */
class FrameBuffer {
 public:
  FrameBuffer(GLint width, GLint height,
              GLenum frame_buffer_type = GL_FRAMEBUFFER,
              GLenum texture_color_buffer_type = GL_TEXTURE_2D,
              GLenum rbo_depth_stencil_type = GL_RENDERBUFFER);

  FrameBuffer(GLint width, GLint height, GLenum frame_buffer_type,
              GLenum texture_color_buffer_type,
              GLint texture_color_buffer_level,
              GLint texture_color_buffer_internalformat,
              GLboolean texture_color_buffer_fixed_samplelocations,
              GLenum rbo_depth_stencil_type,
              GLenum rbo_depth_stencil_type_internalformat);

  ~FrameBuffer();

  FrameBuffer(const FrameBuffer& frame_buffer) = delete;

  FrameBuffer& operator=(const FrameBuffer& frame_buffer) = delete;

  /**
   * BindFrameBuffer the frame buffer object so that subsequent operations can be 
   * performed against it.
   */
  void BindFrameBuffer() const;

  /**
   * Unbind the currently bound frame buffer object.
   */
  void UnBindFrameBuffer() const;

  void BindTextureColor() const;

  void UnBindTextureColor() const;

  void ClearColorAndDepthBit() const;

  void Resize(GLint width, GLint height);

  GLenum GetFrameBufferType() const;

  void Reset(GLint width, GLint height, GLenum frame_buffer_type,
             GLenum texture_color_buffer_type, GLenum depth_stencil_type);

  /**
   * Reset the Framebuffer target type.
   * @param frame_buffer_type must be either GL_DRAW_FRAMEBUFFER, 
   * GL_READ_FRAMEBUFFER or GL_FRAMEBUFFER.
   */
  void SetFrameBufferType(GLenum frame_buffer_type);

  GLenum GetTextureColorBufferType() const;

  void SetTextureColorBufferType(GLenum texture_color_buffer_type);

  GLenum GetRboDepthStencilType() const;

  void SetRboDepthStencilType(GLenum rbo_depth_stencil_type);

 private:
  /**
   * Initialize the frame buffer object, create color and depth template 
   * textures and associate them.
   * @param width Width of the frame buffer.
   * @param height Height of the frame buffer.
   */
  void Initialize(GLint width, GLint height);

  void Initialize(GLint width, GLint height, GLenum frame_buffer_type,
                  GLenum texture_color_buffer_type,
                  GLint texture_color_buffer_level,
                  GLint texture_color_buffer_internalformat,
                  GLboolean texture_color_buffer_fixed_samplelocations,
                  GLenum rbo_depth_stencil_type,
                  GLenum rbo_depth_stencil_type_internalformat);

  void Cleanup();

 protected:
  GLint window_width_, window_height_;
  GLuint frame_buffer_;          // Frame buffer object ID.
  GLenum frame_buffer_type_;     // Framebuffer target type.
  GLuint texture_color_buffer_;  // Color texture buffer ID.
  GLenum texture_color_buffer_type_;
  GLuint rbo_depth_stencil_;  // Depth templates render buffer ids.
  GLenum rbo_depth_stencil_type_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_FRAMEBUFFER_H_
