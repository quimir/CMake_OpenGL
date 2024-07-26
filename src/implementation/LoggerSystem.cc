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

#include "include/LoggerSystem.h"
#include <iostream>
#include <sstream>
#include "include/Time/TimeUtils.h"

LoggerSystem& LoggerSystem::GetInstance() {
  static LoggerSystem instance;
  return instance;
}

void LoggerSystem::Log(LoggerSystem::Level level, const std::string& message) {
  std::lock_guard<std::mutex> lock(log_mutex_);
  RotateLogFile();
  std::string formatted_message =
      "[" + LevelToString(level) + "]  " + "[" +
      TimeUtils::GetInstance().FormatTime(
          TimeUtils::GetInstance().GetCurrentTime()) +
      "]  " + "[";
  std::string current_message = message;
  if (log_wrapping_) {
    if (formatted_message.size() + current_message.size() > wrapping_row_) {
      auto formatted_message_size = formatted_message.size();
      formatted_message += current_message.substr(
                               0, wrapping_row_ - formatted_message_size - 1) +
                           "\n";
      current_message = current_message.substr(
          wrapping_row_ - formatted_message_size - 1, current_message.size());
    }
    while (current_message.size() > wrapping_row_) {
      formatted_message += current_message.substr(0, wrapping_row_) + "\n";
      current_message = current_message.substr(wrapping_row_);
    }
  }
  formatted_message += current_message + "]\n";

  if (!log_file_.is_open()) {
    log_file_.open(last_log_file_name_, std::ios_base::app);
  }

  log_file_ << formatted_message;

  SaveLogFileSetting(false, this->log_file_path_);
}

void LoggerSystem::SetMaxSize(std::size_t size) {
  if (size != this->max_size_)
    this->max_size_ = size;
  SaveLogFileSetting(false, this->log_file_path_);
}

void LoggerSystem::SetMaxAge(std::chrono::seconds age) {
  if (age != this->max_age_)
    this->max_age_ = age;
  SaveLogFileSetting(false, this->log_file_path_);
}

