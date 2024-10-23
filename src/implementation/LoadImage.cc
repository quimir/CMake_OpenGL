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

#include "include/LoadImage.h"
#include "include/Exception.h"
#include "include/LoggerSystem.h"
#include "include/OpenGLStateManager.h"

using namespace std;

std::once_flag LoadImage::initialized_;
LoadImage* LoadImage::instance_ = nullptr;

GLuint LoadImage::LoadTexture2D(const std::string& path, GLint wrap_mode,
                                GLint mag_filter_mode, GLint min_filter_mode,
                                GLboolean gamma_correction) {
  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    return 0;
  }
  GLuint texture;
  glGenTextures(1, &texture);

  // Load image, create texture and generate mipmaps
  int width, height, nr_channels;
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
  glBindTexture(GL_TEXTURE_2D, texture);
  if (ConfigureTexture2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, nr_channels,
                         0, GL_UNSIGNED_BYTE, data, gamma_correction)) {
    /*
	 * Set the texture wrapping parameters 
	 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
    /*
	 * Set texture filtering parameters
	 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag_filter_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, min_filter_mode);
    // Enable the mipmap property on the image
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
  } else {
    throw Exception(
        LoggerSystem::Level::kWarning,
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
            path);
  }
}

LoadImage& LoadImage::GetInstance() {
  if (instance_ == nullptr) {
    std::call_once(initialized_, []() { instance_ = new LoadImage(); });
  }
  return *instance_;
}

void LoadImage::EnableStbImageFlipYAxis() {
  stbi_set_flip_vertically_on_load(true);
}
GLuint LoadImage::LoadCubeMap(std::vector<std::string> faces, GLint wrap_mode,
                              GLint mag_filter_mode, GLint min_filter_mode,
                              GLboolean gamma_correction) {
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  int width, height, nr_channels;
  for (unsigned int i = 0; i < faces.size(); ++i) {
    auto* data = stbi_load(faces[i].c_str(), &width, &height, &nr_channels, 0);
    if (!ConfigureTexture2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                            width, height, nr_channels, 0, GL_UNSIGNED_BYTE,
                            data, gamma_correction)) {
      throw Exception(LoggerSystem::Level::kWarning,
                      "Cube map texture failed to load at path: " + faces[i]);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter_mode);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter_mode);

  return texture_id;
}
bool LoadImage::ConfigureTexture2D(GLenum target, GLint level,
                                   GLint internal_format, int width, int height,
                                   int nr_components, GLint border, GLenum type,
                                   unsigned char* data,
                                   GLboolean gamma_correction) {
  if (data) {
    GLenum format = DetermineFormat(nr_components);

    if (gamma_correction) {
      GammaCorrect(data, width, height, 1, nr_components, 2.2f);
    }

    glTexImage2D(target, level, internal_format, width, height, border, format,
                 type, data);
    stbi_image_free(data);
    return true;
  }

  stbi_image_free(data);
  return false;
}
GLuint LoadImage::LoadTexture2DFromAssimp(const aiTexture* ai_texture,
                                          GLint wrap_mode,
                                          GLint mag_filter_mode,
                                          GLint min_filter_mode,
                                          GLboolean gamma_correction) {
  if (nullptr == ai_texture) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Error! The texture target does not exist.");
    return 0;
  }

  GLuint texture_id = 0;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_mode);

  int width, height, nr_channels;
  unsigned char* image_data = nullptr;
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
    GLenum format = DetermineFormat(nr_channels);

    if (gamma_correction) {
      GammaCorrect(image_data, width, height, 1, nr_channels, 2.2f);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Texture failed to load at path: " +
            std::string(ai_texture->mFilename.C_Str()));
  }

  stbi_image_free(image_data);
  return texture_id;
}

void LoadImage::DisEnableStbImageFlipYAxis() {
  stbi_set_flip_vertically_on_load(false);
}

bool LoadImage::ConfigureTexture1D(GLenum target, GLint level,
                                   GLint internal_format, int width,
                                   int nr_components, GLint border, GLenum type,
                                   unsigned char* data,
                                   GLboolean gamma_correction) {
  if (data) {
    GLenum format = DetermineFormat(nr_components);

    if (gamma_correction) {
      GammaCorrect(data, width, 1, 1, nr_components, 2.2);
    }

    glTexImage1D(target, level, internal_format, width, border, format, type,
                 data);
    stbi_image_free(data);
    return true;
  }

  stbi_image_free(data);
  return false;
}

GLuint LoadImage::LoadTexture1D(const string& path, GLint wrap_mode,
                                GLint mag_filter_mode, GLint min_filter_mode,
                                GLboolean gamma_correction) {
  GLuint texture;
  glGenTextures(1, &texture);

  // Load image, create texture and generate mipmaps
  int width, height, nr_channels;
  unsigned char* data = LoadImageData(path, &width, &height, &nr_channels);
  glBindTexture(GL_TEXTURE_1D, texture);
  if (ConfigureTexture1D(GL_TEXTURE_1D, 0, GL_RGBA, width, nr_channels, 0,
                         GL_UNSIGNED_BYTE, data, gamma_correction)) {
    /*
	 * Set the texture wrapping parameters 
	 */
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrap_mode);
    /*
	 * Set texture filtering parameters
	 */
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, mag_filter_mode);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, min_filter_mode);
    // Enable the mipmap property on the image
    glGenerateMipmap(GL_TEXTURE_1D);
    return texture;
  } else {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
            path);
    throw std::runtime_error(
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
        path);
  }
}
GLuint LoadImage::LoadTexture3D(const vector<std::string>& paths,
                                GLint wrapMode, GLint magFilterMode,
                                GLint minFilterMode,
                                GLboolean gamma_correction) {
  GLuint texture;
  glGenTextures(1, &texture);
  int width, height, nr_channels;
  std::vector<float*> slices;
  bool is_hdr = false;

  for (const auto& path : paths) {
    float* data = nullptr;
    if (stbi_is_hdr(path.c_str())) {
      data = stbi_loadf(path.c_str(), &width, &height, &nr_channels, 0);
      is_hdr = true;
    } else {
      data = reinterpret_cast<float*>(
          stbi_load(path.c_str(), &width, &height, &nr_channels, 0));
    }

    if (data) {
      slices.push_back(data);
    } else {
      for (auto& slice : slices) {
        stbi_image_free(slice);
      }

      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kWarning,
          "Failed to load 3D texture from path: " + path);
      throw std::runtime_error("Failed to load 3D texture from path: " + path);
    }
  }

  glBindTexture(GL_TEXTURE_3D, texture);
  if (ConfigureTexture3D(GL_TEXTURE_3D, 0, GL_RGB, width, height, nr_channels,
                         slices, 0, GL_FLOAT, gamma_correction)) {
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrapMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, minFilterMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, magFilterMode);
    glGenerateMipmap(GL_TEXTURE_3D);
    return texture;
  } else {
    string dir_log;
    for (const auto& path : paths) {
      dir_log.append("; " + path);
    }
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
            dir_log);
    throw std::runtime_error(
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
        dir_log);
  }
}
template <typename T>
bool LoadImage::ConfigureTexture3D(GLenum target, GLint level,
                                   GLint internal_format, int width, int height,
                                   int nr_components,
                                   const std::vector<T>& depth, GLint border,
                                   GLenum type, GLboolean gamma_correction) {
  if (!depth.empty()) {
    GLenum format = DetermineFormat(nr_components);

    glTexImage3D(target, level, internal_format, width, height, depth.size(),
                 border, format, type, nullptr);

    if (depth.size() > 1) {
      for (int i = 0; i < depth.size(); ++i) {
        if (gamma_correction) {
          GammaCorrect(depth[i], width, height, depth.size(), nr_components,
                       2.2f);
        }
        glTexSubImage3D(target, level, 0, 0, i, width, height, 1, format,
                        GL_FLOAT, depth[i]);
        stbi_image_free(depth[i]);
      }
    } else
      for (int i = 0; i < depth.size(); i++) {
        if (gamma_correction) {
          GammaCorrect(depth[i], width, height, depth.size(), nr_components,
                       2.2f);
        }
        stbi_image_free(depth[i]);
      }
    return true;
  }

  return false;
}
GLuint LoadImage::LoadTexture1DArray(const vector<std::string>& paths,
                                     GLint wrapMode, GLint magFilterMode,
                                     GLint minFilterMode,
                                     GLboolean gammaCorrection) {
  int width, height, nr_channels;
  std::vector<unsigned char*> layers;
  for (const auto& path : paths) {
    unsigned char* data = LoadImageData(path, &width, &height, &nr_channels);
    if (data) {
      layers.push_back(data);
    } else {
      for (auto& layer : layers) {
        stbi_image_free(layer);
      }
      throw std::runtime_error("Failed to load 1D array texture from path: " +
                               path);
    }
  }

  if (!layers.empty()) {
    GLenum format = DetermineFormat(nr_channels);

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_1D_ARRAY, texture_id);
    glTexImage3D(GL_TEXTURE_1D_ARRAY, 0, format, width, height, layers.size(),
                 0, format, GL_UNSIGNED_BYTE, nullptr);

    for (size_t i = 0; i < layers.size(); ++i) {
      if (gammaCorrection) {
        GammaCorrect(layers[i], width, height, layers.size(), nr_channels,
                     2.2f);
      }
      glTexSubImage3D(GL_TEXTURE_1D_ARRAY, 0, 0, i, 0, width, height, 1, format,
                      GL_UNSIGNED_BYTE, layers[i]);
      stbi_image_free(layers[i]);
    }

    glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilterMode);
    glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilterMode);

    glGenerateMipmap(GL_TEXTURE_1D_ARRAY);

    return texture_id;
  } else {
    string dir_log;
    for (const auto& path : paths) {
      dir_log.append("; " + path);
    }
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
            dir_log);
    throw std::runtime_error(
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
        dir_log);
  }
}

