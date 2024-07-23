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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_MODEL_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_MODEL_H_

#include <map>

#include "BoneInfo.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

/**
 * The Model class represents a 3D model loaded from an external file. It 
 * contains meshes, textures, and bone information. The class provides methods
 * to draw the model using a shader, and to access various aspects of the 
 * model's data.
 */
class Model {
 public:
  /**
   * Constructor for the Model class. Loads the model from the specified file 
   * path. Optionally applies gamma correction to the textures.
   * @param path The file path of the model file.
   * @param gamma Whether to apply gamma correction to the textures.
   */
  explicit Model(std::string path, bool gamma = false);

  /**
   * Destructor for the Model class. Frees all allocated resources.
   */
  ~Model();

  /**
   * Draws the model using the given shader.
   * @param shader The shader to use for rendering the model.
   */
  void Draw(Shader& shader);

  /**
   * Retrieves the loaded textures.
   * @return A const reference to the vector of loaded textures.
   */
  const std::vector<meshdata::Texture>& GetTextureLoaded() const;

  /**
   * Sets the loaded textures.
   * @param texture_loaded The new vector of loaded textures.
   */
  void SetTextureLoaded(const std::vector<meshdata::Texture>& texture_loaded);

  /**
   * Retrieves the meshes of the model.
   * @return A const reference to the vector of meshes.
   */
  const std::vector<Mesh*> GetMeshes() const;

  /**
   * Sets the meshes of the model.
   * @param meshes The new vector of meshes.
   */
  void SetMeshes(std::vector<Mesh*> meshes);

  /**
   * Checks if gamma correction is enabled for the model.
   * @return True if gamma correction is enabled, false otherwise.
   */
  bool IsGammaCorrection() const;

  /**
   * Enables or disables gamma correction for the model.
   * @param gamma_correction Whether to enable gamma correction.
   */
  void SetGammaCorrection(bool gamma_correction);

  /**
   * Retrieves the bone information map.
   * @return A const reference to the map of bone information.
   */
  const std::map<std::string, BoneInfo>& GetBoneInfoMap() const;

  /**
   * Retrieves the bone counter.
   * @return The bone counter value.
   */
  glm::int32 GetBoneCounter() const;

  /**
   * Sets the bone information map.
   * @param bone_info_map The new map of bone information.
   */
  void SetBoneInfoMap(const std::map<std::string, BoneInfo>& bone_info_map);

  /**
   * Sets the bone counter.
   * @param bone_counter The new bone counter value.
   */
  void SetBoneCounter(glm::int32 bone_counter);

 private:
  /**
   * Loads the model from the specified file path.
   * @param path The file path of the model file.
   */
  void LoadModel(std::string const& path);

  /**
   * Processes a node in the model hierarchy.
   * @param node The AI node to process.
   * @param scene The AI scene containing the node.
   */
  void ProcessNode(aiNode* node, const aiScene* scene);

  /**
   * Processes a mesh and creates a Mesh object from the AI mesh data.
   * @param mesh The AI mesh to process.
   * @param scene The AI scene containing the mesh.
   * @return A pointer to the created Mesh object.
   */
  Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

  /**
   * Loads the textures associated with a material.
   * @param mat The AI material containing the textures.
   * @param type The type of texture to load.
   * @param type_name The name of the texture type.
   * @param scene The AI scene containing the material.
   * @return A vector of Textures loaded from the material.
   */
  std::vector<meshdata::Texture> LoadMaterialTexture(
      aiMaterial* mat, aiTextureType type, const std::string& type_name,
      const aiScene* scene);

  /**
   * Sets the bone data for a vertex to default values.
   * @param vertex The vertex to set the bone data for.
   */
  void SetVertexBoneDataToDefault(meshdata::Vertex& vertex);

  /**
   * Sets the bone data for a vertex.
   * @param vertex The vertex to set the bone data for.
   * @param bone_id The bone ID.
   * @param weight The bone weight.
   */
  void SetVertexBoneData(meshdata::Vertex& vertex, glm::int32 bone_id,
                         glm::float32 weight);

  /**
   * Extracts bone weights for the vertices of a mesh.
   * @param vertices The vector of vertices to extract bone weights for.
   * @param mesh The AI mesh containing the bone weights.
   * @param scene The AI scene containing the mesh.
   */
  void ExtractBoneWeightForVertices(std::vector<meshdata::Vertex>& vertices,
                                    aiMesh* mesh, const aiScene* scene);

 private:
  /*
   * Model data
   */
  std::vector<meshdata::Texture> texture_loaded_;
  std::vector<Mesh*>
      meshes_;  // Note the use of a VertexArray and a Buffer that
                // could trigger the destructor if you don't get
                // its address.
  std::string directory_;
  bool gamma_correction_;

  /*
   * Bone data 
   */
  std::map<std::string, BoneInfo> bone_info_map_;
  glm::int32 bone_counter_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_MODEL_H_
