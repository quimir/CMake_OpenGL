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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_TIMEUTILS_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_TIMEUTILS_H_

#include <chrono>
#include <mutex>
#include <string>

/**
 * Time class, this class provides a unified time, can guarantee the 
 * independence of time.
 * 
 * Usage example:
 * @code
 * auto now_time=TimeUtils::GetInstance().GetCurrentTime();
 * auto format_string=TimeUtils::GetInstance().FormatTime(now_time);
 * @endcode
 * 
 * @note This class is thread-safe. The use of an internal mutex ensures that 
 * an instance is generated only once.
 */
class TimeUtils {
 public:
  static TimeUtils& GetInstance();

  /**
   * Gets the current time, which is scheduled under std::chrono::system_clock::now(). 
   * Details please see: https://en.cppreference.com/w/cpp/chrono/system_clock/now. 
   * The function does nothing with it.
   * @return Returns the current time.
   */
  std::chrono::time_point<std::chrono::system_clock> GetCurrentTime();

  /**
   * Convert the time to a string and normalize it. The normalized time format is: 
   * "%Y-%m-%d:%H-%M-%S", that is, year - month - day - hour - minute - second.
   * @param time_point Normalized time is required.
   * @return Already normalized time string.
   */
  std::string FormatTime(
      const std::chrono::time_point<std::chrono::system_clock>& time_point);

  /**
   * Get the difference between two times.
   * @param start Start time.
   * @param end End time.
   * @return Returns the difference between two times, as a double, down to 
   * seconds.
   */
  double GetDurationInSeconds(
      const std::chrono::time_point<std::chrono::system_clock>& start,
      const std::chrono::time_point<std::chrono::system_clock>& end);

  /**
   * Get the current time node.
   * @return Returns the current time as a double, measured in seconds.
   */
  static double GetTimeStamp();

  ~TimeUtils();

  TimeUtils(TimeUtils& other) = delete;

  TimeUtils& operator=(TimeUtils& other) = delete;

 private:
  TimeUtils() = default;

 private:
  static std::once_flag initialized_;
  static TimeUtils* instance_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_TIMEUTILS_H_
