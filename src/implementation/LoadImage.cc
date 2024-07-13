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
#include "include/LoggerSystem.h"

using namespace std;

constexpr GLuint kLoadImageError = UINT_MAX;

GLuint LoadImage::LoadTexture2D(const std::string& path,
                                GLboolean gamma_correction) {
  GLuint texture;
  glGenTextures(1, &texture);

  // Load image, create texture and generate mipmaps
  int width, height, nr_channels;
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
  glBindTexture(GL_TEXTURE_2D, texture);
  if (LoadTexture2DSetting(GL_TEXTURE_2D, 0, 0, GL_RGBA, width, height,
                           nr_channels, 0, GL_UNSIGNED_BYTE, data,
                           gamma_correction)) {
    /*
	 * Set the texture wrapping parameters 
	 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /*
	 * Set texture filtering parameters
	 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Enable the mipmap property on the image
    glGenerateMipmap(GL_TEXTURE_2D);
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

LoadImage& LoadImage::GetInstance() {
  static LoadImage instance;
  return instance;
}

void LoadImage::OpenStbImageFlipYAxis() {
  stbi_set_flip_vertically_on_load(true);
}
GLuint LoadImage::LoadCubeMap(std::vector<std::string> faces,
                              GLboolean gamma_correction) {
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  int width, height, nr_channels;
  for (unsigned int i = 0; i < faces.size(); ++i) {
    auto* data = stbi_load(faces[i].c_str(), &width, &height, &nr_channels, 0);
    if (!LoadTexture2DSetting(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, 0, GL_RGB,
                              width, height, nr_channels, 0, GL_RGB, data,
                              gamma_correction)) {
      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kWarning,
          "Cube map texture failed to load at path: " + faces[i]);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return texture_id;
}
bool LoadImage::LoadTexture2DSetting(GLenum target, int index, GLint level,
                                     GLint internal_format, int width,
                                     int height, int nr_components,
                                     GLint border, GLenum type,
                                     unsigned char* data,
                                     GLboolean gamma_correction) {
  border = 0;
  if (data) {
    GLenum format;
    if (nr_components == 1)
      format = GL_RED;
    else if (nr_components == 3)
      format = GL_RGB;
    else if (nr_components == 4)
      format = GL_RGBA;

    if (gamma_correction) {
      // Apply gamma correction to the texture data
      for (int i = 0; i < width * height * nr_components; ++i) {
        float value = static_cast<float>(data[i]) / 255.0f;
        value = pow(value, 2.2f);
        data[i] = static_cast<GLuint>(value * 255.0f);
      }
    }

    glTexImage2D(target + index, level, internal_format, width, height, border,
                 format, type, data);
    stbi_image_free(data);
    return true;
  }

  stbi_image_free(data);
  return false;
}
