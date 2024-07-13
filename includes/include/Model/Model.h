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

#include "AssimpGLMHelpers.h"
#include "BoneInfo.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class Model {
 public:
  Model(std::string path, bool animation = false, bool gamma = false);

  void Draw(Shader &shader);

  const std::vector<meshdata::Texture> &GetTextureLoaded() const;

  void SetTextureLoaded(const std::vector<meshdata::Texture> &texture_loaded);

  const std::vector<Mesh> &GetMeshes() const;

  void SetMeshes(const std::vector<Mesh> &meshes);

  const std::string &GetPath() const;

  void SetPath(const std::string &path);

  bool IsGammaCorrection() const;

  void SetGammaCorrection(bool gamma_correction);

  const std::map<std::string, BoneInfo> &GetBoneInfoMap() const;
  
  BoneInfo GetBoneInfo(const std::string& name)const;

  glm::int32 GetBoneCounter() const;

  void SetBoneInfoMap(const std::map<std::string, BoneInfo> &bone_info_map);
  
  void SetBoneInfoMapID(const std::string& bone_name,int id);

  void SetBoneCounter(glm::int32 bone_counter);

 private:
  void LoadModel(std::string const &path);

  void ProcessNode(aiNode *node, const aiScene *scene);

  Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<meshdata::Texture> LoadMaterialTexture(aiMaterial *mat,
													 aiTextureType type,
													 std::string type_name);

  void SetVertexBoneDataToDefault(meshdata::Vertex &vertex);

  void SetVertexBoneData(meshdata::Vertex &vertex,
						 glm::int32 bone_id,
						 glm::float32 weight);

  void ExtractBoneWeightForVertices(std::vector<meshdata::Vertex> &vertices,
									aiMesh *mesh,
									const aiScene *scene);
 private:
  /*
   * Model data
   */
  std::vector<meshdata::Texture> texture_loaded_;
  std::vector<Mesh> meshes_;
  std::string path_;
  bool gamma_correction_;

  /*
   * Bone data 
   */

  std::map<std::string, BoneInfo> bone_info_map_;
  glm::int32 bone_counter_;

  bool animation_;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_MODEL_H_
