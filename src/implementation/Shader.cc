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
#include "include/LoggerSystem.h"

using namespace std;

constexpr GLuint kInfoLogLength = 1024;

void Shader::CheckCompileErrors(GLuint shader, Shader::ShaderErrorType error_type) {
  GLint success;
  char *info_log = nullptr;
  if (error_type != ShaderErrorType::kProgram) {
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
	  info_log=new char[kInfoLogLength];
	  glGetShaderInfoLog(shader, kInfoLogLength, nullptr, info_log);
	  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
									  string("ERROR::SHADER_COMPILATION_ERROR of type: ")
										  + ShaderErrorTypeToString(error_type) +
										  string("\n") + info_log + string(
										  "\n -- --------------------------------------------------- -- "));
	}
  } else {
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
	  info_log=new char[kInfoLogLength];
	  glGetProgramInfoLog(shader, 1024, nullptr, info_log);
	  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
									  string("ERROR::PROGRAM_LINKING_ERROR of type: ")
										  + ShaderErrorTypeToString(error_type) +
										  string("\n") + info_log + string(
										  "\n -- --------------------------------------------------- -- "));
	}
  }
}

GLuint Shader::GetID() const {
  return this->id_;
}

Shader::Shader(const char *vertex_path,
			   const char *fragment_path,
			   const char *geometry_path) {
  // 1. retrieve the vertex/fragment source code from filePath
  string vertex_code;
  string fragment_code;
  string geometry_code;
  ifstream vertex_shader_file;
  ifstream fragment_shader_file;
  ifstream geometry_shader_file;

  // ensure ifstream objects can throw exceptions:
  vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  if (nullptr != geometry_path) {
	geometry_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  }
  try {
	// open files
	vertex_shader_file.open(vertex_path);
	fragment_shader_file.open(fragment_path);
	if (nullptr != geometry_path) {
	  geometry_shader_file.open(geometry_path);
	}
	// read file's buffer contents into streams
	stringstream vertex_shader_stream;
	stringstream fragment_shader_stream;
	stringstream geometry_shader_stream;
	// close file handlers
	vertex_shader_stream << vertex_shader_file.rdbuf();
	fragment_shader_stream << fragment_shader_file.rdbuf();
	if (nullptr != geometry_path) {
	  geometry_shader_stream << geometry_shader_file.rdbuf();
	}
	vertex_shader_file.close();
	fragment_shader_file.close();
	if (nullptr != geometry_path) {
	  geometry_shader_file.close();
	}
	// convert stream into string
	vertex_code = vertex_shader_stream.str();
	fragment_code = fragment_shader_stream.str();
	if (nullptr != geometry_path) {
	  geometry_code = geometry_shader_stream.str();
	}
  }
  catch (ifstream::failure &e) {
	std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
  }
  const char *vertex_shader_code = vertex_code.c_str();
  const char *fragment_shader_code = fragment_code.c_str();
  const char *geometry_shader_code = geometry_code.c_str();

  // 2. compile shaders
  GLuint vertex;
  GLuint fragment;
  GLuint geometry;
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
  if (nullptr != geometry_path) {
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &geometry_shader_code, NULL);
	glCompileShader(geometry);
	CheckCompileErrors(geometry, ShaderErrorType::kGeometry);
  }
  // shader Program
  this->id_ = glCreateProgram();
  glAttachShader(this->id_, vertex);
  glAttachShader(this->id_, fragment);
  if (nullptr != geometry_path) {
	glAttachShader(this->id_, geometry);
  }
  glLinkProgram(this->id_);
  CheckCompileErrors(this->id_, ShaderErrorType::kProgram);
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (nullptr != geometry_path) {
	glDeleteShader(geometry);
  }
}

Shader::Shader(const std::string &vertex_path,
			   const std::string &fragment_path,
			   const std::string &geometry_path) {
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
	std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
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

void Shader::SetBool(const string &name, bool value) {
  glUniform1i(CheckUniformExists(name), (int) value);
}

void Shader::SetInt(const string &name, int value) {
  glUniform1i(CheckUniformExists(name), value);
}

void Shader::SetFloat(const string &name, float value) {
  glUniform1f(CheckUniformExists(name), value);
}

std::string Shader::ShaderErrorTypeToString(Shader::ShaderErrorType type) {
  switch (type) {

	case ShaderErrorType::kProgram: return "Program";
	case ShaderErrorType::kVertex: return "Vertex";
	case ShaderErrorType::kFragment: return "Fragment";
	case ShaderErrorType::kGeometry: return "Geometry";
	case ShaderErrorType::kCompute: return "Compute";
	case ShaderErrorType::kTessellationControl: return "TessellationControl";
	case ShaderErrorType::kTessellationEvaluation: return "TessellationEvaluation";
	default: return "UNKNOWN";
  }
}

GLint Shader::CheckUniformExists(const std::string &uniform_name) {
  if (uniform_warnings_.find(uniform_name) != uniform_warnings_.end()) {
	return -1;
  }

  GLint location = glGetUniformLocation(this->id_, uniform_name.c_str());
  if (location == -1) {
	LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
									"Uniform " + uniform_name
										+ " does not exist in the shader program.");
	uniform_warnings_.insert(uniform_name);
	return -1;
  }

  return location;
}
GLuint Shader::CheckUniformBlockExists(const string &block_name) {
  if (uniform_block_warnings_.find(block_name) != uniform_block_warnings_.end()) {
	return GL_INVALID_INDEX;
  }

  GLuint block_index = glGetUniformBlockIndex(this->id_, block_name.c_str());
  if (block_index == GL_INVALID_INDEX) {
	LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
									"Uniform block" + block_name
										+ " does not exist in the shader program.");
	uniform_block_warnings_.insert(block_name);
	return GL_INVALID_INDEX;
  }

  return block_index;
}
void Shader::SetVec2(const string &name, float x, float y) {
  glUniform2f(CheckUniformExists(name), x, y);
}
void Shader::SetVec2(const string &name, const glm::vec2 &value) {
  glUniform2fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetVec3(const string &name, float x, float y, float z) {
  glUniform3f(CheckUniformExists(name), x, y, z);
}
void Shader::SetVec3(const string &name, const glm::vec3 &value) {
  glUniform3fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetVec4(const string &name, float x, float y, float z, float w) {
  glUniform4f(CheckUniformExists(name), x, y, z, w);
}
void Shader::SetVec4(const string &name, const glm::vec4 &value) {
  glUniform4fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetMat2(const string &name, const glm::mat2 &mat2) {
  glUniformMatrix2fv(CheckUniformExists(name), 1, GL_FALSE, &mat2[0][0]);
}
void Shader::SetMat3(const string &name, const glm::mat3 &mat3) {
  glUniformMatrix3fv(CheckUniformExists(name),1,GL_FALSE,&mat3[0][0]);
}
void Shader::SetMat4(const string &name, const glm::mat4 &mat4) {
  glUniformMatrix4fv(CheckUniformExists(name),1,GL_FALSE,&mat4[0][0]);
}
