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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_TEXTURELOADER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_TEXTURELOADER_H_

#include <mutex>
#include <string>
#include <vector>
#include "assimp/texture.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "stb_image_write.h"

/**
 * @class TextureLoader
 * @brief A utility class for loading and managing textures in OpenGL.
 * 
 * This class supports various texture types including 1D, 2D, 3D, cube maps, 
 * and texture arrays. It provides functionality to configure texture parameters 
 * and handle gamma correction. It also supports multi sampled textures and 
 * texture loading from images or texture data.
 * 
 * Compared with LoadImage, it is more suitable for the unified management of 
 * OpenGL textures. It is recommended to replace LoadImage wherever you use it.
 */
class TextureLoader {
 public:
  /**
   * @enum Type
   * @brief Enumeration for different texture types.
   */
  enum class Type {
    kTexture1D,                 // 1D texture
    kTexture2D,                 // 2D texture
    kTexture3D,                 // 3D texture
    kTexture1DArray,            // 1D texture array
    kTexture2DArray,            // 2D texture array
    kTextureRectangle,          // rectangle texture
    kCubeMap,                   // cube map texture
    kCubeMapArray,              // cube map array texture
    kTextureBuffer,             // texture buffer
    kTexture2DMultisample,      // 2D multisample texture
    kTexture2DMultisampleArray  // 2D multisample array texture
  };

 public:
  /**
   * @struct TextureConfig
   * @brief Texture properties
   * 
   * This struct holds the configuration parameters for texture wrapping,
   * filtering, and gamma correction.
   */
  struct TextureConfig {
    // S-axis wrapping patterns such as GL_REPEAT, GL_CLAMP_TO_EDGE, and so on.
    GLint wrap_s_mode;
    // T-axis wrapping patterns such as GL_REPEAT, GL_CLAMP_TO_EDGE, and so on.
    GLint wrap_t_mode;
    // R-axis wrapping mode, only used with 3D textures or texture arrays, 
    // such as GL_REPEAT, GL_CLAMP_TO_EDGE, etc.
    GLint wrap_r_mode;
    // Enlarge filter modes such as GL LINEAR, GL NEAREST, etc.
    GLint mag_filter_mode;
    // Narrow down filter patterns such as GL_LINEAR_MIPMAP_LINEAR, 
    // GL_NEAREST, and so on.
    GLint min_filter_mode;
    // Whether to apply gamma correction.
    GLboolean gamma_correction;
    // gamma corrected value.
    float gamma_value;
  };

 public:
  /**
   * @brief Enables vertical flip of images loaded by stb_image.
   * 
   * By default, stb_image loads images with the origin at the top-left. This 
   * method sets the library to flip images vertically on load.
   */
  static void EnableStbImageFlipYAxis();

  /**
   * @brief Disables vertical flip of images loaded by stb_image.
   * 
   * Resets the vertical flip setting of stb_image to its default behavior 
   * (no flip).
   */
  static void DisEnableStbImageFlipYAxis();

  /**
   * Constructs a TextureLoader for a 2D texture from a single image.
   * @param texture_type The type of texture to create.
   * @param path Path to the image file.
   * @param wrap_s_mode Wrapping mode for the S coordinate.
   * @param wrap_t_mode Wrapping mode for the T coordinate.
   * @param mag_filter_mode Magnification filter mode.
   * @param min_filter_mode Minification filter mode.
   * @param gamma_correction Whether to apply gamma correction.
   * @param gamma_value Gamma value for correction.
   */
  TextureLoader(Type texture_type, const std::string& path,
                GLint wrap_s_mode = GL_REPEAT, GLint wrap_t_mode = GL_REPEAT,
                GLint mag_filter_mode = GL_LINEAR_MIPMAP_LINEAR,
                GLint min_filter_mode = GL_LINEAR,
                GLboolean gamma_correction = false, float gamma_value = 2.2f);

