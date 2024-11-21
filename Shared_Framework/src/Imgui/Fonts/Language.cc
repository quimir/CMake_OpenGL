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

#include "ImGui/Fonts/Language.h"
#include "FilePathSystem.h"
#include "Exception.h"
#include "fstream"
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __LINUX__
#include <cstdlib>
#endif

using namespace std;

Language::Type Language::CurrentLanguage() const {
#ifdef _WIN32
  LANGID lang_id = GetUserDefaultUILanguage();
  switch (lang_id) {
    case 0x0804:
      return Type::kChinese;
    default:
      return Type::kEnglish;
  }
#elif __LINUX__
  const char* lang = std::getenv("LANG");
  if (lang) {
    std::string lang_str(lang);
    if (lang_str.find("zh") != std::string::npos) {
      return Language::Type::kChinese;
    }
  }

  return Type::kEnglish;
#endif
}
Language::Language(const std::string& file_path) {
  try {
    if (file_path.empty()) {
      throw Exception(
          LoggerSystem::Level::kWarning,
          "The language text file failed to be loaded. The path does not "
          "exist.");
    }

    std::ifstream file(file_path);
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string key, value;
      if (std::getline(iss, key, '=') && std::getline(iss, value)) {
        language_map_[key] = value;
      } else {
        throw Exception(LoggerSystem::Level::kWarning,
                        "Error parsing line: " + line);
      }
    }
  } catch (const std::ifstream::failure& e) {
    throw Exception(
        LoggerSystem::Level::kWarning,
        "Unable to open language file: " + file_path + ", error: " + e.what());
  } catch (const Exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
std::string Language::Translate(const string& key) {
  auto item = language_map_.find(key);
  return item != language_map_.end() ? item->second : key;
}
void Language::AddNewTranslation(const string& key, const string& value) {
  try {
    std::ofstream file(file_path_, std::ios_base::app);
    if (file.is_open()) {
      file << key << "=" << value << endl;
      language_map_[key] = value;
    }
  } catch (ofstream::failure& e) {
    std::cerr << e.what() << std::endl;
  }
}
Language& Language::GetInstance() {
  static Language instance(
      FilePathSystem::GetInstance().GetPath("src/text/lang_mapping.txt"));
  return instance;
}
