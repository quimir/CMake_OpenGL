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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLEXCEPTION_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLEXCEPTION_H_

#include "glad/glad.h"
#include <exception>
#include <string>

#include "Exception.h"
#include "Core/MacroDefinition.h"

class OpenGLException : public Exception {
 public:
  OpenGLException(LoggerSystem::Level level, const std::string& message,
                  GLenum error_code = UINT_MAX);

  GLenum GetErrorCode() const;

 private:
  GLenum error_code_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLEXCEPTION_H_
