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

#include "include/VertexArray.h"
#include "include/LoggerSystem.h"
#include "include/OpenGLStateManager.h"

VertexArray::VertexArray() : vao_id_(0) {
  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }
  glGenVertexArrays(1, &vao_id_);
}
VertexArray::~VertexArray() {
  LoggerSystem::GetInstance().Log(
      LoggerSystem::Level::kInfo,
      "VertexArray with ID " + std::to_string(vao_id_) + " is being destroyed");
  glDeleteVertexArrays(1, &vao_id_);
}
void VertexArray::Bind() {
    glBindVertexArray(vao_id_);
}
void VertexArray::UnBind() {
    glBindVertexArray(0);
}
void VertexArray::AddBuffer(GLuint index, GLint size, GLenum type,
                            GLboolean normalized, GLsizei stride,
                            const void* pointer) const {
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);
}

void VertexArray::AddIntBuffer(GLuint index, GLint size, GLenum type,
                               GLsizei stride, const void* pointer) const {
  glVertexAttribIPointer(index, size, type, stride, pointer);
  glEnableVertexAttribArray(index);
}
void VertexArray::ReGenVertexArrays() {
  if (vao_id_ != 0) {
    glDeleteVertexArrays(1, &vao_id_);
  }
  glGenVertexArrays(1, &vao_id_);
}
GLuint VertexArray::GetVaoId() const {
  return vao_id_;
}
void VertexArray::AddLongBuffer(GLuint index, GLint size, GLenum type,
                                GLsizei stride, const void* pointer) const {
  glVertexAttribLPointer(index, size, type, stride, pointer);
  glEnableVertexAttribArray(index);
}
