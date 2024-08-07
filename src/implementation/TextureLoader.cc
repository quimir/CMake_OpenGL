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

#include "include/TextureLoader.h"
#include <stdexcept>
#include "include/LoggerSystem.h"
#include "include/OpenGLStateManager.h"

void TextureLoader::EnableStbImageFlipYAxis() {
  stbi_set_flip_vertically_on_load(true);
}
void TextureLoader::DisEnableStbImageFlipYAxis() {
  stbi_set_flip_vertically_on_load(false);
}
GLenum TextureLoader::GetGLTextureType(TextureLoader::Type texture_type) const {
  switch (texture_type) {
    case Type::kTexture1D:
      return GL_TEXTURE_1D;
    case Type::kTexture2D:
      return GL_TEXTURE_2D;
    case Type::kTexture3D:
      return GL_TEXTURE_3D;
    case Type::kTexture1DArray:
      return GL_TEXTURE_1D_ARRAY;
    case Type::kTexture2DArray:
      return GL_TEXTURE_2D_ARRAY;
    case Type::kTextureRectangle:
      return GL_TEXTURE_RECTANGLE;
    case Type::kCubeMap:
      return GL_TEXTURE_CUBE_MAP;
    case Type::kCubeMapArray:
      return GL_TEXTURE_CUBE_MAP_ARRAY;
    case Type::kTextureBuffer:
      return GL_TEXTURE_BUFFER;
    case Type::kTexture2DMultisample:
      return GL_TEXTURE_2D_MULTISAMPLE;
    case Type::kTexture2DMultisampleArray:
      return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
    default:
      throw std::invalid_argument("Invalid texture type");
  }
}
GLenum TextureLoader::DetermineDataFormat(int nr_channels) {
  switch (nr_channels) {
    case 1:
      return GL_RED;
    case 3:
      return GL_RGB;
    case 4:
      return GL_RGBA;
    default:
      throw std::runtime_error("Unknown number of channels");
  }
}
void* TextureLoader::LoadImageData(bool is_hdr, const std::string& path,
                                   int* width, int* height, int* nr_channels,
                                   int desired_channels) {
  if (is_hdr) {
    return stbi_loadf(path.c_str(), width, height, nr_channels,
                      desired_channels);
  } else {
    return stbi_load(path.c_str(), width, height, nr_channels,
                     desired_channels);
  }
}
GLuint TextureLoader::ConfigureTextureAutoParams(const std::string& path,
                                                 GLenum texture_type,
                                                 TextureConfig texture_config) {
  if (path.empty()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "No texture loading, please recheck if texture path exists.");
    throw std::runtime_error(
        "No texture loading, please recheck if texture path exists.");
  }

  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(texture_type, texture);

  int width, height, nr_channels;
  bool is_hdr = IsHDR(path);
  auto data = LoadImageData(is_hdr, path, &width, &height, &nr_channels);

  GLenum format = DetermineDataFormat(nr_channels);
  GLenum type = is_hdr ? GL_FLOAT : GL_UNSIGNED_BYTE;
  GLint internalformat =
      is_hdr ? DetermineHDRInternalFormat(nr_channels)
             : DetermineInternalFormat(nr_channels,
                                       texture_config.gamma_correction);

  if (data) {
    if (texture_config.gamma_correction && !is_hdr) {
      GammaCorrect(static_cast<unsigned char*>(data), width, height, 1,
                   nr_channels, texture_config.gamma_value);
    }

    if (texture_type == GL_TEXTURE_1D) {
      glTexImage1D(texture_type, 0, internalformat, width, 0, format, type,
                   data);
    } else if (texture_type == GL_TEXTURE_2D ||
               texture_type == GL_TEXTURE_RECTANGLE) {
      glTexImage2D(texture_type, 0, internalformat, width, height, 0, format,
                   type, data);
    } else if (texture_type == GL_TEXTURE_BUFFER) {
      GLuint buffer;
      glGenBuffers(texture_type, &buffer);
      glBindTexture(texture_type, buffer);
      glBufferData(texture_type, width * height * nr_channels, data,
                   GL_STATIC_DRAW);
      glTexBuffer(texture_type, internalformat, buffer);
      glBindBuffer(texture_type, 0);
    } else {
      LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                      "The type is not defined and cannot be "
                                      "registered with OpenGL. Type name: " +
                                          std::to_string(texture_type));
      throw std::runtime_error(
          "The type is not defined and cannot be registered with OpenGL. Type "
          "name: " +
          std::to_string(texture_type));
    }

    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S,
                    texture_config.wrap_s_mode);
    if (texture_type == GL_TEXTURE_2D || texture_type == GL_TEXTURE_RECTANGLE) {
      glTexParameteri(texture_type, GL_TEXTURE_WRAP_T,
                      texture_config.wrap_t_mode);
    }

    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER,
                    texture_config.min_filter_mode);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER,
                    texture_config.mag_filter_mode);

    ConfigureTextureMipMap(texture_config.min_filter_mode, texture_type);

    stbi_image_free(data);
  } else {
    stbi_image_free(data);
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Failed to load texture from path: " + path);
    throw std::runtime_error("Failed to load texture from path: " + path);
  }

  return texture;
}

