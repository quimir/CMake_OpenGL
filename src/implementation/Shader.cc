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
#include "include/OpenGLException.h"
#include "include/OpenGLStateManager.h"
#include <vector>
#include "include/ImGui/OpenGLLogMessage.h"

using namespace std;

std::mutex Shader::gl_mutex_;

bool Shader::use_check_ = false;

void Shader::CheckCompileErrors(GLuint shader,
                                Shader::ShaderErrorType error_type) {
  std::lock_guard<std::mutex> lock(gl_mutex_);
  GLint success;

  if (error_type != ShaderErrorType::kProgram) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      // Create a buffer of appropriate size
      GLint log_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
      vector<char> info_log(log_length);
      glGetShaderInfoLog(shader, log_length, nullptr, info_log.data());
      throw OpenGLException(
          LoggerSystem::Level::kError,
          "ERROR::SHADER_COMPILATION_ERROR of type: " +
              Shader::ShaderErrorTypeToString(error_type) +
              " cause of error: " + std::string(info_log.data()));
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      GLint log_length = 0;
      glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &log_length);
      std::vector<char> info_log(log_length);
      glGetProgramInfoLog(shader, log_length, nullptr, info_log.data());
      throw OpenGLException(
          LoggerSystem::Level::kError,
          "ERROR::PROGRAM_LINKING_ERROR of type: " +
              Shader::ShaderErrorTypeToString(error_type) +
              " cause of error: " + std::string(info_log.data()));
    }
  }
}

GLuint Shader::GetID() const {
  return this->id_;
}

Shader::Shader(const char* vertex_path, const char* fragment_path,
               const char* geometry_path, const char* tess_control_path,
               const char* tess_evaluation_path, const char* compute_path)
    : id_(0) {
  string vertex_path_string(vertex_path);
  string fragment_path_string(fragment_path);
  string geometry_path_string;
  if (geometry_path != nullptr) {
    geometry_path_string.assign(geometry_path);
  }
  string tess_control_path_string;
  if (tess_control_path != nullptr) {
    tess_control_path_string.assign(tess_control_path);
  }
  string tess_evaluation_path_string;
  if (tess_evaluation_path != nullptr) {
    tess_evaluation_path_string.assign(tess_evaluation_path);
  }
  string compute_path_string;
  if (compute_path != nullptr) {
    compute_path_string.assign(compute_path);
  }
  Initialized(vertex_path_string, fragment_path_string, geometry_path_string,
              tess_control_path_string, tess_evaluation_path_string,
              compute_path_string);
}

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path,
               const std::string& geometry_path,
               const std::string& tess_control_path,
               const std::string& tess_evaluation_path,
               const std::string& compute_path)
    : id_(0) {
  Initialized(vertex_path, fragment_path, geometry_path, tess_control_path,
              tess_evaluation_path, compute_path);
}

void Shader::Use() const {
  std::lock_guard<std::mutex> lock(gl_mutex_);
  glUseProgram(this->id_);
}

void Shader::SetBool(const string& name, bool value) {
  glUniform1i(CheckUniformExists(name), (int)value);
}

void Shader::SetInt(const string& name, int value) {
  glUniform1i(CheckUniformExists(name), value);
}

void Shader::SetFloat(const string& name, float value) {
  glUniform1f(CheckUniformExists(name), value);
}

std::string Shader::ShaderErrorTypeToString(Shader::ShaderErrorType type) {
  switch (type) {
    case ShaderErrorType::kProgram:
      return "Program";
    case ShaderErrorType::kVertex:
      return "Vertex";
    case ShaderErrorType::kFragment:
      return "Fragment";
    case ShaderErrorType::kGeometry:
      return "Geometry";
    case ShaderErrorType::kCompute:
      return "Compute";
    case ShaderErrorType::kTessellationControl:
      return "TessellationControl";
    case ShaderErrorType::kTessellationEvaluation:
      return "TessellationEvaluation";
    default:
      return "UNDEFINED ERROR";
  }
}

