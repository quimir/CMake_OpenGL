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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_FILEPATHSYSTEM_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_FILEPATHSYSTEM_H_

#include <string>
#include <cstdlib>

class FilePathSystem {
 public:
  /**
   * Gets the path of the project in the system.
   * @param path File path, usually enter the name of the file.
   * @return The complete path of the file in the system , is generally the path of the project in the system + the path 
   * of the file.
   */
  std::string GetPath(const std::string &path);

  std::string GetResourcesPath(const std::string &path,
							   const std::string &resources_path =
							   std::string("resources/"));

  static FilePathSystem &GetInstance();

 private:
  FilePathSystem();

  std::string const &GetRoot();

  std::string GetPathRelativeRoot(const std::string &path);

  static std::string GetPathRelativeBinary(const std::string &path);

 private:
  std::string root_;
};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_FILEPATHSYSTEM_H_
