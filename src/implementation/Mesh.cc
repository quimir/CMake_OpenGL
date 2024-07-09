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

#include <utility>

#include "include/Mesh.h"
const std::vector<meshdata::Vertex> &Mesh::GetVertices() const {
  return vertices_;
}
void Mesh::SetVertices(const std::vector<meshdata::Vertex> &vertices) {
  vertices_ = vertices;
}
const std::vector<glm::uint32> &Mesh::GetIndices() const {
  return indices_;
}
void Mesh::SetIndices(const std::vector<glm::uint32> &indices) {
  indices_ = indices;
}
const std::vector<meshdata::Texture> &Mesh::GetTextures() const {
  return textures_;
}
void Mesh::SetTextures(const std::vector<meshdata::Texture> &textures) {
  textures_ = textures;
}
GLuint Mesh::GetVao() const {
  return vao_;
}
void Mesh::SetVao(GLuint vao) {
  vao_ = vao;
}
Mesh::Mesh(std::vector<meshdata::Vertex> vertices,
		   std::vector<glm::uint32> indices,
		   std::vector<meshdata::Texture> texture) :
	vertices_(std::move(vertices)),
	indices_(std::move(indices)),
	textures_(std::move(texture)) {
  // Now that we have all the required data, set the vertex buffers and its attribute 
  // pointers.
  ebo.SetType(GL_ELEMENT_ARRAY_BUFFER);
  SetupMesh();
}
void Mesh::SetupMesh() {
  vao.Bind();
  vbo.Bind();
  ebo.Bind();
  // A great thing about structs is that their memory layout is sequential for all its 
  // items.the effect is that we can simply pass a pointer to the struct and it translates 
  // perfectly to a glm::vec3/2 array which again translates to 3/2 floats which 
  // translates to a byte array.
  vbo.SetData(vertices_, GL_STATIC_DRAW);
  ebo.SetData(indices_, GL_STATIC_DRAW);

  /*
   * Set the vertex attribute pointers 
   */
  // Vertex positions
  vao.AddBuffer(0,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(meshdata::Vertex),
				(void *) nullptr);
  // Vertex normals
  vao.AddBuffer(1,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(meshdata::Vertex),
				(void *) offsetof(meshdata::Vertex, normal));
  // Vertex texture coords
  vao.AddBuffer(2,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(meshdata::Vertex),
				(void *) offsetof(meshdata::Vertex, tex_coords));
  // Vertex tangent
  vao.AddBuffer(3,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(meshdata::Vertex),
				(void *) offsetof(meshdata::Vertex, tangent));
  // Vertex bitangent
  vao.AddBuffer(4,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(meshdata::Vertex),
				(void *) offsetof(meshdata::Vertex, bitangent));
  // Ids
  vao.AddIntBuffer(5,
				   4,
				   GL_INT,
				   sizeof(meshdata::Vertex),
				   (void *) offsetof(meshdata::Vertex, bone_ids));
  // Weights
  vao.AddBuffer(6,
				4,
				GL_FLOAT,
				GL_FALSE,
				sizeof(meshdata::Vertex),
				(void *) offsetof(meshdata::Vertex, weights));

  this->vao.UnBind();


//  /*
//   *  Create buffers/arrays
//   */
//  glGenVertexArrays(1, &this->vao_);
//  glGenBuffers(1, &this->vao_);
//  glGenBuffers(1, &this->ebo_);
//
//  glBindVertexArray(this->vao_);
//  // Load data into vertex buffers
//  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
//  // A great thing about structs is that their memory layout is sequential for all its 
//  // items.the effect is that we can simply pass a pointer to the struct and it translates 
//  // perfectly to a glm::vec3/2 array which again translates to 3/2 floats which 
//  // translates to a byte array.
//  glBufferData(GL_ARRAY_BUFFER,
//			   static_cast<GLsizeiptr>(vertices_.size() * sizeof(meshdata::Vertex)),
//			   &vertices_[0],
//			   GL_STATIC_DRAW);
//
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//			   static_cast<GLsizeiptr>(this->indices_.size() * sizeof(glm::uint32)),
//			   &this->indices_[0],
//			   GL_STATIC_DRAW);

  /*
   * Set the vertex attribute pointers 
   */

//  // Vertex positions
//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(0,
//						3,
//						GL_FLOAT,
//						GL_FALSE,
//						sizeof(meshdata::Vertex),
//						(void *) 0);
//  // Vertex normals
//  glEnableVertexAttribArray(1);
//  glVertexAttribPointer(1,
//						3,
//						GL_FLOAT,
//						GL_FALSE,
//						sizeof(meshdata::Vertex),
//						(void *) offsetof(meshdata::Vertex, normal));
//  // Vertex texture coords
//  glEnableVertexAttribArray(2);
//  glVertexAttribPointer(2,
//						2,
//						GL_FLOAT,
//						GL_FALSE,
//						sizeof(meshdata::Vertex),
//						(void *) offsetof(meshdata::Vertex, tex_coords));
//  // Vertex tangent
//  glEnableVertexAttribArray(3);
//  glVertexAttribPointer(3,
//						3,
//						GL_FLOAT,
//						GL_FALSE,
//						sizeof(meshdata::Vertex),
//						(void *) offsetof(meshdata::Vertex, tangent));
//  // Vertex bitangent
//  glEnableVertexAttribArray(4);
//  glVertexAttribPointer(4,
//						3,
//						GL_FLOAT,
//						GL_FALSE,
//						sizeof(meshdata::Vertex),
//						(void *) offsetof(meshdata::Vertex, bitangent));
//  // Ids
//  glEnableVertexAttribArray(5);
//  glVertexAttribIPointer(5,
//						 4,
//						 GL_INT,
//						 sizeof(meshdata::Vertex),
//						 (void *) offsetof(meshdata::Vertex, bone_ids));
//  // Weights
//  glEnableVertexAttribArray(6);
//  glVertexAttribPointer(6,
//						4,
//						GL_FLOAT,
//						GL_FALSE,
//						sizeof(meshdata::Vertex),
//						(void *) offsetof(meshdata::Vertex, weights));
//
//  glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) {
  GLuint diffuser_nr = 1, specular_nr = 1, normal_nr = 1, height_nr = 1;
  shader.Bind();
  for (GLuint i = 0; i < this->textures_.size(); i++) {
	glActiveTexture(GL_TEXTURE0 + i);// Active proper texture unit before binding
	// Retrieve texture number (the N in diffuse_textureN)
	std::string number;
	std::string name = this->textures_[i].type;
	if (name == "texture_diffuse")
	  number = std::to_string(diffuser_nr++);
	else if (name == "texture_specular")
	  number = std::to_string(specular_nr++); // transfer unsigned int to string
	else if (name == "texture_normal")
	  number = std::to_string(normal_nr++); // transfer unsigned int to string
	else if (name == "texture_height")
	  number = std::to_string(height_nr++); // transfer unsigned int to string

	// Now set the sampler to the correct texture unit
	shader.SetInt(name + number, static_cast<int>(i));
	// And finally bind the texture
	glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  // Draw mesh
  this->vao.Bind();
  //glBindVertexArray(this->vao_);
  glDrawElements(GL_TRIANGLES,
				 static_cast<GLsizei>(this->indices_.size()),
				 GL_UNSIGNED_INT,
				 0);
  this->vao.UnBind();
  //glBindVertexArray(0);

  // Always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}
