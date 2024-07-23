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
VertexArray::VertexArray() : vao_id_(0), binding_state_(false) {
  if (glGetString(GL_VERSION) == nullptr) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "OpenGL was not initialized while building the VertexArray.");
    std::runtime_error(
        "OpenGL was not initialized while building the VertexArray.");
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
  if (!binding_state_) {
    glBindVertexArray(vao_id_);
    binding_state_ = true;
  }
}
void VertexArray::UnBind() {
  if (binding_state_) {
    glBindVertexArray(0);
    binding_state_ = false;
  }
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
const GLuint VertexArray::GetVaoId() const {
  return vao_id_;
}
void VertexArray::AddLongBuffer(GLuint index, GLint size, GLenum type,
                                GLsizei stride, const void* pointer) const {
  glVertexAttribLPointer(index, size, type, stride, pointer);
  glEnableVertexAttribArray(index);
}
bool VertexArray::IsBindingState() const {
  return binding_state_;
}