LoggerSystem::~LoggerSystem() {
  SaveLogFileSetting(false, this->log_file_path_);
  std::lock_guard<std::mutex> lock(log_mutex_);
  log_file_ << "----------------------------------------------------------\n";
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

std::string LoggerSystem::LevelToString(LoggerSystem::Level level) {
  switch (level) {
    case Level::kInfo:
      return "INFO";
    case Level::kWarning:
      return "WARNING";
    case Level::kError:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

void LoggerSystem::RotateLogFile() {
  if (log_file_.is_open() &&
      std::filesystem::file_size(last_log_file_name_) > max_size_) {
    RollOverLogs();
  }

  if (max_age_ != std::chrono::seconds::max()) {
    auto now = TimeUtils::GetInstance().GetCurrentTime();
    if (std::chrono::duration_cast<std::chrono::seconds>(
            now - start_log_time_) > max_age_) {
      if (log_file_.is_open()) {
        log_file_.close();
      }
      DeleteLogs();
    }
  }
}

LoggerSystem::LoggerSystem(std::size_t size, std::chrono::seconds age,
                           const std::string& log_file_path) {
  Initialized(size, age, log_file_path, false, 0);
}

void LoggerSystem::RollOverLogs() {
  log_file_.close();

  auto log_end_time = TimeUtils::GetInstance().GetCurrentTime();
  std::string new_log_file_name =
      "log_" + TimeUtils::GetInstance().FormatTime(this->start_log_time_) +
      "--" + TimeUtils::GetInstance().FormatTime(log_end_time) + ".log";
  std::filesystem::path new_log_path =
      std::filesystem::path(log_file_path_) / new_log_file_name;

  std::filesystem::rename(last_log_file_name_, new_log_path);

  last_log_file_name_ =
      (std::filesystem::path(log_file_path_) /
       ("log_" + TimeUtils::GetInstance().FormatTime(log_end_time) + ".log"))
          .string();
  log_file_.open(last_log_file_name_, std::ios_base::out);
  if (!log_file_.is_open()) {
    throw std::runtime_error("Unable to open new log file");
  }

  this->start_log_time_ = TimeUtils::GetInstance().GetCurrentTime();
  SaveLogFileSetting(false, this->log_file_path_);
}

void LoggerSystem::DeleteAllLogs() {
  std::filesystem::path log_dir =
      std::filesystem::path(log_file_path_).parent_path();
  if (std::filesystem::exists(log_dir) &&
      std::filesystem::is_directory(log_dir)) {
    std::filesystem::remove_all(log_dir);
  }
}

void LoggerSystem::LoadLogFileSetting() {
  std::ifstream last_log_file(this->log_file_path_ + "last_time_log.txt");
  if (last_log_file.is_open()) {
    std::getline(last_log_file, log_file_path_);
    std::getline(last_log_file, last_log_file_name_);

    std::string max_age_str;
    std::getline(last_log_file, max_age_str);
    max_age_ = std::chrono::seconds(std::stoll(max_age_str));

    std::string max_size_str;
    std::getline(last_log_file, max_size_str);
    max_size_ = std::stoull(max_size_str);

    last_log_file.close();
  } else {
    // File might not exist; handle this case as needed
    last_log_file_name_.clear();
  }
}

void LoggerSystem::SaveLogFileSetting(bool change_dir,
                                      const std::string& log_file_path) {
  std::filesystem::path log_dir(log_file_path);
  // Check and create the log directory
  if (!log_dir.empty() && !std::filesystem::exists(log_dir)) {
    std::filesystem::create_directories(log_dir);
  }
  //  if (change_dir)
  //  std::ofstream last_log_file(log_file_path + "last_time_log.txt");
  std::ofstream last_log_file;
  if (change_dir) {
    last_log_file = std::ofstream(this->log_file_path_ + "last_time_log.txt");
  } else {
    last_log_file = std::ofstream(log_file_path + "last_time_log.txt");
  }
  if (last_log_file.is_open()) {
    last_log_file << log_file_path << "\n";
    last_log_file << last_log_file_name_ << "\n";
    last_log_file
        << std::chrono::duration_cast<std::chrono::seconds>(max_age_).count()
        << "\n";
    last_log_file << max_size_ << "\n";
    last_log_file.close();
  } else {
    throw std::runtime_error("Unable to open file for writing");
  }
}

void LoggerSystem::SetMaxAge(double_t age) {
  auto new_max_age = std::chrono::seconds(static_cast<int>(age));
  if (this->max_age_ != new_max_age) {
    this->max_age_ = new_max_age;
  }
}

void LoggerSystem::EnableLogWrapping(int row) {
  if (!log_wrapping_) {
    log_wrapping_ = true;
  }
  if (row >= 10 && wrapping_row_ != row) {
    wrapping_row_ = row;
  }
}

void LoggerSystem::DisEnableLogWrapping() {
  if (log_wrapping_) {
    log_wrapping_ = false;
  }
}

std::chrono::time_point<std::chrono::system_clock>
LoggerSystem::ExtractFirstTimestamp() {
  // Make sure the file is opened in read mode
  std::ifstream log_file(last_log_file_name_);
  if (!log_file.is_open()) {
    throw std::runtime_error("Unable to open log file for reading");
  }

  // Check if the file is empty
  log_file.seekg(0, std::ios::end);
  if (log_file.tellg() == 0) {
    return TimeUtils::GetInstance().GetCurrentTime();
  }

  log_file.seekg(0, std::ios::beg);
  std::stringstream log_stream;
  log_stream
      << log_file.rdbuf();  // Read the file contents into the string stream

  std::string line;
  while (std::getline(log_stream, line)) {
    // Look for the first '[' and ']', then move on to the second '[' and ']'
    auto first_pos = line.find('[');
    if (first_pos != std::string::npos) {
      auto first_end_pos = line.find(']', first_pos);
      // We're looking twice because we're looking at [Log level] [log time]
      // [log content], so we're looking for the second [] here.
      if (first_end_pos != std::string::npos) {
        // Find the second '[' and ']'
        auto second_pos = line.find('[', first_end_pos + 1);
        if (second_pos != std::string::npos) {
          auto second_end_pos = line.find(']', second_pos);
          if (second_end_pos != std::string::npos) {
            std::string timestamp =
                line.substr(second_pos + 1, second_end_pos - second_pos - 1);
            try {
              return ParseTimestamp(timestamp);
            } catch (const std::runtime_error&) {
              continue;
            }
          }
        }
      }
    }
  }
  throw std::runtime_error("No valid timestamp found in log content");
}

std::chrono::time_point<std::chrono::system_clock> LoggerSystem::ParseTimestamp(
    const std::string& time_stamp) {
  std::tm tm = {};
  std::istringstream ss(time_stamp);
  ss >> std::get_time(&tm, "%Y-%m-%d_%H-%M-%S");
  if (ss.fail()) {
    throw std::runtime_error("Failed to parse timestamp");
  }
  auto time_c = std::mktime(&tm);
  return std::chrono::system_clock::from_time_t(time_c);
}

void LoggerSystem::DeleteLogs() {
  DeleteAllLogs();
  std::filesystem::path log_dir(log_file_path_);

  // Check and create the log directory
  if (!log_dir.empty() && !std::filesystem::exists(log_dir)) {
    std::filesystem::create_directories(log_dir);
  }
  start_log_time_ = TimeUtils::GetInstance().GetCurrentTime();
  // Reopen a new log file
  last_log_file_name_ =
      (std::filesystem::path(log_file_path_) /
       ("log_" + TimeUtils::GetInstance().FormatTime(start_log_time_) + ".log"))
          .string();
  log_file_.open(last_log_file_name_,
                 std::ios_base::out);  // Open a new log file
  if (!log_file_.is_open()) {
    throw std::runtime_error("Unable to open new log file after deletion");
  }
  log_file_ << "----------------------------------------------------------\n";
  SaveLogFileSetting(false, this->log_file_path_);
}

void LoggerSystem::Initialized(std::size_t size, std::chrono::seconds age,
                               const std::string& log_file_path,
                               bool log_wrapping, int wrapping_row) {
  if (log_file_path.empty()) {
    throw std::runtime_error("The log file path is not allowed to be empty.");
  }
  Close();
  log_file_path_ = log_file_path;
  max_size_ = size;
  max_age_ = age;
  wrapping_row_ = wrapping_row;
  log_wrapping_ = log_wrapping;

  try {
    std::filesystem::path log_dir(log_file_path_);

    // Check and create the log directory
    if (!log_dir.empty() && !std::filesystem::exists(log_dir)) {
      std::filesystem::create_directories(log_dir);
    }

    LoadLogFileSetting();

    // Check whether the log file exists, and if it does not, create it
    if (last_log_file_name_.empty() ||
        !std::filesystem::exists(last_log_file_name_)) {
      last_log_file_name_ =
          (log_dir / ("log_" +
                      TimeUtils::GetInstance().FormatTime(
                          TimeUtils::GetInstance().GetCurrentTime()) +
                      ".log"))
              .string();
      log_file_.open(last_log_file_name_, std::ios_base::out);
      if (!log_file_.is_open()) {
        throw std::runtime_error("Unable to open log file");
      }
      this->start_log_time_ = TimeUtils::GetInstance().GetCurrentTime();
    } else {
      log_file_.open(last_log_file_name_, std::ios_base::app);
      if (!log_file_.is_open()) {
        throw std::runtime_error("Unable to open log file");
      }
      this->start_log_time_ = ExtractFirstTimestamp();
    }
  } catch (const std::exception& e) {
    std::cerr << "Error initializing Logger: " << e.what() << std::endl;
  }
  log_file_ << "----------------------------------------------------------\n";
}

const std::string& LoggerSystem::GetLogFilePath() const {
  return log_file_path_;
}

void LoggerSystem::SetLogFilePath(const std::string& log_file_path) {
  SaveLogFileSetting(true, log_file_path);
  log_file_path_ = log_file_path;
  last_log_file_name_.clear();
  Initialized(max_size_, max_age_, log_file_path_, log_wrapping_,
              wrapping_row_);
}

void LoggerSystem::Reset(std::size_t size, std::chrono::seconds age,
                         const std::string& log_file_path, bool log_wrapping,
                         int wrapping_row) {
  Initialized(size, age, log_file_path, log_wrapping, wrapping_row);
}

void LoggerSystem::Close() {
  std::lock_guard<std::mutex> lock(log_mutex_);
  if (log_file_.is_open()) {
    log_file_.close();
  }
}
