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

#include "Time/TimeUtils.h"
#include <iomanip>
#include <sstream>

std::once_flag TimeUtils::initialized_;
TimeUtils* TimeUtils::instance_ = nullptr;

std::chrono::time_point<std::chrono::system_clock> TimeUtils::GetCurrentTime() {
  return std::chrono::system_clock::now();
}
std::string TimeUtils::FormatTime(
    const std::chrono::time_point<std::chrono::system_clock>& time_point) {
  auto now = std::chrono::system_clock::to_time_t(time_point);
  std::tm local_time{};
  localtime_s(&local_time, &now);

  std::ostringstream oss;
  oss << std::put_time(&local_time, "%Y-%m-%d_%H-%M-%S");
  return oss.str();
}
double TimeUtils::DeltaTimeToDouble(
    const std::chrono::time_point<std::chrono::system_clock>& start,
    const std::chrono::time_point<std::chrono::system_clock>& end) {
  return std::chrono::duration<double>(end - start).count();
}
TimeUtils& TimeUtils::GetInstance() {
  if (nullptr == instance_) {
    std::call_once(initialized_, []() { instance_ = new TimeUtils; });
  }
  return *instance_;
}
double TimeUtils::GetTimeStamp() {
  auto now = TimeUtils::GetInstance().GetCurrentTime();
  const double epoch_time =
      std::chrono::duration<double>(now.time_since_epoch()).count();

  static double last_time;

  static bool initialized = false;
  if (!initialized) {
    last_time = epoch_time;
    initialized = true;
  }

  double time_stamp = epoch_time - last_time;

  return time_stamp;
}
TimeUtils::~TimeUtils() {
  delete instance_;
}
std::chrono::seconds TimeUtils::TimePointToSeconds(
    const std::chrono::time_point<std::chrono::system_clock>& time_point) {
  return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
}
