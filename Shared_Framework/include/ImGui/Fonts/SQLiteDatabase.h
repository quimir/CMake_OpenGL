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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_FONTS_SQLITEDATABASE_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_FONTS_SQLITEDATABASE_H_

#include "string"
#include "sqlite/sqlite3.h"

class SQLiteDatabase {
 public:
  explicit SQLiteDatabase(const std::string& db_name);
  
 private:
  void OpenSQL();
  
  void CloseSQL();
  
  void CreateTablesIfNotExists();
  
  bool TableExists(const std::string& table_name);
  
  void CreateLanguagesTable();
  
  void CreateTranslationsTable();
 private:
  std::string db_name_;
  sqlite3* db_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_FONTS_SQLITEDATABASE_H_
