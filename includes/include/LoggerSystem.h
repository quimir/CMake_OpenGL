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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_LOGGERSYSTEM_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_LOGGERSYSTEM_H_

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>

/**
 * Logger System It is used to collect and store a portion of the information generated 
 * when the program is running. The information type is under the LoggerSystem::level 
 * enumerated class. Generally, the generated directory is /bin/log/*.log. 
 * You can set the time to clear all log files or call the DeleteAllLogs file, 
 * but it is not recommended to run it.
 * 
 * Usage example:
 * @code
 * LoggerSystem &logger = LoggerSystem::GetInstance();
 * logger.SetMaxSize(1025);
 * logger.Log(LoggerSystem::Level::kInfo, "This is an info message");
 * @endcode
 * 
 * @note This class is thread-safe. It uses internal mutexes to protect access 
 * to shared resources.
 */
class LoggerSystem {
 public:
  /**
   * Level of the log
   */
  enum class Level { kInfo, kWarning, kError };

  /**
   * A unique instance of this class calls the function. Since the principle of this 
   * function is to generate logs and logs generally need to be initialized only once 
   * when the program is running, only one instance needs to be generated.
   * @return An instance of this class.
   */
  static LoggerSystem& GetInstance();

  /**
   * Log file message input function. The level of the log and the content of the log 
   * message will be written into the log file. The write format is as 
   * follows: [Log level] [log time] [log content].
   * 
   * Note: This function locks when used, and every time it is called, 
   * it checks whether the current log file has exceeded the specified time or 
   * maximum storage space. Messages are written to the current log file only after 
   * the check is complete.
   * @param level For the Level of the log message, 
   * refer to the enumeration class :LoggerSystem::Level.
   * @param message Details of the log message.
   */
  void Log(Level level, const std::string& message);

  /**
   * Set the maximum file storage limit. By default, the maximum file storage 
   * limit is 1MB. If the maximum file storage limit exceeds 2MB, create a new file.
   * @param size The size of the file, in bytes.
   */
  void SetMaxSize(std::size_t size);

  /**
   * Set the maximum number of days that the file can be saved, 
   * if the number of days exceeds the number of days/log, all will be cleared, 
   * and the default value is 0 days, which means that all files will be saved.
   * @param age The maximum number of days a file can be kept, in hours.
   */
  void SetMaxAge(std::chrono::seconds age);

  void SetMaxAge(double_t age);

 private:
  explicit LoggerSystem(std::size_t size = std::size_t(1024 * 1024),
                        std::chrono::seconds age =
                            std::chrono::seconds(std::chrono::seconds::max()),
                        std::string log_file_path = std::string("log/"));

  ~LoggerSystem();

  std::string GetCurrentTimeToString() const;

  /**
   * Convert the log level to a string format, and convert the format to uppercase 
   * without the k.
   * @param level See LoggerSystem::Level for details on the level of log files.
   * @return Log level string.
   */
  std::string LevelToString(Level level);

  /**
   * Determine if the current state has exceeded the set time and set stored 
   * value. If more than that, throw it to void RollOverLogs() and 
   * void DeleteAllLogs().
   */
  void RotateLogFile();

  void RollOverLogs();

  void DeleteAllLogs();

  void LoadLastLogFileName();

  void SaveLastLogFileName();

 private:
  std::ofstream log_file_;
  std::mutex log_mutex_;
  std::size_t max_size_;
  std::chrono::seconds max_age_;
  // Log file setting
  std::string log_file_path_;
  std::string last_log_file_name_;

  /* 
   * Log Time
   */
  std::chrono::time_point<std::chrono::system_clock> start_log_time_;
  std::chrono::time_point<std::chrono::system_clock> last_log_time_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_LOGGERSYSTEM_H_
