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

#include "include/LoadImage.h"
#include "include/LoggerSystem.h"
#include "include/Model/AssimpGLMHelpers.h"
#include "include/Model/Model.h"

using namespace std;

const std::vector<meshdata::Texture> &Model::GetTextureLoaded() const {
  return texture_loaded_;
}

void Model::SetTextureLoaded(const std::vector<meshdata::Texture> &texture_loaded) {
  texture_loaded_ = texture_loaded;
}

const std::vector<Mesh> &Model::GetMeshes() const {
  return meshes_;
}

void Model::SetMeshes(const std::vector<Mesh> &meshes) {
  meshes_ = meshes;
}

const std::string &Model::GetPath() const {
  return path_;
}

void Model::SetPath(const std::string &path) {
  path_ = path;
}

bool Model::IsGammaCorrection() const {
  return gamma_correction_;
}

void Model::SetGammaCorrection(bool gamma_correction) {
  gamma_correction_ = gamma_correction;
}

Model::Model(std::string path, bool gamma, bool animation) :
	path_(std::move(path)),
	gamma_correction_(gamma),
	animation_(animation),
	bone_counter_(0) {
  LoadModel(path_);
}

void Model::Draw(Shader &shader) {
  for (auto &meshes : meshes_) {
	meshes.Draw(shader);
  }
}

void Model::LoadModel(const std::string &path) {
  LoggerSystem &logger_system = LoggerSystem::GetInstance();
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path,
										   aiProcess_Triangulate
											   | aiProcess_GenSmoothNormals
											   | aiProcess_FlipUVs
											   | aiProcess_CalcTangentSpace);

  // Check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
	  || !scene->mRootNode) // If is Not Zero
  {
	logger_system.Log(LoggerSystem::Level::kWarning,
					  std::string("ERROR::ASSIMP:: ") + importer.GetErrorString());
	return;
  }

  // Process ASSIMP's root_ node recursively
  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
  // Process each mesh located at the current node
  for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
	/*
	 * The node object only contains indices to index the actual objects in the scene.
	 * The scene contains all the data, node is just to keep stuff organized 
	 * (like relations between nodes). 
	 */
	aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	meshes_.push_back(ProcessMesh(mesh, scene));
  }
  // After we've processed all the meshes (if any) we then recursively process each of 
  // the children nodes
  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
	ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
  /*
   * Data to fill 
   */
  vector<meshdata::Vertex> vertices;
  vector<GLuint> indices;
  vector<meshdata::Texture> textures;

  // Walk through each of the mesh's vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
	meshdata::Vertex vertex{};
	glm::vec3 vector;// We declare a placeholder vector since assimp uses its own vector 
	// class that doesn't directly convert to glm's vec3 class so we transfer the data 
	// to this placeholder glm::vec3 first.
	if (this->animation_) {
	  SetVertexBoneDataToDefault(vertex);
	  vertex.position = AssimpGLMHelpers::GetInstance().GetGLMVec(mesh->mVertices[i]);
	  vertex.normal = AssimpGLMHelpers::GetInstance().GetGLMVec(mesh->mNormals[i]);
	} else {
	  /*
	   * Positions 
	   */
	  vector.x = mesh->mVertices[i].x;
	  vector.y = mesh->mVertices[i].y;
	  vector.z = mesh->mVertices[i].z;
	  vertex.position = vector;
	  /*
	   * Normals
	   */
	  if (mesh->HasNormals()) {
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
	  }
	}
	/*
	 * Texture coordinates 
	 */
	if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
	{
	  glm::vec2 vec;
	  // A vertex can contain up to 8 different texture coordinates. We thus make the 
	  // assumption that we won't.use models where a vertex can have multiple texture 
	  // coordinates, so we always take the first set (0).
	  vec.x = mesh->mTextureCoords[0][i].x;
	  vec.y = mesh->mTextureCoords[0][i].y;
	  vertex.tex_coords = vec;
	  if (this->animation_) {
		/*
		 * Tangent
		 */
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;
		/*
		 * Bitangent
		 */
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector;
	  }
	} else {
	  vertex.tex_coords = glm::vec2(0.0f, 0.0f);
	}
	vertices.push_back(vertex);
  }
  // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve 
  // the corresponding vertex indices.
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
	aiFace face = mesh->mFaces[i];
	// Retrieve all indices of the face and store them in the indices vector
	for (unsigned int j = 0; j < face.mNumIndices; ++j) {
	  indices.push_back(face.mIndices[j]);
	}
  }
  // Process materials
  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  /*
   * we assume a convention for sampler names in the shaders. Each diffuse texture should 
   * be named as 'texture_diffuseN' where N is a sequential number ranging from 1 to 
   * MAX_SAMPLER_NUMBER.Same applies to other texture as the following list summarizes:
   * diffuse: texture_diffuseN,
   * specular: texture_specularN,
   * normal: texture_normalN 
   */

  // Diffuse maps
  vector<meshdata::Texture> diffuse_maps =
	  LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
  // Specular maps
  vector<meshdata::Texture> specular_maps =
	  LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  // Normal maps
  std::vector<meshdata::Texture>
	  normal_maps = LoadMaterialTexture(material, aiTextureType_HEIGHT, "texture_normal");
  textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
  // Height maps
  std::vector<meshdata::Texture> height_maps =
	  LoadMaterialTexture(material, aiTextureType_AMBIENT, "texture_height");
  textures.insert(textures.end(), height_maps.begin(), height_maps.end());

  if (this->animation_) {
	ExtractBoneWeightForVertices(vertices,mesh,scene);
  }

  // Return a mesh object created from the extracted mesh data
  return {vertices, indices, textures};
}

