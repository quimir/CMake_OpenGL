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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_SHADER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_SHADER_H_

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

class Shader {
 private:
  enum class ShaderErrorType {
	kProgram = 0,
	kVertex,
	kFragment,
	kGeometry,
	kCompute,
	kTessellationControl,
	kTessellationEvaluation
  };

 public:
  GLuint GetID() const;

  void Use();

  void SetBool(const std::string &name, bool value) const;

  void SetInt(const std::string &name, int value) const;

  void SetFloat(const std::string &name, float value) const;

  Shader(const char *vertex_path, const char *fragment_path);

  Shader(std::string vertex_path, std::string fragment_path, std::string geometry_path = std::string());

 private:
  static void CheckCompileErrors(GLuint shader, ShaderErrorType error_type);

 private:
  GLuint id_;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_SHADER_H_
