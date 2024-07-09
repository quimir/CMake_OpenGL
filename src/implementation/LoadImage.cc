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
  LoadTexture2DSetting(texture, width, height, nr_channels, data,
                       gamma_correction);
  if (texture != kLoadImageError) {
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

void LoadImage::LoadTexture2DSetting(GLuint& texture_id, int width, int height,
                                     int nr_components, unsigned char* data,
                                     GLboolean gamma_correction) {
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

    glBindTexture(GL_TEXTURE_2D, texture_id);
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

    // Register the image information in OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    // Enable the mipmap property on the image
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }

  stbi_image_free(data);
  texture_id = kLoadImageError;
}
void LoadImage::OpenStbImageFlipYAxis() {
  stbi_set_flip_vertically_on_load(true);
}
