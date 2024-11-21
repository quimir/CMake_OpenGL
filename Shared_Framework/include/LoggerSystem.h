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
#include <cmath>

#include "Core/MacroDefinition.h"

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

  /**
   * Enable log file wrapping. Once set, log messages will be automatically 
   * wrapped before row.
   * @param row Sets the character length of the newline.
   */
  void EnableLogWrapping(int row);

  /**
   * Disable log newline mode.
   */
  void DisEnableLogWrapping();

  const std::string& GetLogFilePath() const;

  /**
   * Reset the path to the log file. 
   * Note: this resets everything in the logfile.
   * @param log_file_path Log file path
   */
  void SetLogFilePath(const std::string& log_file_path);

  /**
   * Reset the journaling filesystem.
   * @param size The maximum amount of log storage, in bits.
   * @param age The maximum lifetime of the log, in units of s.
   * @param log_file_path Path to the log file.
   * @param log_wrapping Whether log wrapping is enabled.
   * @param wrapping_row Change lines every number of characters.
   */
  void Reset(std::size_t size, std::chrono::seconds age,
                         const std::string& log_file_path, bool log_wrapping,
                         int wrapping_row);
  
  /**
   * Disable the use of log files.
   */
  void Close();
  
  /**
   * Convert the log level to a string format, and convert the format to uppercase 
   * without the k.
   * @param level See LoggerSystem::Level for details on the level of log files.
   * @return Log level string.
   */
  std::string LevelToString(Level level);

 private:
  explicit LoggerSystem(std::size_t size = std::size_t(1024 * 1024),
                        std::chrono::seconds age =
                            std::chrono::seconds(std::chrono::seconds::max()),
                        const std::string& log_file_path = std::string("log/"));

  /**
   * Initialize the log management system.
   * @param size The maximum amount of log storage, in bits.
   * @param age The maximum lifetime of the log, in units of s.
   * @param log_file_path Path to the log file.
   * @param log_wrapping Whether log wrapping is enabled.
   * @param wrapping_row Change lines every number of characters.
   */
  void Initialized(std::size_t size, std::chrono::seconds age,
                   const std::string& log_file_path, bool log_wrapping,
                   int wrapping_row);

  ~LoggerSystem();

  /**
   * Determine if the current state has exceeded the set time and set stored 
   * value. If more than that, throw it to void RollOverLogs() and 
   * void DeleteAllLogs().
   */
  void RotateLogFile();

  /**
   * The log rotation system resets the name of the previous log to 
   * log_[start time]- [end time].log, and updates the current log file to 
   * log_[start time].log.
   */
  void RollOverLogs();

  /**
   * Load the information of the last log file. The full details are stored in 
   * a file named last_time_log.txt.
   */
  void LoadLogFileSetting();
  
  /**
   * Load the information of the last log file. The full details are stored in 
   * a file named last_time_log.txt.
   * @param change_dir If so, write the current directory information back to 
   * the original last_time_log.txt.
   * @param log_file_path The directory of the log.
   */
  void SaveLogFileSetting(bool change_dir, const std::string& log_file_path);

  /**
   * Remove all files in the log directory.
   */
  void DeleteAllLogs();

  /**
   * Deleting the log file will create a new log file to store the new 
   * information. (Because it is possible that the time exceeds the set 
   * time and the message is still written when it is divided, a new log must 
   * be set or the risk of a null pointer will be generated.)
   */
  void DeleteLogs();

  /**
   * Parse the time at which a message was first written to the log file.
   * @return The time point of the first log file.
   */
  std::chrono::time_point<std::chrono::system_clock> ExtractFirstTimestamp();

  /**
   * Point in time to convert the log file, because the stored string is 
   * "%Y-%m-%d_%H-%M-%S", it can be parsed into the required time format now.
   * @param time_stamp A point in time string in the log file.
   * @return Converted time format.
   */
  std::chrono::time_point<std::chrono::system_clock> ParseTimestamp(
      const std::string& time_stamp);

  DISABLE_COPY_MOVE(LoggerSystem)

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

  bool log_wrapping_;
  int wrapping_row_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_LOGGERSYSTEM_H_
