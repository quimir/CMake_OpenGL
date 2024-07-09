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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_MESH_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_MESH_H_

#include <string>
#include <vector>

#include "VertexArray.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "include/Buffer.h"
#include "include/MeshData.h"
#include "include/Shader.h"

class Mesh {
 public:
  Mesh(std::vector<meshdata::Vertex> vertices, std::vector<glm::uint32> indices,
       std::vector<meshdata::Texture> texture);

  void Draw(Shader& shader);

  const std::vector<meshdata::Vertex>& GetVertices() const;

  void SetVertices(const std::vector<meshdata::Vertex>& vertices);

  const std::vector<glm::uint32>& GetIndices() const;

  void SetIndices(const std::vector<glm::uint32>& indices);

  const std::vector<meshdata::Texture>& GetTextures() const;

  void SetTextures(const std::vector<meshdata::Texture>& textures);

  GLuint GetVao() const;

  void SetVao(GLuint vao);

 private:
  void SetupMesh();

 private:
  /*
   * Mesh data 
   */
  std::vector<meshdata::Vertex> vertices_;
  std::vector<glm::uint32> indices_;
  std::vector<meshdata::Texture> textures_;
  /*
   * Render data 
   */
  VertexArray vao;
  Buffer vbo, ebo;
  GLuint vao_;
  GLuint vbo_, ebo_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_MESH_H_
