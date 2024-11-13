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
#include "include/OpenGLException.h"
#include "include/OpenGLStateManager.h"

VertexArray::VertexArray(GLsizei n) : vao_id_(0), n_(n), has_data_(false) {
  try {
    if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
      throw OpenGLException(
          LoggerSystem::Level::kError,
          "Serious error! Initialize OpenGL before building shaders!");
    }
    glGenVertexArrays(n_, &vao_id_);
    if (vao_id_ == 0) {
      throw OpenGLException(LoggerSystem::Level::kError,
                            "Unable to generate vertex data in OpenGL.");
    }
  } catch (OpenGLException& e) {
    std::cerr << "An error occurred while creating Vertex Array. Cause: "
              << e.what() << std::endl;
    exit(0);
  }
}
VertexArray::~VertexArray() {
  glDeleteVertexArrays(n_, &vao_id_);
}
void VertexArray::Bind() const {
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
GLuint VertexArray::GetVaoId() const {
  return vao_id_;
}
void VertexArray::AddLongBuffer(GLuint index, GLint size, GLenum type,
                                GLsizei stride, const void* pointer) const {
  glVertexAttribLPointer(index, size, type, stride, pointer);
  glEnableVertexAttribArray(index);
}
void VertexArray::ResetVertexArrays(GLsizei n) {
  try {
    Cleanup();
    n_ = n;
    glGenVertexArrays(n_, &vao_id_);
    if (vao_id_ == 0) {
      throw OpenGLException(LoggerSystem::Level::kError,
                            "Reset vertex array data error!");
    }
  } catch (OpenGLException& e) {
    std::cerr << e.what() << std::endl;
    exit(0);
  }
}
void VertexArray::Cleanup() {
  if (vao_id_ != 0) {
    glDeleteVertexArrays(n_, &vao_id_);
    vao_id_ = 0;
    n_ = 1;
    has_data_ = false;
  }
}
GLsizei VertexArray::GetN() const {
  return n_;
}
void VertexArray::AddBuffer(GLuint index, GLint size, GLenum type,
                            GLboolean normalized, GLsizei stride,
                            const void* pointer) {
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);
  has_data_ = true;
}
void VertexArray::AddIntBuffer(GLuint index, GLint size, GLenum type,
                               GLsizei stride, const void* pointer) {
  glVertexAttribIPointer(index, size, type, stride, pointer);
  glEnableVertexAttribArray(index);
  has_data_ = true;
}
void VertexArray::AddLongBuffer(GLuint index, GLint size, GLenum type,
                                GLsizei stride, const void* pointer) {
  glVertexAttribLPointer(index, size, type, stride, pointer);
  glEnableVertexAttribArray(index);
  has_data_ = true;
}
bool VertexArray::IsEmpty() const {
  return !((vao_id_ != 0) && (has_data_));
}
