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

#include "FilePathSystem.h"
#include "root_directory.h"
#ifdef _WIN32
#include "Windows.h"
#else
#include <unistd.h>
#include <limits.h>
#endif

std::once_flag FilePathSystem::initialized_;
FilePathSystem* FilePathSystem::instance_ = nullptr;

std::string FilePathSystem::GetPath(const std::string& path) {
  if (!this->root_.empty())
    return GetPathRelativeRoot(path);
  else
    return GetPathRelativeBinary(path);
}

FilePathSystem& FilePathSystem::GetInstance() {
  if (instance_ == nullptr) {
    std::call_once(initialized_, []() { instance_ = new FilePathSystem(); });
  }
  return *instance_;
}

FilePathSystem::FilePathSystem() {
  char const* env_root = getenv("LOGL_ROOT_PATH");
  char const* given_root = (env_root != nullptr ? env_root : logl_root);
  this->root_ = (given_root != nullptr ? given_root : "");
}
std::string const& FilePathSystem::GetRoot() {
  return this->root_;
}

std::string FilePathSystem::GetPathRelativeRoot(const std::string& path) {
  return GetRoot() + std::string("/") + path;
}

std::string FilePathSystem::GetPathRelativeBinary(const std::string& path) {
  return "../../" + path;
}
std::string FilePathSystem::GetResourcesPath(
    const std::string& path, const std::string& resources_path) {
  if (!this->root_.empty()) {
    return GetPathRelativeRoot(resources_path + path);
  } else {
    return GetPathRelativeBinary(resources_path + path);
  }
}
FilePathSystem::~FilePathSystem() {
  delete instance_;
}
std::string FilePathSystem::GetGLSLPath(const std::string& path) {
  return GetResourcesPath("glsl/" + path);
}
std::string FilePathSystem::GetExecutablePath() const {
  char buffer[1024];
#ifdef _WIN32
  GetModuleFileNameA(nullptr, buffer, sizeof(buffer));
#else
  ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
  if (len != -1) {
    buffer[len] = '\0';
  }
#endif
  std::string path(buffer);
  return path.substr(0, path.find_last_of("/\\"));
}
std::string FilePathSystem::GetExecutablePath(
    const std::string& name) const {
  return GetExecutablePath() + "/" + name;
}
