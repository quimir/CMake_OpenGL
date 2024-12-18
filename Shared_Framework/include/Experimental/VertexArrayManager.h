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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXARRAYMANAGER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXARRAYMANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../VertexArray.h"
#include "Core/MacroDefinition.h"

class VertexArrayManager {
 public:
  VertexArrayManager();

  void CreateVertexArray(const std::string& name);

  std::shared_ptr<VertexArray> GetVertexArray(const std::string& name);

  void Bind(const std::string& name);

  void Unbind();

  void DeleteVertexArray(const std::string& name);

 private:
  bool VertexArrayExists(const std::string& name) const;

 private:
  GLuint current_vao_id_;
  std::unordered_map<std::string, std::shared_ptr<VertexArray>>
      vertex_array_registry_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXARRAYMANAGER_H_
