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

#include <mutex>
#include <string>
#include <vector>

#include "MeshData.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "include/Buffers.h"
#include "include/Shader.h"
#include "include/VertexArray.h"

/**
 * The Mesh class handles the storage and rendering of mesh data, including 
 * vertices, indices, and textures.
 * 
 * This class is responsible for setting up and rendering a mesh using OpenGL. 
 * It manages vertex buffers, element buffers, and vertex array objects.
 * 
 * Usage example:
 * @code
 * std::vector<meshdata::Vertex> vertices = ...;
 * std::vector<glm::uint32> indices = ...;
 * std::vector<meshdata::Texture> textures = ...;
 * Mesh mesh(vertices, indices, textures);
 * Shader shader = ...;
 * mesh.Draw(shader);
 * @endcode
 * 
 * @note This class is thread-safe. It uses internal mutexes to protect access 
 * to shared resources.
 */
class Mesh {
 public:
  /**
   * Constructs a Mesh object with the given vertices, indices, and textures.
   * @param vertices The vertex data for the mesh.
   * @param indices The index data for the mesh.
   * @param texture The texture data for the mesh.
   */
  explicit Mesh(const std::vector<meshdata::Vertex>& vertices,
                const std::vector<glm::uint32>& indices,
                const std::vector<meshdata::Texture>& texture);

  Mesh(const Mesh&) = delete;

  Mesh& operator=(const Mesh&) = delete;

  Mesh(Mesh&& other) noexcept;

  /**
   * Renders the mesh using the given shader. This function binds the necessary 
   * textures and draws the mesh.
   * @param shader The shader to use for rendering the mesh.
   */
  void Draw(Shader& shader);

  /**
   * Destructor for the Mesh class.Cleans up the allocated resources.
   */
  ~Mesh();

  /**
   * Gets the vertices of the mesh.
   * @return A const reference to the vector of vertices.
   */
  const std::vector<meshdata::Vertex>& GetVertices() const;

  /**
   * Sets the vertices of the mesh.
   * @param vertices The new vertices for the mesh.
   */
  void SetVertices(const std::vector<meshdata::Vertex>& vertices);

  /**
   * Gets the indices of the mesh.
   * @return A const reference to the vector of indices.
   */
  const std::vector<glm::uint32>& GetIndices() const;

  /**
   * Sets the indices of the mesh.
   * @param indices The new indices for the mesh.
   */
  void SetIndices(const std::vector<glm::uint32>& indices);

  /**
   * Gets the textures of the mesh.
   * @return A const reference to the vector of textures.
   */
  const std::vector<meshdata::Texture>& GetTextures() const;

  /**
   * Sets the textures of the mesh.
   * @param textures The new textures for the mesh.
   */
  void SetTextures(const std::vector<meshdata::Texture>& textures);

  /**
   * Gets the Vertex Array Object (VAO) of the mesh.
   * @return A const reference to the VAO.
   */
  const VertexArray& GetVao() const;

 private:
  /**
   * Sets up the mesh for rendering.This function initializes the vertex 
   * buffer, element buffer, and vertex attributes.
   */
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
  VertexArray vao_;
  Buffers vbo_, ebo_;

  mutable std::mutex mesh_mutex_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_MESH_H_
