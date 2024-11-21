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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_FILEPATHSYSTEM_INL_
#define CMAKE_OPEN_INCLUDES_INCLUDE_FILEPATHSYSTEM_INL_

#include <memory>
#include <stdexcept>
#include "FilePathSystem.h"

template <typename... Args>
inline std::string FilePathSystem::SplicePath(const std::string& format,
                                       Args... args) {
  size_t size = snprintf(nullptr, 0, format.c_str(), args...) +
                1;  // Extra space for '\0'
  if (size <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}

#endif