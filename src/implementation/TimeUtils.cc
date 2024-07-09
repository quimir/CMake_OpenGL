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

#include "include/TimeUtils.h"
#include <iomanip>
#include <sstream>
std::chrono::time_point<std::chrono::system_clock> TimeUtils::GetCurrentTime() {
  return std::chrono::system_clock::now();
}
std::string TimeUtils::FormatTime(const std::chrono::time_point<std::chrono::system_clock> &time_point) {
  auto now = std::chrono::system_clock::to_time_t(time_point);
  std::tm local_time{};
  localtime_s(&local_time, &now);

  std::ostringstream oss;
  oss << std::put_time(&local_time, "%Y-%m-%d:%H-%M-%S");
  return oss.str();
}
double TimeUtils::GetDurationInSeconds(const std::chrono::time_point<std::chrono::system_clock> &start,
									   const std::chrono::time_point<std::chrono::system_clock> &end) {
  return std::chrono::duration<double>(end-start).count();
}
TimeUtils & TimeUtils::GetInstance() {
  static TimeUtils instance;
  return instance;
}
double TimeUtils::GetTimeStamp() {
  auto now = GetCurrentTime();
  return std::chrono::duration<double>(now.time_since_epoch()).count();
}
