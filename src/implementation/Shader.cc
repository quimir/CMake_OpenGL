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

void Shader::CheckCompileErrors(GLuint shader,
                                Shader::ShaderErrorType error_type) {
  GLint success;
  char* info_log = nullptr;
  if (error_type != ShaderErrorType::kProgram) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      info_log = new char[kInfoLogLength];
      glGetShaderInfoLog(shader, kInfoLogLength, nullptr, info_log);
      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kError,
          string("ERROR::SHADER_COMPILATION_ERROR of type: ") +
              ShaderErrorTypeToString(error_type) + string("\n") + info_log +
              string(
                  "\n -- --------------------------------------------------- "
                  "-- "));
      delete info_log;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      info_log = new char[kInfoLogLength];
      glGetProgramInfoLog(shader, 1024, nullptr, info_log);
      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kError,
          string("ERROR::PROGRAM_LINKING_ERROR of type: ") +
              ShaderErrorTypeToString(error_type) + string("\n") + info_log +
              string(
                  "\n -- --------------------------------------------------- "
                  "-- "));
      delete info_log;
    }
  }
}

GLuint Shader::GetID() const {
  return this->id_;
}

Shader::Shader(const char* vertex_path, const char* fragment_path,
               const char* geometry_path, const char* tess_control_path,
               const char* tess_evaluation_path, const char* compute_path) {
  if (glGetString(GL_VERSION) == nullptr) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }
  if (nullptr != compute_path) {
    CheckOpenGLVersion(4, 3);
  }
  if (nullptr != tess_control_path || nullptr != tess_evaluation_path) {
    CheckOpenGLVersion(4, 0);
  }
  // 1. retrieve the vertex/fragment source code from filePath
  string vertex_code;
  string fragment_code;
  string geometry_code;
  string compute_code;
  string tess_control_code;
  string tess_evaluation_code;
  ifstream vertex_shader_file;
  ifstream fragment_shader_file;
  ifstream geometry_shader_file;
  ifstream compute_shader_file;
  ifstream tess_control_shader_file;
  ifstream tess_evaluation_shader_file;

  // ensure ifstream objects can throw exceptions:
  vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  if (nullptr != geometry_path) {
    geometry_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  }
  if (nullptr != compute_path) {
    compute_shader_file.exceptions(ifstream::failbit | ifstream ::badbit);
  }
  if (nullptr != tess_control_path) {
    tess_control_shader_file.exceptions(ifstream::failbit | ifstream ::badbit);
  }
  if (nullptr != tess_evaluation_path) {
    tess_evaluation_shader_file.exceptions(ifstream ::failbit |
                                           ifstream ::badbit);
  }
  try {
    // open files
    vertex_shader_file.open(vertex_path);
    fragment_shader_file.open(fragment_path);
    if (nullptr != geometry_path) {
      geometry_shader_file.open(geometry_path);
    }
    if (nullptr != compute_path) {
      compute_shader_file.open(compute_path);
    }
    if (nullptr != tess_control_path) {
      tess_control_shader_file.open(tess_control_path);
    }
    if (nullptr != tess_evaluation_path) {
      tess_evaluation_shader_file.open(tess_evaluation_path);
    }
    // read file's buffer contents into streams
    stringstream vertex_shader_stream;
    stringstream fragment_shader_stream;
    stringstream geometry_shader_stream;
    stringstream compute_shader_stream;
    stringstream tess_control_shader_stream;
    stringstream tess_evaluation_shader_stream;
    // close file handlers
    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();
    if (nullptr != geometry_path) {
      geometry_shader_stream << geometry_shader_file.rdbuf();
    }
    if (nullptr != compute_path) {
      compute_shader_stream << compute_shader_file.rdbuf();
    }
    if (nullptr != tess_control_path) {
      tess_control_shader_stream << tess_control_shader_file.rdbuf();
    }
    if (nullptr != tess_evaluation_path) {
      tess_evaluation_shader_stream << tess_evaluation_shader_file.rdbuf();
    }
    vertex_shader_file.close();
    fragment_shader_file.close();
    if (nullptr != geometry_path) {
      geometry_shader_file.close();
    }
    if (nullptr != compute_path) {
      compute_shader_file.close();
    }
    if (nullptr != tess_control_path) {
      tess_control_shader_file.close();
    }
    if (nullptr != tess_evaluation_path) {
      tess_evaluation_shader_file.close();
    }
    // convert stream into string
    vertex_code = vertex_shader_stream.str();
    fragment_code = fragment_shader_stream.str();
    if (nullptr != geometry_path) {
      geometry_code = geometry_shader_stream.str();
    }
    if (nullptr != compute_path) {
      compute_code = compute_shader_stream.str();
    }
    if (nullptr != tess_control_path) {
      tess_control_code = tess_control_shader_stream.str();
    }
    if (nullptr != tess_evaluation_path) {
      tess_evaluation_code = tess_evaluation_shader_stream.str();
    }
  } catch (ifstream::failure& e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << endl;
  }
  const char* vertex_shader_code = vertex_code.c_str();
  const char* fragment_shader_code = fragment_code.c_str();
  const char* geometry_shader_code = geometry_code.c_str();
  const char* compute_shader_code = compute_code.c_str();
  const char* tess_control_shader_code = tess_control_code.c_str();
  const char* tess_evaluation_shader_code = tess_evaluation_code.c_str();

  // 2. compile shaders
  GLuint vertex;
  GLuint fragment;
  GLuint geometry;
  GLuint compute;
  GLuint tess_control;
  GLuint tess_evaluation;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, ShaderErrorType::kVertex);
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, ShaderErrorType::kFragment);
  // geometru shader
  if (nullptr != geometry_path) {
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &geometry_shader_code, nullptr);
    glCompileShader(geometry);
    CheckCompileErrors(geometry, ShaderErrorType::kGeometry);
  }

  if (nullptr != compute_path) {
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &compute_shader_code, nullptr);
    glCompileShader(compute);
    CheckCompileErrors(compute, ShaderErrorType::kCompute);
  }

  if (nullptr != tess_evaluation_path) {
    tess_evaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tess_evaluation, 1, &tess_evaluation_shader_code, nullptr);
    glCompileShader(tess_evaluation);
    CheckCompileErrors(tess_evaluation,
                       ShaderErrorType::kTessellationEvaluation);
  }

  if (nullptr != tess_control_path) {
    tess_control = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tess_control, 1, &tess_control_shader_code, nullptr);
    glCompileShader(tess_control);
    CheckCompileErrors(tess_control, ShaderErrorType::kTessellationControl);
  }
  // shader Program
  this->id_ = glCreateProgram();
  glAttachShader(this->id_, vertex);
  glAttachShader(this->id_, fragment);
  if (nullptr != geometry_path) {
    glAttachShader(this->id_, geometry);
  }
  if (nullptr != compute_path) {
    glAttachShader(this->id_, compute);
  }
  if (nullptr != tess_control_path) {
    glAttachShader(this->id_, tess_control);
  }
  if (nullptr != tess_evaluation_path) {
    glAttachShader(this->id_, tess_evaluation);
  }
  glLinkProgram(this->id_);
  CheckCompileErrors(this->id_, ShaderErrorType::kProgram);
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (nullptr != geometry_path) {
    glDeleteShader(geometry);
  }
  if (nullptr != compute_path) {
    glDeleteShader(compute);
  }
  if (nullptr != tess_control_path) {
    glDeleteShader(tess_control);
  }
  if (nullptr != tess_evaluation_path) {
    glDeleteShader(tess_evaluation);
  }
}

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path,
               const std::string& geometry_path,
               const std::string& tess_control_path,
               const std::string& tess_evaluation_path,
               const std::string& compute_path) {
  if (glGetString(GL_VERSION) == nullptr) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Serious error! Initialize OpenGL before building shaders!");
    throw std::runtime_error(
        "Serious error! Initialize OpenGL before building shaders!");
  }
  if (!compute_path.empty()) {
    CheckOpenGLVersion(4, 3);
  }
  if (!tess_control_path.empty() || !tess_evaluation_path.empty()) {
    CheckOpenGLVersion(4, 0);
  }
  // 1. retrieve the vertex/fragment source code from filePath
  string vertex_code, fragment_code, geometry_code, compute_code,
      tess_control_code, tess_evaluation_code;
  ifstream vertex_shader_file, fragment_shader_file, geometry_shader_file,
      compute_shader_file, tess_control_shader_file,
      tess_evaluation_shader_file;

  // ensure ifstream objects can throw exceptions:
  vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  if (!geometry_path.empty()) {
    geometry_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  }
  if (!compute_path.empty()) {
    compute_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  }
  if (!tess_control_path.empty()) {
    tess_control_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
  }
  if (!tess_evaluation_path.empty()) {
    tess_evaluation_shader_file.exceptions(ifstream::failbit |
                                           ifstream::badbit);
  }
  try {
    // open files
    vertex_shader_file.open(vertex_path);
    fragment_shader_file.open(fragment_path);
    if (!geometry_path.empty()) {
      geometry_shader_file.open(geometry_path);
    }
    if (!compute_path.empty()) {
      compute_shader_file.open(compute_path);
    }
    if (!geometry_path.empty()) {
      geometry_shader_file.open(geometry_path);
    }
    if (!tess_control_path.empty()) {
      tess_control_shader_file.open(tess_control_path);
    }
    if (!tess_evaluation_path.empty()) {
      tess_evaluation_shader_file.open(tess_evaluation_path);
    }

    // read file's buffer contents into streams
    stringstream vertex_shader_stream, fragment_shader_stream,
        geometry_shader_stream, compute_shader_stream,
        tess_control_shader_stream, tess_evaluation_shader_stream;
    // close file handlers
    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();
    if (!geometry_path.empty()) {
      geometry_shader_stream << geometry_shader_file.rdbuf();
    }
    if (!compute_path.empty()) {
      compute_shader_stream << compute_shader_file.rdbuf();
    }
    if (!tess_control_path.empty()) {
      tess_control_shader_stream << tess_control_shader_file.rdbuf();
    }
    if (!tess_evaluation_path.empty()) {
      tess_evaluation_shader_stream << tess_evaluation_shader_file.rdbuf();
    }

    vertex_shader_file.close();
    fragment_shader_file.close();
    if (!geometry_path.empty()) {
      geometry_shader_file.close();
    }
    if (!compute_path.empty()) {
      compute_shader_file.close();
    }
    if (!tess_control_path.empty()) {
      tess_control_shader_file.close();
    }
    if (!tess_evaluation_path.empty()) {
      tess_evaluation_shader_file.close();
    }
    // convert stream into string
    vertex_code = vertex_shader_stream.str();
    fragment_code = fragment_shader_stream.str();
    if (!geometry_path.empty()) {
      geometry_code = geometry_shader_stream.str();
    }
    if (!compute_path.empty()) {
      compute_code = compute_shader_stream.str();
    }
    if (!tess_control_path.empty()) {
      tess_control_code = tess_control_shader_stream.str();
    }
    if (!tess_evaluation_path.empty()) {
      tess_evaluation_code = tess_evaluation_shader_stream.str();
    }
  } catch (ifstream::failure& e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << endl;
  }
  const char* vertex_shader_code = vertex_code.c_str();
  const char* fragment_shader_code = fragment_code.c_str();
  const char* geometry_shader_code = geometry_code.c_str();
  const char* compute_shader_code = compute_code.c_str();
  const char* tess_control_shader_code = tess_control_code.c_str();
  const char* tess_evaluation_shader_code = tess_evaluation_code.c_str();

  // 2. compile shaders
  GLuint vertex;
  GLuint fragment;
  GLuint geometry;
  GLuint compute;
  GLuint tess_control;
  GLuint tess_evaluation;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, ShaderErrorType::kVertex);
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, ShaderErrorType::kFragment);
  // geometry shader
  if (!geometry_path.empty()) {
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &geometry_shader_code, nullptr);
    glCompileShader(geometry);
    CheckCompileErrors(geometry, ShaderErrorType::kGeometry);
  }
  // compute shader
  if (!compute_path.empty()) {
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &compute_shader_code, nullptr);
    glCompileShader(compute);
    CheckCompileErrors(compute, ShaderErrorType::kCompute);
  }
  // tess evaluation shader
  if (!tess_evaluation_path.empty()) {
    tess_evaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tess_evaluation, 1, &tess_evaluation_shader_code, nullptr);
    glCompileShader(tess_evaluation);
    CheckCompileErrors(tess_evaluation,
                       ShaderErrorType::kTessellationEvaluation);
  }

  if (!tess_control_path.empty()) {
    tess_control = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tess_control, 1, &tess_control_shader_code, nullptr);
    glCompileShader(tess_control);
    CheckCompileErrors(tess_control, ShaderErrorType::kTessellationControl);
  }
  // shader Program
  this->id_ = glCreateProgram();
  glAttachShader(this->id_, vertex);
  glAttachShader(this->id_, fragment);
  if (!geometry_path.empty()) {
    glAttachShader(this->id_, geometry);
  }
  if (!compute_path.empty()) {
    glAttachShader(this->id_, compute);
  }
  if (!tess_control_path.empty()) {
    glAttachShader(this->id_, tess_control);
  }
  if (!tess_evaluation_path.empty()) {
    glAttachShader(this->id_, tess_evaluation);
  }
  glLinkProgram(this->id_);
  CheckCompileErrors(this->id_, ShaderErrorType::kProgram);
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (!geometry_path.empty()) {
    glDeleteShader(geometry);
  }
  if (!compute_path.empty()) {
    glDeleteShader(compute);
  }
  if (!tess_control_path.empty()) {
    glDeleteShader(tess_control);
  }
  if (!tess_evaluation_path.empty()) {
    glDeleteShader(tess_evaluation);
  }
}

