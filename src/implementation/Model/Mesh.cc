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

#include "include/Model/Mesh.h"
#include <utility>
#include "include/LoggerSystem.h"

using namespace model;
const std::vector<meshdata::Vertex>& Mesh::GetVertices() const {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  return vertices_;
}
void Mesh::SetVertices(const std::vector<meshdata::Vertex>& vertices) {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  vertices_ = vertices;
  SetupMesh();
}
const std::vector<glm::uint32>& Mesh::GetIndices() const {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  return indices_;
}
void Mesh::SetIndices(const std::vector<glm::uint32>& indices) {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  indices_ = indices;
  SetupMesh();
}
const std::vector<meshdata::Texture>& Mesh::GetTextures() const {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  return textures_;
}
void Mesh::SetTextures(const std::vector<meshdata::Texture>& textures) {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  textures_ = textures;
}
Mesh::Mesh(const std::vector<meshdata::Vertex>& vertices,
           const std::vector<glm::uint32>& indices,
           const std::vector<meshdata::Texture>& texture)
    : vertices_(vertices),
      indices_(indices),
      textures_(texture),
      ebo_(1, GL_ELEMENT_ARRAY_BUFFER),
      vbo_(1) {
  // Now that we have all the required data, set the vertex buffers and its
  // attribute pointers.
  SetupMesh();
}
void Mesh::SetupMesh() {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  vao_.Bind();
  // A great thing about structs is that their memory layout is sequential for all its
  // items.the effect is that we can simply pass a pointer to the struct and it translates
  // perfectly to a glm::vec3/2 array which again translates to 3/2 floats which
  // translates to a byte array.
  vbo_.Bind();
  vbo_.SetData(vertices_, GL_STATIC_DRAW);
  ebo_.Bind();
  ebo_.SetData(indices_, GL_STATIC_DRAW);

  /**
   * Set the vertex attribute pointers
   */
  // Vertex positions
  vao_.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(meshdata::Vertex),
                 (void*)nullptr);
  // Vertex normals
  vao_.AddBuffer(1, 3, GL_FLOAT, GL_FALSE, sizeof(meshdata::Vertex),
                 (void*)offsetof(meshdata::Vertex, normal));
  // Vertex texture coords
  vao_.AddBuffer(2, 2, GL_FLOAT, GL_FALSE, sizeof(meshdata::Vertex),
                 (void*)offsetof(meshdata::Vertex, tex_coords));
  // Vertex tangent
  vao_.AddBuffer(3, 3, GL_FLOAT, GL_FALSE, sizeof(meshdata::Vertex),
                 (void*)offsetof(meshdata::Vertex, tangent));
  // Vertex bitangent
  vao_.AddBuffer(4, 3, GL_FLOAT, GL_FALSE, sizeof(meshdata::Vertex),
                 (void*)offsetof(meshdata::Vertex, bitangent));
  // Ids
  vao_.AddIntBuffer(5, 4, GL_INT, sizeof(meshdata::Vertex),
                    (void*)offsetof(meshdata::Vertex, bone_ids));
  // Weights
  vao_.AddBuffer(6, 4, GL_FLOAT, GL_FALSE, sizeof(meshdata::Vertex),
                 (void*)offsetof(meshdata::Vertex, weights));

  this->vao_.UnBind();
  this->vbo_.UnBind();
  this->ebo_.UnBind();
}

void Mesh::Draw(Shader& shader) {
  std::lock_guard<std::mutex> lock(mesh_mutex_);
  GLuint diffuser_nr = 1, specular_nr = 1, normal_nr = 1, height_nr = 1;
  shader.Use();
  for (GLuint i = 0; i < this->textures_.size(); i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i);  // Active proper texture unit before binding
    // Retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = this->textures_[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(diffuser_nr++);
    else if (name == "texture_specular")
      number =
          std::to_string(specular_nr++);  // transfer unsigned int to string
    else if (name == "texture_normal")
      number = std::to_string(normal_nr++);  // transfer unsigned int to
                                             // string
    else if (name == "texture_height")
      number = std::to_string(height_nr++);  // transfer unsigned int to
                                             // string
    else {
      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kWarning,
          "Looking for uniform texture error, no texture appears!");
      continue;
    }
    // Now set the sampler to the correct texture unit
    shader.SetInt(name + number, static_cast<int>(i));
    // And finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  // Draw mesh
  this->vao_.Bind();
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()),
                 GL_UNSIGNED_INT, nullptr);
  this->vao_.UnBind();

  // Always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
  shader.UnUse();
}

const VertexArray& Mesh::GetVao() const {
  return vao_;
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertices_(std::move(other.vertices_)),
      indices_(std::move(other.indices_)),
      textures_(std::move(other.textures_)),
      vao_(),
      vbo_(other.vbo_),
      ebo_(other.ebo_) {
  other.vertices_.clear();
  other.textures_.clear();
  other.indices_.clear();
  other.vao_.ResetVertexArrays();
  other.vbo_.ResetBuffers(other.vbo_.GetN(), other.vbo_.GetType());
  other.ebo_.ResetBuffers(other.ebo_.GetN(), other.ebo_.GetType());
}