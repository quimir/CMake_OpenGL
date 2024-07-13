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

#include "include/Buffer.h"
#include "include/LoggerSystem.h"
#include "include/Model/MeshData.h"

Buffer::Buffer(GLenum type) : type_(type) {
  if (glGetString(GL_VERSION) == nullptr) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                    "OpenGL is not initialized");
    return;
  }
  glGenBuffers(1, &buffer_id_);
}
Buffer::~Buffer() {
  glDeleteBuffers(1, &buffer_id_);
}
void Buffer::Bind() const {
  glBindBuffer(this->type_, buffer_id_);
}
void Buffer::UnBind() const {
  glBindBuffer(type_, 0);
}
void Buffer::SetData(const void* data, GLsizeiptr size, GLenum usage) const {
  glBufferData(type_, size, data, usage);
}
GLenum Buffer::GetType() const {
  return type_;
}
void Buffer::SetType(GLenum type) {
  type_ = type;
}
template <typename T>
void Buffer::SetData(const std::vector<T>& data, GLenum usage) const {
  glBufferData(type_,data.size()*sizeof(T),data.data(),usage);
}

template void Buffer::SetData<struct meshdata::Vertex>(
    const std::vector<meshdata::Vertex>& data, GLenum usage) const;

template void Buffer::SetData<unsigned int>(
    const std::vector<unsigned int>& data, GLenum usage) const;