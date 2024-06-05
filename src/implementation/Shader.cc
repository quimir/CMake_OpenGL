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

#include "include/Shader.h"

using namespace std;

constexpr GLuint kInfoLogLength = 1024;

void Shader::CheckCompileErrors(GLuint shader, Shader::ShaderErrorType error_type) {
  GLint success;
  char *info_log = nullptr;
  if (error_type != ShaderErrorType::kProgram) {
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
	  glGetShaderInfoLog(shader, kInfoLogLength, nullptr, info_log);
	  cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << static_cast<int>(error_type) << endl << info_log <<
		   "\n -- --------------------------------------------------- -- " << std::endl;
	}
  } else {
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
	  glGetProgramInfoLog(shader, kInfoLogLength, nullptr, info_log);
	  std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << static_cast<int>(error_type) << "\n" << info_log
				<< "\n -- --------------------------------------------------- -- " << std::endl;
	}
  }
}

GLuint Shader::GetID() const {
  return this->id_;
}

Shader::Shader(const char *vertex_path, const char *fragment_path) {
  // 1. retrieve the vertex/fragment source code from filePath
  string vertex_code;
  string fragment_code;
  ifstream vertex_shader_file;
  ifstream fragment_shader_file;

  // ensure ifstream objects can throw exceptions:
  vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  try {
	// open files
	vertex_shader_file.open(vertex_path);
	fragment_shader_file.open(fragment_path);
	// read file's buffer contents into streams
	stringstream vertex_shader_stream;
	stringstream fragment_shader_stream;
	// close file handlers
	vertex_shader_stream << vertex_shader_file.rdbuf();
	fragment_shader_stream << fragment_shader_file.rdbuf();
	vertex_shader_file.close();
	fragment_shader_file.close();
	// convert stream into string
	vertex_code = vertex_shader_stream.str();
	fragment_code = fragment_shader_stream.str();
  }
  catch (ifstream::failure &e) {
	cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
  }
  const char *vertex_shader_code = vertex_code.c_str();
  const char *fragment_shader_code = fragment_code.c_str();

  // 2. compile shaders
  GLuint vertex;
  GLuint fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_shader_code, NULL);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, ShaderErrorType::kVertex);
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_shader_code, NULL);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, ShaderErrorType::kFragment);
  // shader Program
  this->id_ = glCreateProgram();
  glAttachShader(this->id_, vertex);
  glAttachShader(this->id_, fragment);
  glLinkProgram(this->id_);
  CheckCompileErrors(this->id_, ShaderErrorType::kProgram);
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::Shader(std::string vertex_path,
			   std::string fragment_path,
			   std::string geometry_path) {
  // 1. retrieve the vertex/fragment source code from filePath
  string vertex_code, fragment_code, geometry_code;
  ifstream vertex_shader_file, fragment_shader_file, geometry_shader_file;

  // ensure ifstream objects can throw exceptions:
  vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  if (!geometry_path.empty()) {
	geometry_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  }
  try {
	// open files
	vertex_shader_file.open(vertex_path);
	fragment_shader_file.open(fragment_path);
	if (!geometry_path.empty()) {
	  geometry_shader_file.open(geometry_path);
	}

	// read file's buffer contents into streams
	stringstream vertex_shader_stream, fragment_shader_stream, geometry_shader_stream;
	// close file handlers
	vertex_shader_stream << vertex_shader_file.rdbuf();
	fragment_shader_stream << fragment_shader_file.rdbuf();
	if (!geometry_path.empty()) {
	  geometry_shader_stream << geometry_shader_file.rdbuf();
	}

	vertex_shader_file.close();
	fragment_shader_file.close();
	if (!geometry_path.empty()) {
	  geometry_shader_file.close();
	}
	// convert stream into string
	vertex_code = vertex_shader_stream.str();
	fragment_code = fragment_shader_stream.str();
	if (!geometry_path.empty()) {
	  geometry_code = geometry_shader_stream.str();
	}
  }
  catch (ifstream::failure &e) {
	cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
  }
  const char *vertex_shader_code = vertex_code.c_str();
  const char *fragment_shader_code = fragment_code.c_str();
  const char *geometry_shader_code = geometry_code.c_str();

  // 2. compile shaders
  GLuint vertex, fragment, geometry;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_shader_code, NULL);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, ShaderErrorType::kVertex);
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_shader_code, NULL);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, ShaderErrorType::kFragment);
  if (!geometry_path.empty()) {
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &geometry_shader_code, NULL);
	glCompileShader(geometry);
	CheckCompileErrors(geometry, ShaderErrorType::kGeometry);
  }
  // shader Program
  this->id_ = glCreateProgram();
  glAttachShader(this->id_, vertex);
  glAttachShader(this->id_, fragment);
  if (!geometry_path.empty()) {
	glAttachShader(this->id_, geometry);
  }
  glLinkProgram(this->id_);
  CheckCompileErrors(this->id_, ShaderErrorType::kProgram);
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (!geometry_path.empty()) {
	glDeleteShader(geometry);
  }
}

void Shader::Use() {
  glUseProgram(this->id_);
}

void Shader::SetBool(const string &name, bool value) const {
  glUniform1i(glGetUniformLocation(this->id_, name.c_str()), (int) value);
}

void Shader::SetInt(const string &name, int value) const {
  glUniform1i(glGetUniformLocation(this->id_, name.c_str()), value);
}

void Shader::SetFloat(const string &name, float value) const {
  glUniform1f(glGetUniformLocation(this->id_, name.c_str()), value);
}