  /**
  * Constructs a TextureLoader for a 2D texture array from multiple images.
  * @param texture_type The type of texture to create.
  * @param paths Paths to the images for each layer.
  * @param wrap_s_mode Wrapping mode for the S coordinate.
  * @param wrap_t_mode Wrapping mode for the T coordinate.
  * @param wrap_r_mode Wrapping mode for the R coordinate.
  * @param mag_filter_mode Magnification filter mode.
  * @param min_filter_mode Minification filter mode.
  * @param gamma_correction Whether to apply gamma correction.
  * @param gamma_value Gamma value for correction.
  */
  TextureLoader(Type texture_type, const std::vector<std::string>& paths,
                GLint wrap_s_mode, GLint wrap_t_mode, GLint wrap_r_mode,
                GLint mag_filter_mode, GLint min_filter_mode,
                GLboolean gamma_correction, float gamma_value);

  /**
   * Constructs a TextureLoader for a multi sampled texture.
   * @param texture_type The type of texture to create.
   * @param samples Number of samples for multisampling.
   * @param internalformat Internal format of the texture.
   * @param width Width of the texture.
   * @param height Height of the texture.
   * @param depth Depth of the texture.
   * @param fixed_sample_locations Whether to use fixed sample locations.
   * @param mag_filter_mode Magnification filter mode.
   * @param min_filter_mode Minification filter mode.
   */
  TextureLoader(Type texture_type, GLsizei samples, GLenum internalformat,
                GLsizei width, GLsizei height, GLsizei depth,
                GLboolean fixed_sample_locations, GLint mag_filter_mode,
                GLint min_filter_mode);

  /**
   * Constructs a TextureLoader for a cube map array from multiple images.
   * @param texture_type The type of texture to create.
   * @param paths Paths to the images for each face of each cube map layer.
   * @param wrap_s_mode Wrapping mode for the S coordinate.
   * @param wrap_t_mode Wrapping mode for the T coordinate.
   * @param wrap_r_mode Wrapping mode for the R coordinate.
   * @param mag_filter_mode Magnification filter mode.
   * @param min_filter_mode Minification filter mode.
   * @param gamma_correction Whether to apply gamma correction.
   * @param gamma_value Gamma value for correction.
   */
  TextureLoader(Type texture_type,
                const std::vector<std::vector<std::string>>& paths,
                GLint wrap_s_mode, GLint wrap_t_mode, GLint wrap_r_mode,
                GLint mag_filter_mode, GLint min_filter_mode,
                GLboolean gamma_correction, float gamma_value);

  TextureLoader(Type texture_type, const aiTexture* ai_texture,
                TextureConfig texture_config);

  /**
   * Binds the texture to the specified texture unit.
   * @param texture_unit Texture unit to bind to (default is GL_TEXTURE0).
   */
  void Bind(GLenum texture_unit = GL_TEXTURE0);

  /**
   * Unbinds the texture from its current texture unit.
   */
  void UnBind();

  /**
   * Resets the active texture.
   */
  void ResetActiveTexture();

  /**
   * @brief Destructor for the TextureLoader class.
   * 
   * Deletes the texture from the GPU.
   */
  ~TextureLoader();

  /**
   * Retrieves the texture ID.
   * @return GLuint The OpenGL texture ID.
   */
  GLuint GetTextureId() const;
  
  bool IsEmpty()const;

 private:
  
  void Cleanup();
  /**
   * Converts TextureLoader::Type to GLenum for OpenGL.
   * @param texture_type The texture type.
   * @return GLenum The corresponding OpenGL texture type.
   */
  GLenum GetGLTextureType(Type texture_type) const;

  /**
   * Determines the data format based on the number of color channels.
   * @param nr_channels Number of color channels.
   * @return GLenum The corresponding OpenGL data format.
   */
  GLenum DetermineDataFormat(int nr_channels);

  /**
   * Determines the internal format based on the number of color channels and gamma correction.
   * @param nr_channels Number of color channels.
   * @param gamma_correction Whether gamma correction is applied.
   * @return GLint The corresponding OpenGL internal format.
   */
  GLint DetermineInternalFormat(int nr_channels, bool gamma_correction);

  /**
   * Determines the internal format for HDR textures based on the number of color channels.
   * @param nr_channels Number of color channels.
   * @return GLint The corresponding OpenGL internal format for HDR textures.
   */
  GLint DetermineHDRInternalFormat(int nr_channels);

