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

#include "stb_image.h"
#include "FilePathSystem.h"
#include "glad/glad.h"

class LoadImage {
 public:
  GLuint LoadTexture2D(const std::string &path, GLboolean gamma_correction = false);
  
  void OpenStbiYasis();

  static LoadImage &GetInstance();

  LoadImage &operator=(LoadImage &other) = delete;

 private:
  GLuint LoadTexture2DSetting(GLuint &texture_id,
									int width,
									int height,
									int nr_components,
									unsigned char *data,
									GLboolean gamma_correction);
  LoadImage() = default;
  
  LoadImage(LoadImage &other) = default;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_LOADIMAGE_H_