void Shader::Bind() {
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
  if (uniform_warnings_.find(uniform_name) != uniform_warnings_.end()) {
    return -1;
  }

  GLint location = glGetUniformLocation(this->id_, uniform_name.c_str());
  if (location == -1) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Uniform " + uniform_name + " does not exist in the shader program.");
    uniform_warnings_.insert(uniform_name);
    return -1;
  }

  return location;
}
GLuint Shader::CheckUniformBlockExists(const string& block_name) {
  if (uniform_block_warnings_.find(block_name) !=
      uniform_block_warnings_.end()) {
    return GL_INVALID_INDEX;
  }

  GLuint block_index = glGetUniformBlockIndex(this->id_, block_name.c_str());
  if (block_index == GL_INVALID_INDEX) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Uniform block" + block_name +
            " does not exist in the shader program.");
    uniform_block_warnings_.insert(block_name);
    return GL_INVALID_INDEX;
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
void Shader::UnBind() {
  glUseProgram(0);
}
Shader::~Shader() {
  UnBind();
  glDeleteShader(this->id_);
}
void Shader::CheckOpenGLVersion(int major_number, int minor_number) const {
  int major, minor;
  // Get the registered version of OpenGL
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  if (major < major_number || (major == major_number && minor < minor_number)) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                    "OpenGl " + std::to_string(major_number) +
                                        "." + std::to_string(minor_number) +
                                        "or higher is required for shaders.");
    throw std::runtime_error("OpenGl " + std::to_string(major_number) + "." +
                             std::to_string(minor_number) +
                             "or higher is required for shaders.");
  }
}

void Shader::SetDispatchCompute(GLuint num_groups_x, GLuint num_groups_y,
                                GLuint num_groups_z) {
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}
void Shader::SetMemoryBarrier(GLbitfield barriers) {
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
