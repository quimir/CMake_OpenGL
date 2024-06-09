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

#include <sstream>
#include <utility>
#include "include/LoggerSystem.h"

LoggerSystem &LoggerSystem::GetInstance() {
  static LoggerSystem instance;
  return instance;
}
void LoggerSystem::Log(LoggerSystem::Level level, const std::string &message) {
  std::lock_guard<std::mutex> lock(log_mutex_);
  RotateLogFile();

  log_file_ << "[" << LevelToString(level) << "]" << "  " << "["
			<< GetCurrentTimeString() << "]" << "  " << "[" <<
			message << "]" << std::endl;
}

void LoggerSystem::SetMaxSize(std::size_t size) {
  if (size != this->max_size_)
	this->max_size_ = size;
}

void LoggerSystem::SetMaxAge(std::chrono::hours age) {
  if (age != this->max_age_)
	this->max_age_ = age;
}

std::string LoggerSystem::GetCurrentTimeString() const {
  return FormatTime(GetCurrentTime());
}

LoggerSystem::~LoggerSystem() {
  SaveLastLogFileName();
  if (log_file_.is_open()) {
	log_file_.close();
  }
}

std::string LoggerSystem::LevelToString(LoggerSystem::Level level) {
  switch (level) {
	case Level::kInfo: return "INFO";
	case Level::kWarning: return "WARNING";
	case Level::kError: return "ERROR";
	default: return "UNKNOWN";
  }
}

void LoggerSystem::RotateLogFile() {

  if (log_file_.is_open()
	  && std::filesystem::file_size(last_log_file_name_) > max_size_) {
	RollOverLogs();
  }

  if (max_age_ != std::chrono::hours::zero()) {
	auto now = GetCurrentTime();
	if (std::chrono::duration_cast<std::chrono::hours>(now - last_log_time_)
		> max_age_) {
	  if (log_file_.is_open()) {
		log_file_.close();
	  }
	  DeleteAllLogs();
	  last_log_time_ = now;
	}
  }
}

LoggerSystem::LoggerSystem(std::size_t size, std::chrono::hours age,
						   std::string log_file_path) :
	max_size_(size), max_age_(age), log_file_path_(std::move(log_file_path)) {
  try {
	std::filesystem::path log_dir(log_file_path_);

	// Check and create the log directory
	if (!log_dir.empty() && !std::filesystem::exists(log_dir)) {
	  std::filesystem::create_directories(log_dir);
	}

	LoadLastLogFileName();

	// Check whether the log file exists, and if it does not, create it
	if (last_log_file_name_.empty() || !std::filesystem::exists(last_log_file_name_)) {
	  last_log_file_name_ = (log_dir
		  / ("log_" + FormatTime(std::chrono::system_clock::now()) + ".log")).string();
	  log_file_.open(last_log_file_name_, std::ios_base::out);
	} else {
	  log_file_.open(last_log_file_name_, std::ios_base::app);
	}

	if (!log_file_.is_open()) {
	  throw std::runtime_error("Unable to open log file");
	}

	this->start_log_time_ = GetCurrentTime();
	this->last_log_time_ = this->start_log_time_;
  } catch (const std::exception &e) {
	std::cerr << "Error initializing Logger: " << e.what() << std::endl;
  }
}

void LoggerSystem::RollOverLogs() {
  log_file_.close();

  auto log_end_time = GetCurrentTime();
  std::string new_log_file_name = "log_" + FormatTime(this->start_log_time_) + "--" +
	  FormatTime(log_end_time) + ".log";
  std::filesystem::path
	  new_log_path = std::filesystem::path(log_file_path_) / new_log_file_name;

  std::filesystem::rename(last_log_file_name_, new_log_path);

  last_log_file_name_ = (std::filesystem::path(log_file_path_)
	  / ("log_" + FormatTime(log_end_time) + ".log")).string();
  log_file_.open(last_log_file_name_, std::ios_base::out);
  if (!log_file_.is_open()) {
	throw std::runtime_error("Unable to open new log file");
  }

  this->start_log_time_ = GetCurrentTime();
  SaveLastLogFileName();
}

std::string LoggerSystem::FormatTime(const std::chrono::system_clock::time_point
									 &time_point) const {
  auto now = std::chrono::system_clock::to_time_t(time_point);
  std::tm local_time{};
  localtime_s(&local_time, &now);

  std::ostringstream oss;
  oss << std::put_time(&local_time, "%Y-%m-%d_%H-%M-%S");
  return oss.str();
}

std::chrono::time_point<std::chrono::system_clock> LoggerSystem::GetCurrentTime() const {
  return std::chrono::system_clock::now();
}

void LoggerSystem::DeleteAllLogs() {
  std::filesystem::path
	  log_dir = std::filesystem::path(log_file_path_).parent_path();
  for (const auto &entry : std::filesystem::directory_iterator(
	  log_dir)) {
	if (entry.is_regular_file() && entry.path().extension() == ".log") {
	  std::filesystem::remove(entry.path());
	}
  }
}

void LoggerSystem::LoadLastLogFileName() {
  std::string last_log_file_path = log_file_path_ + "last_time_log.txt";
  if (std::filesystem::exists(last_log_file_path)) {
	std::ifstream last_log_file(last_log_file_path);
	if (last_log_file.is_open()) {
	  std::getline(last_log_file, last_log_file_name_);
	  last_log_file.close();
	}
  } else {
	last_log_file_name_.clear();
  }
}

void LoggerSystem::SaveLastLogFileName() {
  std::ofstream last_log_file(this->log_file_path_ + "last_time_log.txt");
  if (last_log_file.is_open()) {
	last_log_file << last_log_file_name_;
	last_log_file.close();
  }
}

void LoggerSystem::SetMaxAge(double_t age) {
  auto new_max_age = std::chrono::hours(static_cast<int>(age));
  if (this->max_age_ != new_max_age) {
	this->max_age_ = new_max_age;
  }
}