GLint Shader::CheckUniformExists(const std::string& uniform_name) {
  GLint location = INT_MAX;
  try {
    if (use_check_) {
      Use();
    }
    if (uniform_warnings_.find(uniform_name) != uniform_warnings_.end()) {
      return location;
    }

    location = glGetUniformLocation(this->id_, uniform_name.c_str());
    if (location == -1) {
      CheckActiveUniform(uniform_name);
    }
  } catch (OpenGLException& e) {
    uniform_warnings_.insert(uniform_name);
    std::cerr << "CheckUniform error because: " << e.what() << std::endl;
  }

  return location;
}
GLuint Shader::CheckUniformBlockExists(const string& block_name) {
  GLuint block_index = UINT_MAX;
  try {
    if (use_check_) {
      Use();
    }
    if (uniform_block_warnings_.find(block_name) !=
        uniform_block_warnings_.end()) {
      return block_index;
    }

    block_index = glGetUniformBlockIndex(this->id_, block_name.c_str());
    if (block_index == GL_INVALID_INDEX) {
      CheckActiveUniformBlock(block_name);
    }
  } catch (OpenGLException& e) {
    uniform_block_warnings_.insert(block_name);
    std::cerr << "CheckUniform block error because: " << e.what() << std::endl;
  }
  return block_index;
}
void Shader::SetVec2(const string& name, float x, float y) {
  glUniform2f(CheckUniformExists(name), x, y);
}
void Shader::SetVec2(const string& name, const glm::vec2& value) {
  glUniform2fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetVec3(const string& name, float x, float y, float z) {
  glUniform3f(CheckUniformExists(name), x, y, z);
}
void Shader::SetVec3(const string& name, const glm::vec3& value) {
  glUniform3fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetVec4(const string& name, float x, float y, float z, float w) {
  glUniform4f(CheckUniformExists(name), x, y, z, w);
}
void Shader::SetVec4(const string& name, const glm::vec4& value) {
  glUniform4fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetMat2(const string& name, const glm::mat2& mat2) {
  glUniformMatrix2fv(CheckUniformExists(name), 1, GL_FALSE, &mat2[0][0]);
}
void Shader::SetMat3(const string& name, const glm::mat3& mat3) {
  glUniformMatrix3fv(CheckUniformExists(name), 1, GL_FALSE, &mat3[0][0]);
}
void Shader::SetMat4(const string& name, const glm::mat4& mat4) {
  glUniformMatrix4fv(CheckUniformExists(name), 1, GL_FALSE, &mat4[0][0]);
}
void Shader::UnUse() {
  glUseProgram(0);
}
Shader::~Shader() {
  Cleanup();
}

void Shader::SetDispatchCompute(GLuint num_groups_x, GLuint num_groups_y,
                                GLuint num_groups_z) const {
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}
void Shader::SetMemoryBarrier(GLbitfield barriers) const {
  glMemoryBarrier(barriers);
}
void Shader::SetMat2(const string& name, GLsizei count, GLboolean transpose,
                     const GLfloat* value) {
  glUniformMatrix2fv(CheckUniformExists(name), count, transpose, value);
}
void Shader::SetMat2(const string& name, GLsizei count, GLboolean transpose,
                     const glm::mat2& mat2) {
  glUniformMatrix2fv(CheckUniformExists(name), count, transpose, &mat2[0][0]);
}
void Shader::SetVec2(const string& name, GLsizei count, const GLfloat* value) {
  glUniform2fv(CheckUniformExists(name), count, value);
}
void Shader::SetVec2(const string& name, GLsizei count,
                     const glm::vec2& value) {
  glUniform2fv(CheckUniformExists(name), count, &value[0]);
}
void Shader::SetMat3(const string& name, GLsizei count, GLboolean transpose,
                     const GLfloat* value) {
  glUniformMatrix3fv(CheckUniformExists(name), count, transpose, value);
}
void Shader::SetMat3(const std::string& name, GLsizei count,
                     GLboolean transpose, const glm::mat3& mat3) {
  glUniformMatrix3fv(CheckUniformExists(name), count, transpose, &mat3[0][0]);
}
void Shader::SetMat4(const string& name, GLsizei count, GLboolean transpose,
                     const GLfloat* value) {
  glUniformMatrix4fv(CheckUniformExists(name), count, transpose, value);
}
void Shader::SetMat4(const string& name, GLsizei count, GLboolean transpose,
                     const glm::mat4& mat4) {
  glUniformMatrix4fv(CheckUniformExists(name), count, transpose, &mat4[0][0]);
}
void Shader::Initialized(const string& vertex_path, const string& fragment_path,
                         const string& geometry_path,
                         const string& tess_control_path,
                         const string& tess_evaluation_path,
                         const string& compute_path) {
  try {
    Shader::CheckActivatedOpenGL();
    if (!compute_path.empty()) {
      if (!OpenGLStateManager::GetInstance().CheckOpenGLVersion(4, 3)) {
        throw OpenGLException(
            LoggerSystem::Level::kError,
            "The OpenGL version is too early. Upgrade the "
            "OpenGL version and then use the Compute shader.");
      }
    }
    if (!tess_control_path.empty() || !tess_evaluation_path.empty()) {
      if (!OpenGLStateManager::GetInstance().CheckOpenGLVersion(4, 0)) {
        throw OpenGLException(LoggerSystem::Level::kError,
                              "The OpenGL version is too early. Upgrade the "
                              "OpenGL version and then use the tess shader.");
      }
    }
    // 1. retrieve the vertex/fragment source code from filePath
    string vertex_code = ReadShaderFile(vertex_path);
    string fragment_code = ReadShaderFile(fragment_path);
    string geometry_code = ReadShaderFile(geometry_path);
    string compute_code = ReadShaderFile(compute_path);
    string tess_control_code = ReadShaderFile(tess_control_path);
    string tess_evaluation_code = ReadShaderFile(tess_evaluation_path);

    // 2. compile shaders
    GLuint vertex = CompileShader(vertex_code, GL_VERTEX_SHADER);
    GLuint fragment = CompileShader(fragment_code, GL_FRAGMENT_SHADER);
    GLuint geometry = geometry_path.empty()
                          ? 0
                          : CompileShader(geometry_code, GL_GEOMETRY_SHADER);
    GLuint compute = compute_path.empty()
                         ? 0
                         : CompileShader(compute_code, GL_COMPUTE_SHADER);
    GLuint tess_control =
        tess_control_path.empty()
            ? 0
            : CompileShader(tess_control_code, GL_TESS_CONTROL_SHADER);
    GLuint tess_evaluation =
        tess_evaluation_path.empty()
            ? 0
            : CompileShader(tess_evaluation_code, GL_TESS_EVALUATION_SHADER);

    // shader Program
    this->id_ = glCreateProgram();
    glAttachShader(this->id_, vertex);
    glAttachShader(this->id_, fragment);
    if (geometry) {
      glAttachShader(this->id_, geometry);
    }
    if (compute) {
      glAttachShader(this->id_, compute);
    }
    if (tess_control) {
      glAttachShader(this->id_, tess_control);
    }
    if (tess_evaluation) {
      glAttachShader(this->id_, tess_evaluation);
    }
    glLinkProgram(this->id_);
    Shader::CheckCompileErrors(this->id_, ShaderErrorType::kProgram);
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometry) {
      glDeleteShader(geometry);
    }
    if (compute) {
      glDeleteShader(compute);
    }
    if (tess_control) {
      glDeleteShader(tess_control);
    }
    if (tess_evaluation) {
      glDeleteShader(tess_evaluation);
    }
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(
        string("Shader creation failed because: ") + e.what());
#ifdef _DEBUG
    std::cerr << "Shader creation failed because: " << e.what() << std::endl;
#endif
  }
}

void Shader::ResetShader(const string& vertex_path, const string& fragment_path,
                         const string& geometry_path,
                         const string& tess_control_path,
                         const string& tess_evaluation_path,
                         const string& compute_path) {
  Cleanup();

  Initialized(vertex_path, fragment_path, geometry_path, tess_control_path,
              tess_evaluation_path, compute_path);
}

void Shader::EnableUseCheck() {
  if (!use_check_)
    use_check_ = true;
}

void Shader::DisEnableUseCheck() {
  if (use_check_)
    use_check_ = false;
}
void Shader::Cleanup() {
  if (id_ != 0) {
    glDeleteShader(this->id_);
    this->id_ = 0;
  }
  uniform_warnings_.clear();
  uniform_block_warnings_.clear();
}
void Shader::CheckActivatedOpenGL() {
  if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
    throw OpenGLException(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
  }
}
void Shader::SetVec3(const string& name, const GLsizei count,
                     const GLfloat* value) {
  glUniform3fv(CheckUniformExists(name), count, value);
}
void Shader::SetBool(const string& name, bool value) const {
  glUniform1i(CheckUniformExists(name), static_cast<int>(value));
}
GLint Shader::CheckUniformExists(const string& uniform_name) const {
  GLint location = UINT_MAX;
  try {
    if (use_check_) {
      Use();
    }

    location = glGetUniformLocation(this->id_, uniform_name.c_str());
    if (location == -1) {
      CheckActiveUniform(uniform_name);
    }
  } catch (OpenGLException& e) {
    std::cerr << "CheckUniform error because: " << e.what() << std::endl;
  }
  return location;
}
GLuint Shader::CheckUniformBlockExists(const string& block_name) const {
  GLuint block_index = UINT_MAX;
  try {
    if (use_check_) {
      Use();
    }
    if (uniform_block_warnings_.find(block_name) !=
        uniform_block_warnings_.end()) {
      return block_index;
    }

    block_index = glGetUniformBlockIndex(this->id_, block_name.c_str());
    if (block_index == GL_INVALID_INDEX) {
      CheckActiveUniformBlock(block_name);
    }
  } catch (OpenGLException& e) {
    std::cerr << "CheckUniform block error because: " << e.what() << std::endl;
  }
  return block_index;
}
void Shader::SetInt(const string& name, int value) const {
  glUniform1d(CheckUniformExists(name), value);
}
void Shader::SetFloat(const string& name, float value) const {
  glUniform1f(CheckUniformExists(name), value);
}
void Shader::SetVec2(const string& name, float x, float y) const {
  glUniform2f(CheckUniformExists(name), x, y);
}
void Shader::SetVec2(const string& name, GLsizei count,
                     const GLfloat* value) const {
  glUniform2fv(CheckUniformExists(name), count, value);
}
void Shader::SetVec2(const string& name, GLsizei count,
                     const glm::vec2& value) const {
  glUniform2fv(CheckUniformExists(name), count, &value[0]);
}
void Shader::SetVec2(const string& name, const glm::vec2& value) const {
  glUniform2fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetVec3(const string& name, float x, float y, float z) const {
  glUniform3f(CheckUniformExists(name), x, y, z);
}
void Shader::SetVec3(const string& name, const glm::vec3& value) const {
  glUniform3fv(CheckUniformExists(name), 1, &value[0]);
}
void Shader::SetVec3(const string& name, GLsizei count,
                     const GLfloat* value) const {
  glUniform3fv(CheckUniformExists(name), count, value);
}
void Shader::SetMat4(const string& name, const glm::mat4& mat4) const {
  glUniformMatrix4fv(CheckUniformExists(name), 1, GL_FALSE, &mat4[0][0]);
}
void Shader::SetMat4(const string& name, GLsizei count, GLboolean transpose,
                     const GLfloat* value) const {
  glUniformMatrix4fv(CheckUniformExists(name), count, transpose, value);
}
bool Shader::IsEmpty() const {
  return id_ == 0;
}
void Shader::CheckActiveUniform(const std::string& uniform_name) const {
  GLint num_uniforms = INT_MAX;
  glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &num_uniforms);

  for (GLint i = 0; i < num_uniforms; ++i) {
    char active_uniform_name[256];
    GLsizei length;
    GLint size;
    GLenum type;
    glGetActiveUniform(id_, i, sizeof(active_uniform_name), &length, &size,
                       &type, active_uniform_name);
    if (uniform_name == std::string(active_uniform_name)) {
      throw OpenGLException(
          LoggerSystem::Level::kWarning,
          "Shader id: " + to_string(id_) + string(" uniform name: ") +
              uniform_name +
              string(" exists but is not used by the shader (optimized out)."));
    }
  }

  throw OpenGLException(LoggerSystem::Level::kWarning,
                        "Shader id: " + to_string(id_) +
                            " Uniform name: " + uniform_name +
                            " does not exist in the shader program.");
}
void Shader::CheckActiveUniformBlock(const string& uniform_block_name) const {
  GLint num_blocks = 0;
  glGetProgramiv(id_, GL_ACTIVE_UNIFORM_BLOCKS, &num_blocks);
  for (GLint i = 0; i < num_blocks; ++i) {
    char active_block_name[256];
    GLsizei length;
    glGetActiveUniformBlockName(id_, i, sizeof(active_block_name), &length,
                                active_block_name);
    if (uniform_block_name == std::string(active_block_name)) {
      throw OpenGLException(
          LoggerSystem::Level::kWarning,
          string("Shader id: ") + std::to_string(id_) +
              string(" uniform block name ") + uniform_block_name +
              string(" exists but is not used by the shader (optimized out)."));
    }
  }

  throw OpenGLException(LoggerSystem::Level::kWarning,
                        "Shader id: " + std::to_string(id_) +
                            " Uniform block " + uniform_block_name +
                            " does not exist in the shader program.");
}
std::string Shader::ReadShaderFile(const string& path) {
  if (path.empty())
    return "";

  ifstream shader_file;
  shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    shader_file.open(path);
  } catch (const ifstream::failure& e) {
    throw OpenGLException(
        LoggerSystem::Level::kError,
        "Unable to open shader file: " + path + ", error: " + e.what());
  }
  stringstream shader_stream;
  try {
    shader_stream << shader_file.rdbuf();
  } catch (const std::ifstream::failure& e) {
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Cannot read the contents of shader file: " + path +
                              ", error: " + e.what());
  }
  shader_file.close();

  return shader_stream.str();
}
GLuint Shader::CompileShader(const std::string& source_code,
                             GLenum shader_type) {
  GLuint shader = glCreateShader(shader_type);
  const char* source = source_code.c_str();
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  Shader::CheckCompileErrors(shader,
                             Shader::ShaderTypeToShaderErrorType(shader_type));
  return shader;
}
Shader::ShaderErrorType Shader::ShaderTypeToShaderErrorType(
    GLuint shader_type) {
  if (shader_type == GL_VERTEX_SHADER) {
    return Shader::ShaderErrorType::kVertex;
  } else if (shader_type == GL_FRAGMENT_SHADER) {
    return ShaderErrorType::kFragment;
  } else if (shader_type == GL_GEOMETRY_SHADER) {
    return ShaderErrorType::kGeometry;
  } else if (shader_type == GL_COMPUTE_SHADER) {
    return ShaderErrorType::kCompute;
  } else if (shader_type == GL_TESS_CONTROL_SHADER) {
    return ShaderErrorType::kTessellationControl;
  } else if (shader_type == GL_TESS_EVALUATION_SHADER) {
    return ShaderErrorType::kTessellationEvaluation;
  }

  throw OpenGLException(LoggerSystem::Level::kWarning,
                        "The shader could not be found.");
}