  /**
   * Loads image data from file.
   * @param is_hdr Whether the image is in HDR format.
   * @param path Path to the image file.
   * @param width Pointer to store the width of the image.
   * @param height Pointer to store the height of the image.
   * @param nr_channels Pointer to store the number of color channels.
   * @param desired_channels Desired number of color channels (0 for no 
   * conversion).
   * @return void* Pointer to the loaded image data.
   */
  void* LoadImageData(bool is_hdr, const std::string& path, int* width,
                      int* height, int* nr_channels, int desired_channels = 0);

  /**
   * Determines if the image at the specified path is an HDR image.
   * @param path Path to the image file.
   * @return bool True if the image is HDR, false otherwise.
   */
  bool IsHDR(const std::string& path);

  /**
   * Configures a multi sampled texture.
   * @param texture_type The texture type.
   * @param samples Number of samples for multisampling.
   * @param internalformat Internal format of the texture.
   * @param width  Width of the texture.
   * @param height Height of the texture.
   * @param depth Depth of the texture.
   * @param fixed_sample_locations Whether to use fixed sample locations.
   * @param mag_filter_mode Magnification filter mode.
   * @param min_filter_mode Minification filter mode.
   * @return GLuint The OpenGL texture ID.
   */
  GLuint ConfigureTextureMultisample(GLenum texture_type, GLsizei samples,
                                     GLenum internalformat, GLsizei width,
                                     GLsizei height, GLsizei depth,
                                     GLboolean fixed_sample_locations,
                                     GLint mag_filter_mode,
                                     GLint min_filter_mode);

  /**
   * Configures texture parameters automatically based on the given path and 
   * configuration.
   * @param path Path to the image file.
   * @param texture_type The texture type.
   * @param texture_config Configuration for the texture.
   * @return GLuint The OpenGL texture ID.
   */
  GLuint ConfigureTextureAutoParams(const std::string& path,
                                    GLenum texture_type,
                                    TextureConfig texture_config);

  /**
   * Configures texture parameters automatically based on the given paths and 
   * configuration.
   * @param paths Paths to the images for each layer.
   * @param texture_type The texture type.
   * @param texture_config Configuration for the texture.
   * @return GLuint The OpenGL texture ID.
   */
  GLuint ConfigureTextureAutoParams(const std::vector<std::string>& paths,
                                    GLenum texture_type,
                                    TextureConfig texture_config);

  /**
   * Configures texture parameters automatically based on the given paths and 
   * configuration.
   * @param paths Paths to the images for each face of each cube map layer.
   * @param texture_type The texture type.
   * @param texture_config Configuration for the texture.
   * @return GLuint The OpenGL texture ID.
   */
  GLuint ConfigureTextureAutoParams(
      const std::vector<std::vector<std::string>>& paths, GLenum texture_type,
      TextureConfig texture_config);

  /**
   * Configures texture parameters for an Assimp texture.
   * @param ai_texture Pointer to the Assimp texture.
   * @param texture_type The texture type.
   * @param texture_config Configuration for the texture.
   * @return GLuint The OpenGL texture ID.
   */
  GLuint ConfigureAssimpTextureAutoParams(const aiTexture* ai_texture,
                                          GLenum texture_type,
                                          TextureConfig texture_config);

  /**
   * Configures texture mipmap settings.
   * @param min_filter_mode Minification filter mode.
   * @param texture_type The texture type.
   */
  void ConfigureTextureMipMap(GLint min_filter_mode, GLenum texture_type);

  /**
   * Applies gamma correction to texture data.
   * @tparam T The type of the texture data.
   * @param data Pointer to the texture data.
   * @param width Width of the texture.
   * @param height Height of the texture.
   * @param depth Depth of the texture.
   * @param nr_components Number of color components.
   * @param gamma Gamma correction value.
   */
  template <typename T>
  void GammaCorrect(T* data, int width, int height, int depth,
                    int nr_components, float gamma);

 private:
  GLuint texture_id_ = 0;// OpenGL texture ID.
  GLenum texture_type_;// The type of the texture.
  std::mutex mutex_;// Mutex for thread safety.
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_TEXTURELOADER_H_