GLuint LoadImage::LoadTexture2DArray(const vector<std::string>& paths,
                                     GLint wrapMode, GLint magFilterMode,
                                     GLint minFilterMode,
                                     GLboolean gammaCorrection) {
  GLuint texture;
  glGenTextures(GL_TEXTURE_2D_ARRAY, &texture);
  int width, height, nr_channels;
  std::vector<unsigned char*> layers;
  for (const auto& path : paths) {
    unsigned char* data = LoadImageData(path, &width, &height, &nr_channels);
    if (data) {
      layers.push_back(data);
    } else {
      for (auto& layer : layers) {
        stbi_image_free(layer);
      }
      throw Exception(LoggerSystem::Level::kWarning,
                      "Failed to load 2D array texture from path: " + path);
    }
  }

  glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
  if (ConfigureTexture3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, width, height,
                         nr_channels, layers, 0, GL_UNSIGNED_BYTE,
                         gammaCorrection)) {
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilterMode);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilterMode);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    return texture;
  } else {
    string dir_log;
    for (const auto& path : paths) {
      dir_log.append("; " + path);
    }
    throw Exception(
        LoggerSystem::Level::kWarning,
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
            dir_log);
  }
}
GLuint LoadImage::LoadTextureAuto2D(const string& path, GLint wrap_mode,
                                    GLint mag_filter_mode,
                                    GLint min_filter_mode,
                                    GLboolean gamma_correction) {
  GLuint texture_id;
  glGenTextures(1, &texture_id);

  int width, height, nr_components;
  auto data = LoadImageData(path, &width, &height, &nr_components);

  if (data) {
    GLenum format;
    format = DetermineFormat(nr_components);
    if (gamma_correction) {
      GammaCorrect(data, width, height, 1, nr_components, 2.2f);
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_mode);
    stbi_image_free(data);
  } else {
    stbi_image_free(data);
    throw Exception(
        LoggerSystem::Level::kWarning,
        "The resource file also failed to load and is about to clean up the "
        "storage space for the image.file name: " +
            path);
  }
  return texture_id;
}
GLenum LoadImage::DetermineFormat(int nr_channels) {
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
stbi_uc* LoadImage::LoadImageData(const string& path, int* width, int* height,
                                  int* nr_channels, int desired_channels) {
  return stbi_load(path.c_str(), width, height, nr_channels, desired_channels);
}

bool LoadImage::ConfigureTexture2DWithAutoParams(GLenum target, GLint level,
                                                 int width, int height,
                                                 int nr_components,
                                                 GLint border, GLenum type,
                                                 unsigned char* data,
                                                 GLboolean gamma_correction) {
  if (data) {
    GLenum format = DetermineFormat(nr_components);

    if (gamma_correction) {
      GammaCorrect(data, width, height, 1, nr_components, 2.2f);
    }

    glTexImage2D(target, level, format, width, height, border, format, type,
                 data);
    stbi_image_free(data);
    return true;
  }

  stbi_image_free(data);
  return false;
}
bool LoadImage::ConfigureTexture1DWithAutoParams(GLenum target, GLint level,
                                                 int width, int nr_components,
                                                 GLint border, GLenum type,
                                                 unsigned char* data,
                                                 GLboolean gamma_correction) {
  if (data) {
    GLenum format = DetermineFormat(nr_components);

    if (gamma_correction) {
      GammaCorrect(data, width, 1, 1, nr_components, 2.2);
    }

    glTexImage1D(target, level, format, width, border, format, type, data);
    stbi_image_free(data);
    return true;
  }

  stbi_image_free(data);
  return false;
}

template <typename T>
void LoadImage::GammaCorrect(T* data, int width, int height, int depth,
                             int nr_components, float gamma) {
  int size = width * height * depth * nr_components;
  for (int i = 0; i < size; ++i) {
    // Apply gamma correction to the texture data
    auto value = static_cast<float>(data[i] / 255.0f);
    value = pow(value, gamma);
    data[i] = static_cast<T>(value * 255.0f);
  }
}

template bool LoadImage::ConfigureTexture3D<unsigned char*>(
    GLenum target, GLint level, GLint internal_format, int width, int height,
    int nr_components, const vector<unsigned char*>& depth, GLint border,
    GLenum type, GLboolean gamma_correction);

template bool LoadImage::ConfigureTexture3D<float*>(
    GLenum target, GLint level, GLint internal_format, int width, int height,
    int nr_components, const vector<float*>& depth, GLint border, GLenum type,
    GLboolean gamma_correction);