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

#include "include/Experimental/VertexArrayManager.h"
#include "include/LoggerSystem.h"
void VertexArrayManager::CreateVertexArray(const std::string& name) {
  if (this->vertex_array_registry_.find(name) !=
      this->vertex_array_registry_.end()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Vertex array with this: " + name + "already exists.");
    throw std::runtime_error("Vertex array with this:" + name +
                             "name already exists.");
  }

  this->vertex_array_registry_[name] = std::make_shared<VertexArray>();
}
std::shared_ptr<VertexArray> VertexArrayManager::GetVertexArray(
    const std::string& name) {
  auto item = this->vertex_array_registry_.find(name);
  if (item == this->vertex_array_registry_.end()) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                    name + "not found.");
    throw std::runtime_error(name + "not found.");
  }

  return item->second;
}
void VertexArrayManager::Bind(const std::string& name) {
  auto item = this->vertex_array_registry_.find(name);
  if (item == this->vertex_array_registry_.end()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Error location: VertexArrayManager::Use(const std::string &name)" +
            std::string("error message:") + name + "not found.");
  }

  if (current_vao_id_ != item->second->GetVaoId()) {
    item->second->Bind();
    current_vao_id_ = item->second->GetVaoId();
  }
}
void VertexArrayManager::Unbind() {
  if (current_vao_id_ != 0) {
    glBindVertexArray(0);
    current_vao_id_ = 0;
  }
}
void VertexArrayManager::DeleteVertexArray(const std::string& name) {
  auto item = this->vertex_array_registry_.find(name);
  if (item == this->vertex_array_registry_.end()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Error location: void VertexArrayManager::DeleteVertexArray(const "
        "std::string& name)" +
            std::string("error message: ") + name + "not found.");
    throw std::runtime_error(name + "not found.");
  }
  this->vertex_array_registry_.erase(item);
}
bool VertexArrayManager::VertexArrayExists(const std::string& name) const {
  return this->vertex_array_registry_.find(name) !=
         this->vertex_array_registry_.end();
}
VertexArrayManager::VertexArrayManager()
    : current_vao_id_(0),
      vertex_array_registry_(
          std::unordered_map<std::string, std::shared_ptr<VertexArray>>()) {}
