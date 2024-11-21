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

#include "Buffers.h"
#include "LoggerSystem.h"
#include "Model/MeshData.h"
#include "OpenGLException.h"
#include "OpenGLStateManager.h"
#include "ImGui/OpenGLLogMessage.h"

using namespace model;

Buffers::Buffers(GLsizei n, GLenum type)
    : type_(type), buffer_id_(0), n_(n), has_data_(false) {
  try {
    if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
      throw OpenGLException(
          LoggerSystem::Level::kError,
          "Serious error! Initialize OpenGL before building shaders!");
    }
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(e.what());
    exit(0);
  }
  glGenBuffers(n_, &buffer_id_);
}
Buffers::~Buffers() {
  glDeleteBuffers(n_, &buffer_id_);
}
void Buffers::Bind() const {
  glBindBuffer(this->type_, buffer_id_);
}
void Buffers::UnBind() const {
  glBindBuffer(type_, 0);
}
void Buffers::SetData(const void* data, GLsizeiptr size, GLenum usage) const {
  if (OpenGLStateManager::GetInstance().CheckOpenGLVersion(4, 5)) {
    glNamedBufferData(buffer_id_, size, data, usage);
  } else {
    glBufferData(type_, size, data, usage);
  }
}
GLenum Buffers::GetType() const {
  return type_;
}
void Buffers::ReSetType(GLenum type) {
  type_ = type;
}
void Buffers::ResetBuffers(GLsizei n, GLenum type) {
  if (type != UINT_MAX) {
    type_ = type;
  }
  if (buffer_id_ != 0) {
    glDeleteBuffers(1, &buffer_id_);
    buffer_id_ = 0;
    has_data_ = false;
  }
  n_ = n;
  glGenBuffers(n_, &buffer_id_);
}
GLsizei Buffers::GetN() const {
  return n_;
}
void Buffers::SetData(const void* data, GLsizeiptr size, GLenum usage) {
  if (OpenGLStateManager::GetInstance().CheckOpenGLVersion(4, 5)) {
    glNamedBufferData(buffer_id_, size, data, usage);
  } else {
    glBufferData(type_, size, data, usage);
  }
  has_data_ = true;
}
bool Buffers::IsEmpty() const {
  return !((buffer_id_ != 0) && (has_data_));
}
void Buffers::GetBufferParameteriv(GLenum value, GLint* data) {
  if (OpenGLStateManager::GetInstance().CheckOpenGLVersion(4, 5)) {
    glGetNamedBufferParameteriv(buffer_id_, value, data);
  } else {
    glGetBufferParameteriv(type_, value, data);
  }
}

template <typename T>
void Buffers::SetData(const std::vector<T>& data, GLenum usage) {
  if (OpenGLStateManager::GetInstance().CheckOpenGLVersion(4, 5)) {
    glNamedBufferData(buffer_id_, data.size() * sizeof(T), data.data(), usage);
  } else {
    glBufferData(type_, data.size() * sizeof(T), data.data(), usage);
  }
  has_data_ = true;
}
template <typename T>
void Buffers::SetData(const std::vector<T>& data, GLenum usage) const {
  if (OpenGLStateManager::GetInstance().CheckOpenGLVersion(4, 5)) {
    glNamedBufferData(buffer_id_, data.size() * sizeof(T), data.data(), usage);
  } else {
    glBufferData(type_, data.size() * sizeof(T), data.data(), usage);
  }
}

template void Buffers::SetData<struct meshdata::Vertex>(
    const std::vector<meshdata::Vertex>& data, GLenum usage) const;

template void Buffers::SetData<unsigned int>(
    const std::vector<unsigned int>& data, GLenum usage) const;

template void Buffers::SetData<struct meshdata::Vertex>(
    const std::vector<meshdata::Vertex>& data, GLenum usage);

template void Buffers::SetData<unsigned int>(
    const std::vector<unsigned int>& data, GLenum usage);