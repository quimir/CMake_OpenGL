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

#include "Exception.h"
Exception::Exception(LoggerSystem::Level level, const std::string& message)
    : level_(level), message_(message) {
  LoggerSystem::GetInstance().Log(level, message);
  message_ =LoggerSystem::GetInstance().LevelToString(level_)+" " + message_;
}
#ifdef __MINGW32__
const char* Exception::what() const noexcept {
  return message_.c_str();
}
#endif

#ifdef _MSC_VER
const char* Exception::what() const {
  return message_.c_str();
}
#endif
