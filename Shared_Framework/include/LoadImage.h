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

#include <mutex>
#include <string>
#include <vector>

#include "assimp/texture.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "Core/MacroDefinition.h"

/**
 * Images are registered with OpenGL and assigned attributes based on their 
 * type.It also uses the singleton pattern, where a unique instance can only 
 * be used via GetInstance().Copy construction and copy construction are 
 * not allowed in this class.
 * 
 * Usage example:
 * @code
 * std::string path="/path/image.png"
 * GLuint opengl_index=LoadImage::GetInstance().LoadTexture2D(path);
 * @endcode
 * 
 * @note This class is thread-safe. The use of an internal mutex ensures that 
 * an instance is generated only once.
 */
class LoadImage {
 public:
  GLuint LoadTexture1D(const std::string& path, GLint wrap_mode = GL_REPEAT,
                       GLint mag_filter_mode = GL_LINEAR_MIPMAP_LINEAR,
                       GLint min_filter_mode = GL_LINEAR,
                       GLboolean gamma_correction = false);

  GLuint LoadTexture1DArray(const std::vector<std::string>& paths,
                            GLint wrapMode, GLint magFilterMode,
                            GLint minFilterMode, GLboolean gammaCorrection);

  /**
   * Register 2D textures into OpenGL. The OpenGL index of the texture is 
   * returned on success; otherwise, a runtime_error exception is thrown.
   * @param path The full path to the file on the system
   * @param gamma_correction Whether gamma correction is enabled or not.
   * @return The OpenGL index of the texture is returned on success; 
   * otherwise, a runtime_error exception is thrown.
   */
  GLuint LoadTexture2D(const std::string& path, GLint wrap_mode = GL_REPEAT,
                       GLint mag_filter_mode = GL_LINEAR_MIPMAP_LINEAR,
                       GLint min_filter_mode = GL_LINEAR,
                       GLboolean gamma_correction = false);

  GLuint LoadTextureAuto2D(const std::string& path, GLint wrap_mode = GL_REPEAT,
                           GLint mag_filter_mode = GL_LINEAR_MIPMAP_LINEAR,
                           GLint min_filter_mode = GL_LINEAR,
                           GLboolean gamma_correction = false);

  GLuint LoadTexture2DFromAssimp(const aiTexture* ai_texture, GLint wrap_mode,
                                 GLint mag_filter_mode, GLint min_filter_mode,
                                 GLboolean gamma_correction = false);

  GLuint LoadTexture2DArray(const std::vector<std::string>& paths,
                            GLint wrapMode, GLint magFilterMode,
                            GLint minFilterMode,
                            GLboolean gammaCorrection = false);

  GLuint LoadTexture3D(const std::vector<std::string>& paths, GLint wrapMode,
                       GLint magFilterMode, GLint minFilterMode,
                       GLboolean gamma_correction = false);

  /**
   * Load the cube map into OpenGL. If an error occurs, it will be displayed in 
   * a log file.
   * @param faces cube map the path of each face. Please store in right, left, 
   * top, bottom, back and front, otherwise the image will be generated out of 
   * place.
   * @param gamma_correction Whether to enable gamma correction for each image.
   * @return If successful, the OpenGL index of the CubeMap is returned.
   */
  GLuint LoadCubeMap(std::vector<std::string> faces,
                     GLint wrap_mode = GL_CLAMP_TO_EDGE,
                     GLint mag_filter_mode = GL_LINEAR,
                     GLint min_filter_mode = GL_LINEAR,
                     GLboolean gamma_correction = false);

  /**
   * Tell stb_image.h to flip loaded texture's on the y-axis.
   */
  void EnableStbImageFlipYAxis();

  void DisEnableStbImageFlipYAxis();

  static LoadImage& GetInstance();

  LoadImage& operator=(LoadImage& other) = delete;

  LoadImage(const LoadImage& other) = delete;

  ~LoadImage() = default;

 private:
  /**
   * specify a two-dimensional texture image.
   * 
   * Please refer to: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
   * @param target Specifies the target texture. Must be GL_TEXTURE_2D, 
   * GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_1D_ARRAY, 
   * GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE, 
   * GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
   * GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
   * GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 
   * or GL_PROXY_TEXTURE_CUBE_MAP.
   * @param index An index of texture targets, with target as the head.
   * @param level Specifies the level-of-detail number. Level 0 is the base 
   * image level. Level n is the nth mipmap reduction image. If target is 
   * GL_TEXTURE_RECTANGLE or GL_PROXY_TEXTURE_RECTANGLE, level must be 0.
   * @param internal_format Specifies the number of color components in the 
   * texture. Must be one of base internal formats given in Table 1, one of the 
   * sized internal formats given in Table 2, or one of the compressed internal 
   * formats given in Table 3, below.
   * @param width Specifies the width of the texture image. All implementations 
   * support texture images that are at least 1024 texels wide.
   * @param height Specifies the height of the texture image, or the number of 
   * layers in a texture array, in the case of the GL_TEXTURE_1D_ARRAY and 
   * GL_PROXY_TEXTURE_1D_ARRAY targets. All implementations support 2D texture 
   * images that are at least 1024 texels high, and texture arrays that are at 
   * least 256 layers deep.
   * @param nr_components Gray level information derived through stb image.
   * @param border This value must be 0.
   * @param type Specifies the data type of the pixel data. The following 
   * symbolic values are accepted: GL_UNSIGNED_BYTE, GL_BYTE, 
   * GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, 
   * GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, 
   * GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV,
   * GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV,
   * GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, 
   * GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, 
   * GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV.
   * @param data Specifies a pointer to the image data in memory.
   * @param gamma_correction Whether to enable gamma correction
   * @return Returns true on success
   */
  bool ConfigureTexture2D(GLenum target, GLint level, GLint internal_format,
                          int width, int height, int nr_components,
                          GLint border, GLenum type, unsigned char* data,
                          GLboolean gamma_correction);

  bool ConfigureTexture2DWithAutoParams(GLenum target, GLint level, int width,
                                        int height, int nr_components,
                                        GLint border, GLenum type,
                                        unsigned char* data,
                                        GLboolean gamma_correction);

  bool ConfigureTexture1D(GLenum target, GLint level, GLint internal_format,
                          int width, int nr_components, GLint border,
                          GLenum type, unsigned char* data,
                          GLboolean gamma_correction);

  bool ConfigureTexture1DWithAutoParams(GLenum target, GLint level, int width,
                                        int nr_components, GLint border,
                                        GLenum type, unsigned char* data,
                                        GLboolean gamma_correction);

  template <typename T>
  bool ConfigureTexture3D(GLenum target, GLint level, GLint internal_format,
                          int width, int height, int nr_components,
                          const std::vector<T>& depth, GLint border,
                          GLenum type, GLboolean gamma_correction);

  template <typename T>
  void GammaCorrect(T* data, int width, int height, int depth,
                    int nr_components, float gamma);

  GLenum DetermineFormat(int nr_channels);

  stbi_uc* LoadImageData(const std::string& path, int* width, int* height,
                         int* nr_channels, int desired_channels = 0);

  LoadImage() = default;

 private:
  static std::once_flag initialized_;
  static LoadImage* instance_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_LOADIMAGE_H_
