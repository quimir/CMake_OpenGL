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

#include "include/Model/Model.h"
#include "include/FilePathSystem.h"
#include "include/LoadImage.h"
#include "include/LoggerSystem.h"
#include "include/Model/AssimpGLMHelpers.h"

using namespace std;

Model::Model(const std::string& path, bool gamma)
    : gamma_correction_(gamma), bone_counter_(0) {
  LoadModel(path);
}

void Model::Draw(Shader& shader) {
  for (auto& meshes : meshes_) {
    meshes->Draw(shader);
  }
}

void Model::LoadModel(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  // Check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode)  // If is Not Zero
  {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        std::string("ERROR::ASSIMP:: ") + importer.GetErrorString());
    throw std::runtime_error(std::string("ERROR::ASSIMP:: ") +
                             importer.GetErrorString());
  }

  directory_ = path.substr(0, path.find_last_of('/'));
  // Process ASSIMP's root_ node recursively
  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  // Process each mesh located at the current node
  for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
    /*
	 * The node object only contains indices to index the actual objects in the scene.
	 * The scene contains all the data, node is just to keep stuff organized 
	 * (like relations between nodes). 
	 */
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(ProcessMesh(mesh, scene));
  }
  /**
   * After we've processed all the meshes (if any) we then recursively process 
   * each of the children nodes
   */
  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  /*
   * Data to fill 
   */
  vector<meshdata::Vertex> vertices;
  vector<GLuint> indices;
  vector<meshdata::Texture> textures;

  // Walk through each of the mesh's vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    meshdata::Vertex vertex{};
    /**
     * We declare a placeholder vector since assimp uses its own vector class 
     * that doesn't directly convert to glm's vec3 class, so we transfer the 
     * data to this placeholder glm::vec3 first.
     */

    // Positions
    vertex.position =
        AssimpGLMHelpers::GetInstance().Assimp3DToGLMVec3(mesh->mVertices[i]);

    // Normals
    if (mesh->HasNormals()) {
      vertex.normal =
          AssimpGLMHelpers::GetInstance().Assimp3DToGLMVec3(mesh->mNormals[i]);
    } else {
      vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // Texture coordinates
    if (mesh->mTextureCoords[0])  // Does the mesh contain texture coordinates?
    {
      /**
       * A vertex can contain up to 8 different texture coordinates. We thus 
       * make the assumption that we won't.use models where a vertex can have
       * multiple texture coordinates, so we always take the first set (0).
       */

      vertex.tex_coords =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

      // Tangent
      vertex.tangent =
          AssimpGLMHelpers::GetInstance().Assimp3DToGLMVec3(mesh->mTangents[i]);

      // Bitangent
      vertex.bitangent = AssimpGLMHelpers::GetInstance().Assimp3DToGLMVec3(
          mesh->mBitangents[i]);
    } else {
      vertex.tex_coords = glm::vec2(0.0f, 0.0f);
      vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
      vertex.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (scene->HasAnimations() || scene->mMeshes[0]->HasBones()) {
      SetVertexBoneDataToDefault(vertex);
    }
    vertices.push_back(vertex);
  }

  /**
   * Now wak through each of the mesh's faces (a face is a mesh its triangle) 
   * and retrieve the corresponding vertex indices.
   */
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    // Retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }
  // Process materials
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  /*
   * we assume a convention for sampler names in the shaders. Each diffuse 
   * texture should be named as 'texture_diffuseN' where N is a sequential 
   * number ranging from 1 to MAX_SAMPLER_NUMBER.Same applies to other texture 
   * as the following list summarizes:
   * diffuse: texture_diffuseN,
   * specular: texture_specularN,
   * normal: texture_normalN,
   * height: texture_heightN
   */

  // Diffuse maps
  vector<meshdata::Texture> diffuse_maps = LoadMaterialTexture(
      material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
  // Specular maps
  vector<meshdata::Texture> specular_maps = LoadMaterialTexture(
      material, aiTextureType_SPECULAR, "texture_specular", scene);
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  // Normal maps
  std::vector<meshdata::Texture> normal_maps = LoadMaterialTexture(
      material, aiTextureType_NORMALS, "texture_normal", scene);
  textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
  // Height maps
  std::vector<meshdata::Texture> height_maps = LoadMaterialTexture(
      material, aiTextureType_HEIGHT, "texture_height", scene);
  textures.insert(textures.end(), height_maps.begin(), height_maps.end());

  if (scene->HasAnimations() || scene->mMeshes[0]->HasBones()) {
    ExtractBoneWeightForVertices(vertices, mesh, scene);
  }

  // Return a mesh object created from the extracted mesh data
  return new Mesh(vertices, indices, textures);
}

std::vector<meshdata::Texture> Model::LoadMaterialTexture(
    aiMaterial* mat, aiTextureType type, const std::string& type_name,
    const aiScene* scene) {
  vector<meshdata::Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString str;
    mat->GetTexture(type, i, &str);
    // Check if texture was loaded before and if so, continue to next iteration:
    // skip loading a new texture
    auto item_path =
        std::find_if(texture_loaded_.begin(), texture_loaded_.end(),
                     [&str](const meshdata::Texture& texture) {
                       return texture.path == str.C_Str();
                     });

    if (item_path != texture_loaded_.end()) {
      textures.push_back((*item_path));
    } else {
      /*
	   * If texture hasn't been loaded already, load it
	   */
      meshdata::Texture texture;
      auto file_path = FilePathSystem::GetInstance().SplicePath(
          "%s/%s", directory_.c_str(), str.C_Str());

      auto ai_texture = scene->GetEmbeddedTexture(str.C_Str());
      if (ai_texture != nullptr) {
        texture.id = LoadImage::GetInstance().LoadTexture2DFromAssimp(
            ai_texture, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR,
            gamma_correction_);
      } else {
        texture.id = LoadImage::GetInstance().LoadTexture2D(
            file_path, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR,
            gamma_correction_);
      }
      texture.type = type_name;
      texture.path = str.C_Str();
      textures.push_back(texture);
      /**
       * Store it as texture loaded for entire model,to ensure we won't 
       * unnecessarily load duplicate textures.
       */
      texture_loaded_.push_back(texture);
    }
  }

  return textures;
}

