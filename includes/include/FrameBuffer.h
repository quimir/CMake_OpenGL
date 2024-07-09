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

class FrameBuffer {
 public:
  FrameBuffer(GLint width, GLint height);
  
  ~FrameBuffer();
  
  FrameBuffer(const FrameBuffer& frame_buffer)=delete;
  
  FrameBuffer& operator=(const FrameBuffer& frame_buffer)=delete;
  
  void Bind()const;
  
  void UnBind()const;
  
  GLuint GetTextureColorBuffer() const;
  
  GLuint GetFrameBuffer() const;
  
  void Resize(GLint width,GLint height);

 private:
  void Initialize(GLint width, GLint height);
  
  void Cleanup();
  
 private:
  GLuint frame_buffer_;
  GLuint texture_color_buffer_;
  GLuint rbo_depth_stencil_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_FRAMEBUFFER_H_