GLuint TextureLoader::ConfigureTextureAutoParams(
    const std::vector<std::string>& paths, GLenum texture_type,
    TextureConfig texture_config) {
  if (paths.empty()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "No texture loading, please recheck if texture path exists.");
    throw std::runtime_error(
        "No texture loading, please recheck if texture path exists.");
  }

  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(texture_type, texture);

  int width, height, nr_channels;
  std::vector<void*> layers;
  bool is_hdr = IsHDR(paths[0]);

  for (const auto& path : paths) {
    auto data = LoadImageData(is_hdr, path, &width, &height, &nr_channels);
    if (data) {
      layers.push_back(data);
    } else {
      for (auto& layer : layers) {
        stbi_image_free(layer);
      }
      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kWarning,
          "Failed to load texture from path: " + path);
      throw std::runtime_error("Failed to load texture from path: " + path);
    }
  }

  GLenum format = DetermineDataFormat(nr_channels);
  GLenum type = is_hdr ? GL_FLOAT : GL_UNSIGNED_BYTE;
  GLint internal_format =
      is_hdr ? DetermineHDRInternalFormat(nr_channels)
             : DetermineInternalFormat(nr_channels,
                                       texture_config.gamma_correction);

  if (texture_type == GL_TEXTURE_2D_ARRAY ||
      texture_type == GL_TEXTURE_1D_ARRAY || texture_type == GL_TEXTURE_3D) {
    if (texture_config.gamma_correction && !is_hdr) {
      for (auto& layer : layers) {
        GammaCorrect(static_cast<unsigned char*>(layer), width, height,
                     paths.size(), nr_channels, texture_config.gamma_value);
      }
    }
    glTexImage3D(texture_type, 0, internal_format, width, height, layers.size(),
                 0, format, type, nullptr);
    for (size_t i = 0; i < layers.size(); ++i) {
      glTexSubImage3D(texture_type, 0, 0, 0, i, width, height, 1, format, type,
                      layers[i]);
      stbi_image_free(layers[i]);
    }
  } else if (texture_type == GL_TEXTURE_CUBE_MAP) {
    if (texture_config.gamma_correction && !is_hdr) {
      for (auto& layer : layers) {
        GammaCorrect(static_cast<unsigned char*>(layer), width, height, 1,
                     nr_channels, texture_config.gamma_value);
      }
    }

    for (int i = 0; i < layers.size(); ++i) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format,
                   width, height, 0, format, type, layers[i]);
    }
  } else {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                    "The type is not defined and cannot be "
                                    "registered with OpenGL. Type name: " +
                                        std::to_string(texture_type));
    throw std::runtime_error(
        "The type is not defined and cannot be registered with OpenGL. Type "
        "name: " +
        std::to_string(texture_type));
  }

  glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, texture_config.wrap_s_mode);
  if (texture_type != GL_TEXTURE_1D_ARRAY) {
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_T,
                    texture_config.wrap_t_mode);
    if (texture_type == GL_TEXTURE_3D || texture_type == GL_TEXTURE_CUBE_MAP) {
      glTexParameteri(texture_type, GL_TEXTURE_WRAP_R,
                      texture_config.wrap_r_mode);
    }
  }
  glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER,
                  texture_config.min_filter_mode);
  glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER,
                  texture_config.mag_filter_mode);

  ConfigureTextureMipMap(texture_config.min_filter_mode, texture_type);

  return texture;
}

