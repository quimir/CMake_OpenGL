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

/**
 * Information used to process the frame buffer. Divided into texture color 
 * buffer, frame buffer, rendering buffer. It is usually attached to 
 * OpenGLWindow and uses RenderToFramebuffer as its frame buffering logic. But 
 * it is also used independently. There is no risk of memory leaks and all 
 * memory is reclaimed via destructors.
 * 
 * Use reference:
 * FrameBuffer frame_buffer(800,600);
 * frame_buffer.Bind();
 * 
 * //Build your frame buffer operations here.
 * 
 * frame_buffer.UnBind();
 */
class FrameBuffer {
 public:
  FrameBuffer(GLint width, GLint height);
  
  ~FrameBuffer();
  
  FrameBuffer(const FrameBuffer& frame_buffer)=delete;
  
  FrameBuffer& operator=(const FrameBuffer& frame_buffer)=delete;
  
  /**
   * Bind the frame buffer object so that subsequent operations can be 
   * performed against it.
   */
  void Bind()const;
  
  /**
   * Unbind the currently bound frame buffer object.
   */
  void UnBind()const;
  
  GLuint GetTextureColorBuffer() const;
  
  GLuint GetFrameBuffer() const;
  
  /**
   * Resize the frame buffer object and reinitialize it.
   * @param width New frame buffer width.
   * @param height New frame buffer height.
   */
  void Resize(GLint width,GLint height);

 private:
  /**
   * Initialize the frame buffer object, create color and depth template 
   * textures and associate them.
   * @param width Width of the frame buffer.
   * @param height Height of the frame buffer.
   */
  void Initialize(GLint width, GLint height);
  
  void Cleanup();
  
 private:
  GLuint frame_buffer_; // Frame buffer object ID.
  GLuint texture_color_buffer_;// Color texture buffer ID.
  GLuint rbo_depth_stencil_;// Depth templates render buffer ids.
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_FRAMEBUFFER_H_
