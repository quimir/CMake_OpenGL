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
#include <unordered_set>

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

  void SetBool(const std::string &name, bool value);

  void SetInt(const std::string &name, int value);

  void SetFloat(const std::string &name, float value);

  Shader(const char *vertex_path,
		 const char *fragment_path,
		 const char *geometry_path = nullptr);

  Shader(const std::string &vertex_path,
		 const std::string &fragment_path,
		 const std::string &geometry_path = std::string());

 private:
  void CheckCompileErrors(GLuint shader, ShaderErrorType error_type);

  std::string ShaderErrorTypeToString(ShaderErrorType type);

  GLint CheckUniformExists(const std::string &uniform_name);

  GLuint CheckUniformBlockExists(const std::string &block_name);

 private:
  GLuint id_;
  std::unordered_set<std::string> uniform_warnings_;
  std::unordered_set<std::string> uniform_block_warnings_;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_SHADER_H_
