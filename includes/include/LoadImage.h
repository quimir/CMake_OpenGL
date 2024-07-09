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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_LOADIMAGE_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_LOADIMAGE_H_

#include <string>
#include <vector>

#include "glad/glad.h"
#include "stb_image.h"

/**
 * Images are registered with OpenGL and assigned attributes based on their 
 * type.It also uses the singleton pattern, where a unique instance can only 
 * be used via GetInstance ().Copy construction and copy construction are 
 * not allowed in this class.
 */
class LoadImage {
 public:
  /**
   * Register 2D textures into OpenGL. The OpenGL index of the texture is 
   * returned on success; otherwise, a runtime_error exception is thrown.
   * @param path The full path to the file on the system
   * @param gamma_correction Whether gamma correction is enabled or not.
   * @return The OpenGL index of the texture is returned on success; 
   * otherwise, a runtime_error exception is thrown.
   */
  GLuint LoadTexture2D(const std::string& path,
                       GLboolean gamma_correction = false);

  /**
   * Tell stb_image.h to flip loaded texture's on the y-axis.
   */
  void OpenStbImageFlipYAxis();

  static LoadImage& GetInstance();

  LoadImage& operator=(LoadImage& other) = delete;
  
  LoadImage(const LoadImage& other)=delete;
  
  ~LoadImage()=default;

 private:
  /**
   * Parse the texture information, register the texture information into 
   * OpenGL, and write the registered texture ID to texture_id.
   * @param texture_id Texture ID, which is written to the OpenGL index if the 
   * registration was successful, or kLoadImageError otherwise.
   * @param width Width of texture
   * @param height The length of the texture.
   * @param nr_components Color channel information of the texture.
   * @param data Texture data
   * @param gamma_correction Gamma correction
   */
  void LoadTexture2DSetting(GLuint& texture_id, int width, int height,
                              int nr_components, unsigned char* data,
                              GLboolean gamma_correction);
  LoadImage() = default;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_LOADIMAGE_H_
