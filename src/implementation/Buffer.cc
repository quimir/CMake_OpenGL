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

#include "include/Buffers.h"
#include "include/LoggerSystem.h"
#include "include/Model/MeshData.h"
#include "include/OpenGLStateManager.h"

Buffers::Buffers(GLenum type)
    : type_(type), buffer_id_(0) {
  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }
  glGenBuffers(1, &buffer_id_);
}
Buffers::~Buffers() {
  glDeleteBuffers(1, &buffer_id_);
}
void Buffers::Bind() {
    glBindBuffer(this->type_, buffer_id_);
}
void Buffers::UnBind() {
    glBindBuffer(type_, 0);
}
void Buffers::SetData(const void* data, GLsizeiptr size, GLenum usage) const {
  glBufferData(type_, size, data, usage);
}
GLenum Buffers::GetType() const {
  return type_;
}
void Buffers::ReSetType(GLenum type) {
  type_ = type;
}
void Buffers::ReGenBuffers(GLenum type) {
  if (type != UINT_MAX) {
    type_ = type;
  }
  if (buffer_id_ != 0) {
    glDeleteBuffers(1, &buffer_id_);
  }
  glGenBuffers(1, &buffer_id_);
}
template <typename T>
void Buffers::SetData(const std::vector<T>& data, GLenum usage) const {
  glBufferData(type_, data.size() * sizeof(T), data.data(), usage);
}

template void Buffers::SetData<struct meshdata::Vertex>(
    const std::vector<meshdata::Vertex>& data, GLenum usage) const;

template void Buffers::SetData<unsigned int>(
    const std::vector<unsigned int>& data, GLenum usage) const;