std::vector<meshdata::Texture> Model::LoadMaterialTexture(aiMaterial *mat,
														  aiTextureType type,
														  std::string type_name) {
  vector<meshdata::Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
	aiString str;
	mat->GetTexture(type, i, &str);
	// Check if texture was loaded before and if so, continue to next iteration: 
	// skip loading a new texture
	bool skip = false;
	for (auto &j : texture_loaded_) {
	  if ((j.path.compare(str.C_Str())) == 0) {
		textures.push_back(j);
		skip = true;// A texture with the same filepath has already been loaded, continue 
		// to next one. (optimization)
		break;
	  }
	}
	if (!skip) {
	  /*
	   * If texture hasn't been loaded already, load it
	   */

	  meshdata::Texture texture;
	  texture.id = LoadImage::GetInstance().LoadTexture2D(str.C_Str(), gamma_correction_);
	  texture.type = type_name;
	  texture.path = str.C_Str();
	  textures.push_back(texture);
	  texture_loaded_.push_back(texture);// Store it as texture loaded for entire model, 
	  // to ensure we won't unnecessarily load duplicate textures.
	}
  }

  return textures;
}
const map<std::string, BoneInfo> &Model::GetBoneInfoMap() const {
  return bone_info_map_;
}
glm::int32 Model::GetBoneCounter() const {
  return bone_counter_;
}
void Model::SetBoneInfoMap(const map<std::string, BoneInfo> &bone_info_map) {
  bone_info_map_ = bone_info_map;
}
void Model::SetBoneCounter(glm::int32 bone_counter) {
  this->bone_counter_ = bone_counter;
}

void Model::SetVertexBoneDataToDefault(meshdata::Vertex &vertex) {
  for (int i = 0; i < meshdata::kMaxBoneInfluence; ++i) {
	vertex.bone_ids[i] = -1;
	vertex.weights[i] = 0.0f;
  }
}

void Model::SetVertexBoneData(meshdata::Vertex &vertex,
							  glm::int32 bone_id,
							  glm::float32 weight) {
  for (int i = 0; i < meshdata::kMaxBoneInfluence; ++i) {
	if (vertex.bone_ids[i] < 0) {
	  vertex.weights[i] = weight;
	  vertex.bone_ids[i] = bone_id;
	  break;
	}
  }
}
void Model::ExtractBoneWeightForVertices(vector<meshdata::Vertex> &vertices,
										 aiMesh *mesh,
										 const aiScene *scene) {

  for (int bone_index = 0; bone_index < mesh->mNumBones; ++bone_index) {
	int bone_id = -1;
	string bone_name = mesh->mBones[bone_index]->mName.C_Str();
	if (this->bone_info_map_.find(bone_name) == bone_info_map_.end()) {
	  BoneInfo new_bone_info{};
	  new_bone_info.id = bone_counter_;
	  new_bone_info.offset =
		  AssimpGLMHelpers::GetInstance().ConvertMatrixToGLMFormat(mesh->mBones[bone_index]->mOffsetMatrix);
	  bone_info_map_[bone_name] = new_bone_info;
	  bone_id = bone_counter_;
	  bone_counter_++;
	} else {
	  bone_id = bone_info_map_[bone_name].id;
	}

	if (bone_id == -1) {
	  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
									  "Error, the skeleton is not present in the skeleton.");
	  return;
	}

	auto weights = mesh->mBones[bone_index]->mWeights;
	unsigned int num_weights = mesh->mBones[bone_index]->mNumWeights;

	for (int weight_index = 0; weight_index < num_weights; ++weight_index) {
	  unsigned int vertex_id = weights[weight_index].mVertexId;
	  float weight = weights[weight_index].mWeight;
	  if (vertex_id > vertices.size()) {
		LoggerSystem::GetInstance().Log(
			LoggerSystem::Level::kWarning,
			"Error, the ID of the skeleton weight does not exist, please check the "
			"original file before loading the skeleton animation.");
		return;
	  }
	  SetVertexBoneData(vertices[vertex_id],bone_id,weight);
	}
  }
}
void Model::SetBoneInfoMapID(const std::string& bone_name, int id) {
  this->bone_info_map_[bone_name].id=id;
}
BoneInfo Model::GetBoneInfo(const std::string& name) const {
  return this->bone_info_map_.at(name);
}
