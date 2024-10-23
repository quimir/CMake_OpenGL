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

#include <cstdlib>
#include <mutex>
#include <string>

/**
 * File path system, you can get the complete path in the project system. It 
 * works through ${CMAKE_SOURCE_FILE}/configuration/root_directory.h.i, 
 * When CMake is built, root_directory.h.im generates a file called 
 * root_directory.h, which contains the full path of the project on the system. 
 * See /CMakeLists.txt for an implementation. There is no risk of memory leaks, 
 * and the class does not allow copying and copying.It also uses the singleton 
 * pattern, so you have to call GetInstance() to get an instance of it.
 * 
 * Usage example:
 * @code
 * std::string path=FilePathSystem::GetInstance().GetPath(
 * "resource/image.image");
 * std::string path_resource=FilePathSystem::GetInstance().GetResources(
 * "image.image");
 * @endcode
 * 
 * @note This class is thread-safe. The use of an internal mutex ensures that 
 * an instance is generated only once.
 */
class FilePathSystem {
 public:
  /**
   * Gets the path of the project in the system.
   * @param path The relative path of the file in the project. Such as I have a 
   * file : "{the path of the project through the system}/includes/include/
   * FilePathSystem.h" then only need to input "includes/include/
   * FilePathSystem.h".
   * @return The complete path of the file in the system , is generally the path 
   * of the project in the system + the path  of the file.
   */
  std::string GetPath(const std::string& path);

  /**
   * Gets the path to the file resource file.
   * @param path The relative path of the file in the project. When no changes 
   * are made to the resource path.Such as I have a file : "{the path of the 
   * project through the system}/resources/textures/bricks2.jpg" then only need 
   * to input "textures/bricks2.jpg"
   * @param resources_path Resource path, you can change it yourself if not 
   * it's GetPath()+resources/
   * @return The complete path to the resource file on the system.
   */
  std::string GetResourcesPath(
      const std::string& path,
      const std::string& resources_path = std::string("resources/"));
  
  std::string GetGLSLPath(const std::string& path);

  static FilePathSystem& GetInstance();

  FilePathSystem(const FilePathSystem& other) = delete;

  FilePathSystem& operator=(const FilePathSystem& other) = delete;

  std::string const& GetRoot();

  /**
   * The strings are concatenated according to some rule, and the input format 
   * of the rule follows the form of the function snprintf.
   * @tparam Args Multiple parameter templates.
   * @param format The string to concatenate can consist of multiple arguments.
   * @param args Rules for concatenation. Details please refer to:
   * https://cplusplus.com/reference/cstdio/snprintf/
   * @return The concatenated string is returned on success, otherwise an 
   * exception is thrown.
   */
  template <typename... Args>
  std::string SplicePath(const std::string& format, Args... args);

  ~FilePathSystem();

  FilePathSystem& operator=(FilePathSystem& other) = delete;

 private:
  FilePathSystem();

  /**
   * Gets the absolute path of the file on the system.
   * @param path The relative path of the file in the system.
   * @return "{The path of the project through the system}/{path}"
   */
  std::string GetPathRelativeRoot(const std::string& path);

  /**
   *A relative path to get the location of a file in the system. Take the 
   * current file path for example, Its absolute path is "{the path of the 
   * project through the System}/includes/include/FilePathSystem.h" if you want 
   * to use this function to enter as "includes/include/FilePathSystem.h". 
   * That is, it returns "/".
   * 
   * Note that this may not return "/" depending on where the file is in the 
   * project, but "/includes/include" will return something else so you need to 
   * adjust the function. override the function with override if you are 
   * adjusting the path to the file.
   * @param path The relative path of the file in the project.
   * @return "/path" if the file location has not been changed.
   */
  virtual std::string GetPathRelativeBinary(const std::string& path);

 private:
  std::string root_;

  static std::once_flag initialized_;
  static FilePathSystem* instance_;
};

#include "FilePathSystem.inl"

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_FILEPATHSYSTEM_H_
