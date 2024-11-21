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

#include "ImGui/Fonts/SQLiteDatabase.h"
#include "Exception.h"
SQLiteDatabase::SQLiteDatabase(const std::string& db_name) {}
void SQLiteDatabase::OpenSQL() {
  if (sqlite3_open(db_name_.c_str(), &db_) != SQLITE_OK) {
    throw Exception(
        LoggerSystem::Level::kError,
        "Error opening database: " + std::string(sqlite3_errmsg(db_)));
    db_ = nullptr;
  }
}
void SQLiteDatabase::CloseSQL() {
  if (db_) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}
void SQLiteDatabase::CreateTablesIfNotExists() {}
bool SQLiteDatabase::TableExists(const std::string& table_name) {
  std::string sql =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='" +
      table_name + "';";
  sqlite3_stmt* stmt;
  bool exists=false;
  if (sqlite3_prepare_v2(db_,sql.c_str(),-1,&stmt, nullptr)==SQLITE_OK)
  {
    if (sqlite3_step(stmt)==SQLITE_ROW)
    {
      exists=true;
    }
  }
  sqlite3_finalize(stmt);
  return false;
}
void SQLiteDatabase::CreateLanguagesTable() {}
void SQLiteDatabase::CreateTranslationsTable() {}