void Model::SetVertexBoneDataToDefault(meshdata::Vertex& vertex) {
  for (int i = 0; i < meshdata::kMaxBoneInfluence; ++i) {
    vertex.bone_ids[i] = -1;
    vertex.weights[i] = 0.0f;
  }
}

void Model::SetVertexBoneData(meshdata::Vertex& vertex, glm::int32 bone_id,
                              glm::float32 weight) {
  for (int i = 0; i < meshdata::kMaxBoneInfluence; ++i) {
    if (vertex.bone_ids[i] < 0) {
      vertex.weights[i] = weight;
      vertex.bone_ids[i] = bone_id;
      break;
    }
  }
}

void Model::ExtractBoneWeightForVertices(vector<meshdata::Vertex>& vertices,
                                         aiMesh* mesh, const aiScene* scene) {
  for (int bone_index = 0; bone_index < mesh->mNumBones; ++bone_index) {
    int bone_id = -1;
    string bone_name = mesh->mBones[bone_index]->mName.C_Str();
    if (this->bone_info_map_.find(bone_name) == bone_info_map_.end()) {
      BoneInfo new_bone_info{};
      new_bone_info.id = bone_counter_;
      new_bone_info.offset =
          AssimpGLMHelpers::GetInstance().ConvertMatrix4ToGLMFormat(
              mesh->mBones[bone_index]->mOffsetMatrix);
      bone_info_map_[bone_name] = new_bone_info;
      bone_id = bone_counter_;
      bone_counter_++;
    } else {
      bone_id = bone_info_map_[bone_name].id;
    }

    if (bone_id == -1) {
      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kWarning,
          "Error, the skeleton is not present in the skeleton.");
      throw std::runtime_error(
          "Error, the skeleton is not present in the skeleton.");
    }

    auto weights = mesh->mBones[bone_index]->mWeights;
    unsigned int num_weights = mesh->mBones[bone_index]->mNumWeights;

    for (int weight_index = 0; weight_index < num_weights; ++weight_index) {
      unsigned int vertex_id = weights[weight_index].mVertexId;
      float weight = weights[weight_index].mWeight;
      if (vertex_id > vertices.size()) {
        LoggerSystem::GetInstance().Log(
            LoggerSystem::Level::kWarning,
            "Error, the ID of the skeleton weight does not exist, "
            "please check the original file before loading the skeleton "
            "animation.");
        throw std::runtime_error(
            "Error, the ID of the skeleton weight does not exist, "
            "please check the original file before loading the skeleton "
            "animation.");
      }
      SetVertexBoneData(vertices[vertex_id], bone_id, weight);
    }
  }
}

Model::~Model() {
  if (!meshes_.empty()) {
    for (auto meshes : meshes_) {
      if (meshes) {
        delete meshes;
      }
    }
  }
}

const map<std::string, BoneInfo>& Model::GetBoneInfoMap() const {
  return bone_info_map_;
}

glm::int32 Model::GetBoneCounter() const {
  return bone_counter_;
}

void Model::SetBoneInfoMap(const map<std::string, BoneInfo>& bone_info_map) {
  bone_info_map_ = bone_info_map;
}

void Model::SetBoneCounter(glm::int32 bone_counter) {
  this->bone_counter_ = bone_counter;
}

const std::vector<meshdata::Texture>& Model::GetTextureLoaded() const {
  return texture_loaded_;
}

void Model::SetTextureLoaded(
    const std::vector<meshdata::Texture>& texture_loaded) {
  texture_loaded_ = texture_loaded;
}

vector<Mesh*> Model::GetMeshes() const {
  return meshes_;
}

void Model::SetMeshes(vector<Mesh*> meshes) {
  meshes_ = meshes;
}

bool Model::IsGammaCorrection() const {
  return gamma_correction_;
}

void Model::SetGammaCorrection(bool gamma_correction) {
  gamma_correction_ = gamma_correction;
}