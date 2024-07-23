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

Buffers::Buffers(GLenum type)
    : type_(type), buffer_id_(0), binding_state_(false) {
  if (glGetString(GL_VERSION) == nullptr) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                    "OpenGL is not initialized");
    return;
  }
  glGenBuffers(1, &buffer_id_);
}
Buffers::~Buffers() {
  glDeleteBuffers(1, &buffer_id_);
}
void Buffers::Bind() {
  if (!binding_state_) {
    glBindBuffer(this->type_, buffer_id_);
    binding_state_ = true;
  }
}
void Buffers::UnBind() {
  if (binding_state_) {
    glBindBuffer(type_, 0);
    binding_state_ = false;
  }
}
void Buffers::SetData(const void* data, GLsizeiptr size, GLenum usage) const {
  glBufferData(type_, size, data, usage);
}
GLenum Buffers::GetType() const {
  return type_;
}
void Buffers::SetType(GLenum type) {
  type_ = type;
}
bool Buffers::IsBindingState() const {
  return binding_state_;
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