GLuint TextureLoader::ConfigureTextureAutoParams(
    const std::vector<std::vector<std::string>>& paths, GLenum texture_type,
    TextureConfig texture_config) {
  if (paths.empty()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "No texture loading, please recheck if texture path exists.");
    throw std::runtime_error(
        "No texture loading, please recheck if texture path exists.");
  };

  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(texture_type, texture);

  int width, height, nr_channels;
  int num_layers = static_cast<int>(paths.size());
  std::vector<unsigned char*> layer_data(num_layers * 6);
  bool is_hdr = IsHDR(paths[0][0]);
  for (const auto& i : paths) {
    for (size_t j = 0; j < i.size(); ++j) {
      auto data = LoadImageData(is_hdr, i[j], &width, &height, &nr_channels);
      if (data) {
        if (texture_config.gamma_correction && !is_hdr) {
          GammaCorrect(static_cast<unsigned char*>(data), width, height, 1,
                       nr_channels, texture_config.gamma_value);
        }
        layer_data[i.size() + j] = static_cast<unsigned char*>(data);
      } else {
        for (auto& layer : layer_data) {
          stbi_image_free(layer);
        }
        LoggerSystem::GetInstance().Log(
            LoggerSystem::Level::kWarning,
            "Failed to load texture from path: " + i[j]);
        throw std::runtime_error("Failed to load texture from path: " + i[j]);
      }
    }
  }
  GLenum format = DetermineDataFormat(nr_channels);
  GLenum type = is_hdr ? GL_FLOAT : GL_UNSIGNED_BYTE;
  GLint internalformat =
      is_hdr ? DetermineHDRInternalFormat(nr_channels)
             : DetermineInternalFormat(nr_channels,
                                       texture_config.gamma_correction);

  if (texture_type == GL_TEXTURE_CUBE_MAP_ARRAY) {
    glTexImage3D(texture_type, 0, internalformat, width, height, num_layers * 6,
                 0, format, type, nullptr);
    // Upload each face to the texture array
    for (size_t i = 0; i < paths.size(); ++i) {
      for (size_t j = 0; j < paths[i].size(); ++j) {
        GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + j;
        glTexSubImage3D(target, 0, 0, 0, i * paths[i].size() + j, width, height,
                        1, format, type, layer_data[i * paths[i].size() + j]);
        stbi_image_free(layer_data[i * paths[i].size() + j]);
      }
    }
  }

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S,
                  texture_config.wrap_s_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T,
                  texture_config.wrap_t_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R,
                  texture_config.wrap_r_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER,
                  texture_config.min_filter_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER,
                  texture_config.mag_filter_mode);

  ConfigureTextureMipMap(texture_config.min_filter_mode, texture_type);

  return texture;
}
bool TextureLoader::IsHDR(const std::string& path) {
  return stbi_is_hdr(path.c_str());
}
GLuint TextureLoader::ConfigureTextureMultisample(
    GLenum texture_type, GLsizei samples, GLenum internalformat, GLsizei width,
    GLsizei height, GLsizei depth, GLboolean fixed_sample_locations,
    GLint mag_filter_mode, GLint min_filter_mode) {
  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(texture_type, texture);

  if (texture_type == GL_TEXTURE_2D_MULTISAMPLE) {
    glTexImage2DMultisample(texture_type, samples, internalformat, width,
                            height, fixed_sample_locations);
  } else if (texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY) {
    glTexImage3DMultisample(texture_type, samples, internalformat, width,
                            height, depth, fixed_sample_locations);
  }

  glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, mag_filter_mode);
  glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, min_filter_mode);

  return texture;
}
TextureLoader::TextureLoader(TextureLoader::Type texture_type,
                             const std::string& path, GLint wrap_s_mode,
                             GLint wrap_t_mode, GLint mag_filter_mode,
                             GLint min_filter_mode, GLboolean gamma_correction,
                             float gamma_value) {
  texture_type_ = GetGLTextureType(texture_type);
  texture_id_ = ConfigureTextureAutoParams(
      path, texture_type_,
      {wrap_s_mode, wrap_t_mode, 0, mag_filter_mode, min_filter_mode,
       gamma_correction, gamma_value});
}
TextureLoader::TextureLoader(Type texture_type,
                             const std::vector<std::string>& paths,
                             GLint wrap_s_mode, GLint wrap_t_mode,
                             GLint wrap_r_mode, GLint mag_filter_mode,
                             GLint min_filter_mode, GLboolean gamma_correction,
                             float gamma_value) {
  texture_type_ = GetGLTextureType(texture_type);
  texture_id_ = ConfigureTextureAutoParams(
      paths, texture_type_,
      {wrap_s_mode, wrap_t_mode, wrap_r_mode, mag_filter_mode, min_filter_mode,
       gamma_correction, gamma_value});
}
TextureLoader::TextureLoader(TextureLoader::Type texture_type, GLsizei samples,
                             GLenum internalformat, GLsizei width,
                             GLsizei height, GLsizei depth,
                             GLboolean fixed_sample_locations,
                             GLint mag_filter_mode, GLint min_filter_mode) {
  texture_type_ = GetGLTextureType(texture_type);
  ConfigureTextureMultisample(texture_type_, samples, internalformat, width,
                              height, depth, fixed_sample_locations,
                              mag_filter_mode, min_filter_mode);
}
TextureLoader::TextureLoader(Type texture_type,
                             const std::vector<std::vector<std::string>>& paths,
                             GLint wrap_s_mode, GLint wrap_t_mode,
                             GLint wrap_r_mode, GLint mag_filter_mode,
                             GLint min_filter_mode, GLboolean gamma_correction,
                             float gamma_value) {
  texture_type_ = GetGLTextureType(texture_type);
  texture_id_ = ConfigureTextureAutoParams(
      paths, texture_type_,
      {wrap_s_mode, wrap_t_mode, wrap_r_mode, mag_filter_mode, min_filter_mode,
       gamma_correction, gamma_value});
}
GLuint TextureLoader::ConfigureAssimpTextureAutoParams(
    const aiTexture* ai_texture, GLenum texture_type,
    TextureConfig texture_config) {
  if (nullptr == ai_texture) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Error! The texture target does not exist.");
    return 0;
  }

  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }

  GLuint texture;
  glGenTextures(1, &texture);

  int width, height, nr_channels;
  void* image_data = nullptr;
  if (ai_texture->mHeight == 0) {
    image_data = stbi_load_from_memory(
        reinterpret_cast<unsigned char*>(ai_texture->pcData),
        ai_texture->mWidth, &width, &height, &nr_channels, 0);
  } else {
    image_data = stbi_load_from_memory(
        reinterpret_cast<unsigned char*>(ai_texture->pcData),
        ai_texture->mWidth * ai_texture->mHeight, &width, &height, &nr_channels,
        0);
  }

  if (image_data) {
    GLenum format = DetermineDataFormat(nr_channels);
    bool is_hdr = IsHDR(ai_texture->mFilename.C_Str());
    GLenum type = is_hdr ? GL_FLOAT : GL_UNSIGNED_BYTE;
    GLint internalformat =
        is_hdr ? DetermineHDRInternalFormat(nr_channels)
               : DetermineInternalFormat(nr_channels,
                                         texture_config.gamma_correction);
    if (texture_config.gamma_correction && !is_hdr) {
      GammaCorrect(static_cast<unsigned char*>(image_data), width, height, 1,
                   nr_channels, texture_config.gamma_value);
    }
    glBindTexture(texture_type, texture);
    if (texture_type == GL_TEXTURE_2D) {
      glTexImage2D(texture_type, 0, internalformat, width, height, 0, format,
                   type, image_data);
    } else if (texture_type == GL_TEXTURE_CUBE_MAP) {
      for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat,
                     width, height, 0, format, type, image_data);
      }
    }

    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S,
                    texture_config.wrap_s_mode);
    if (texture_type != GL_TEXTURE_1D) {
      glTexParameteri(texture_type, GL_TEXTURE_WRAP_T,
                      texture_config.wrap_t_mode);
      if (texture_type == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_R,
                        texture_config.wrap_r_mode);
      }
    }

    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER,
                    texture_config.min_filter_mode);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER,
                    texture_config.mag_filter_mode);

    ConfigureTextureMipMap(texture_config.min_filter_mode, texture_type);

    stbi_image_free(image_data);
  } else {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Texture failed to load at path: " +
            std::string(ai_texture->mFilename.C_Str()));
  }

  stbi_image_free(image_data);
  return texture;
}
GLint TextureLoader::DetermineInternalFormat(int nr_channels,
                                             bool gamma_correction) {
  GLint internal_format = GL_RGB;
  if (gamma_correction) {
    internal_format = GL_SRGB;
  }

  switch (nr_channels) {
    case 1:
      internal_format = GL_RED;
      break;
    case 3:
      internal_format = gamma_correction ? GL_SRGB : GL_RGB;
      break;
    case 4:
      internal_format = gamma_correction ? GL_SRGB_ALPHA : GL_RGBA;
      break;
    default:
      throw std::runtime_error("Unknown number of channels");
  }

  return internal_format;
}
GLint TextureLoader::DetermineHDRInternalFormat(int nr_channels) {
  GLint internal_format;
  switch (nr_channels) {
    case 1:
      internal_format = GL_R32F;
      break;
    case 3:
      internal_format = GL_RGB32F;
      break;
    case 4:
      internal_format = GL_RGBA32F;
    default:
      throw std::runtime_error("Unknown number of channels");
  }

  return internal_format;
}
void TextureLoader::Bind(GLenum texture_unit) {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  glActiveTexture(texture_unit);
  glBindTexture(texture_type_, texture_id_);
}
void TextureLoader::UnBind() {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  glBindTexture(texture_type_, 0);
}
TextureLoader::~TextureLoader() {
  if (texture_id_ != 0) {
    glDeleteTextures(1, &texture_id_);
  }
}
void TextureLoader::ConfigureTextureMipMap(GLint min_filter_mode,
                                           GLenum texture_type) {
  if (min_filter_mode == GL_LINEAR_MIPMAP_LINEAR ||
      min_filter_mode == GL_LINEAR_MIPMAP_NEAREST ||
      min_filter_mode == GL_NEAREST_MIPMAP_LINEAR ||
      min_filter_mode == GL_NEAREST_MIPMAP_NEAREST) {
    glGenerateMipmap(texture_type);
  }
}

GLuint TextureLoader::GetTextureId() const {
  return texture_id_;
}

void TextureLoader::ResetActiveTexture() {
  glActiveTexture(GL_TEXTURE0);
}
TextureLoader::TextureLoader(TextureLoader::Type texture_type,
                             const aiTexture* ai_texture,
                             TextureLoader::TextureConfig texture_config) {
  texture_type_ = GetGLTextureType(texture_type);
  texture_type_ = ConfigureAssimpTextureAutoParams(ai_texture, texture_type_,
                                                   texture_config);
}

template <typename T>
void TextureLoader::GammaCorrect(T* data, int width, int height, int depth,
                                 int nr_components, float gamma) {
  int size = width * height * depth * nr_components;
  for (int i = 0; i < size; ++i) {
    auto value = static_cast<float>(data[i] / 255.0f);
    value = pow(value, gamma);
    data[i] = static_cast<T>(value * 255.0f);
  }
}
