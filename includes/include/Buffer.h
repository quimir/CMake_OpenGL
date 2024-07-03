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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXBUFFER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXBUFFER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Buffer {
 public:
  Buffer(GLenum type = GL_ARRAY_BUFFER);

  ~Buffer();

  GLenum GetType() const;

  void SetType(GLenum type);

  void Bind() const;

  void UnBind() const;

  void SetData(const void *data, GLsizeiptr size, GLenum usage) const;

  template<typename T>
  void SetData(const std::vector<T> &data, GLenum usage) const;

 private:
  GLuint buffer_id_;
  GLenum type_;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXBUFFER_H_
