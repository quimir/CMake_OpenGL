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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_FONTS_LANGUAGE_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_FONTS_LANGUAGE_H_

#include <string>
#include <unordered_map>

class Language{
 public:
  enum class Type{
    kEnglish,
    kChinese
  };
  
 public:
  static Language& GetInstance();
  
  Type CurrentLanguage()const;

  std::string Translate(const std::string& key);
  
  void AddNewTranslation(const std::string &key,const std::string & value);
  
 private:
  explicit Language(const std::string& file_path);
  
 private:
  std::string file_path_;
  std::unordered_map<std::string,std::string> language_map_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_FONTS_LANGUAGE_H_
