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

#include "include/OpenGLMessage.h"
#include "include/LoggerSystem.h"
#include <string>
#include "include/OpenGLException.h"

using namespace std;

void OpenGLMessage::OpenGlDebugMessageCallback(GLenum source,
											   GLenum type,
											   GLuint id,
											   GLenum severity,
											   GLsizei length,
											   const GLchar *message,
											   const void *userParam) {
  // ignore non-significant error/warning codes
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

  LoggerSystem &logger_system = LoggerSystem::GetInstance();
  logger_system.Log(LoggerSystem::Level::kInfo,
					string("---------------\n") + string("Debug message (")
						+ to_string(id) + string("): ") +
						message);

  switch (source) {
	case GL_DEBUG_SOURCE_API:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Source: API");
	  break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Source: Window System");
	  break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Source: Shader Compiler");
	  break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Source: Third Party");
	  break;
	case GL_DEBUG_SOURCE_APPLICATION:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Source: Application");
	  break;
	case GL_DEBUG_SOURCE_OTHER:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Source: Other");
	  break;
  }

  switch (type) {
	case GL_DEBUG_TYPE_ERROR:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Error");
	  break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Deprecated Behaviour");
	  break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Undefined Behaviour");
	  break;
	case GL_DEBUG_TYPE_PORTABILITY:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Portability");
	  break;
	case GL_DEBUG_TYPE_PERFORMANCE:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Performance");
	  break;
	case GL_DEBUG_TYPE_MARKER:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Marker");
	  break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Push Group");
	  break;
	case GL_DEBUG_TYPE_POP_GROUP:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Pop Group");
	  break;
	case GL_DEBUG_TYPE_OTHER:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Type: Other");
	  break;
  }

  switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Severity: high");
	  break;
	case GL_DEBUG_SEVERITY_MEDIUM:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Severity: medium");
	  break;
	case GL_DEBUG_SEVERITY_LOW:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Severity: low");
	  break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
	  logger_system.Log(LoggerSystem::Level::kInfo,
						"Severity: notification");
	  break;
  }
}

void OpenGLMessage::EnableOpenGLDebugMessage() {
  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGlDebugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  }
}

GLenum OpenGLMessage::OpenGLCheckError_(const char *file, int line) {
  LoggerSystem &logger_system = LoggerSystem::GetInstance();
  GLenum error_code;
  while ((error_code = glGetError()) != GL_NO_ERROR) {
	std::string error;
	switch (error_code) {
	  case GL_INVALID_ENUM: error = "INVALID_ENUM";
		break;
	  case GL_INVALID_VALUE: error = "INVALID_VALUE";
		break;
	  case GL_INVALID_OPERATION: error = "INVALID_OPERATION";
		break;
	  case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW";
		break;
	  case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW";
		break;
	  case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY";
		break;
	  case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION";
		break;
	}
	logger_system.Log(LoggerSystem::Level::kInfo,
					  error + string("|") + file + string("(") + to_string(line)
						  + string(")"));
  }
  return error_code;
}

OpenGLMessage &OpenGLMessage::GetInstance() {
  static OpenGLMessage instance;
  return instance;
}
void OpenGLMessage::GetOpenGLCheckError(const char* file, int line) {
  OpenGLCheckError_(file,line);
}
