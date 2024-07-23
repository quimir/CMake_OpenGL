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

#include "include/Time/Timer.h"
#include "include/LoggerSystem.h"

void Timer::StartTimer() {
  if (this->running_) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Warning! The previous timer is still running, stop the previous timer "
        "and then start the timer.");
    std::cerr << "Warning! The previous timer is still running, stop the "
                 "previous timer and then start the timer."
              << std::endl;
    return;
  }
  this->start_time_ = std::chrono::high_resolution_clock::now();
  running_ = true;
}
void Timer::StopTimer() {
  if (!this->running_) {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Warning! The current timer has all stopped, please do not repeat "
        "stop, please start the timer and then stop the operation.");
    std::cerr
        << "Warning! The current timer has all stopped, please do not repeat "
           "stop, please start the timer and then stop the operation."
        << std::endl;
    return;
  }
  this->end_time_ = std::chrono::high_resolution_clock::now();
  running_ = false;
}
double Timer::ElapsedSeconds() const {
  if (!running_ &&
      start_time_ ==
          std::chrono::time_point<std::chrono::high_resolution_clock>()) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
                                    "Error! The timer has not been started, "
                                    "cannot calculate elapsed time.");
    std::cerr << "Error! The timer has not been started, cannot calculate "
                 "elapsed time."
              << std::endl;
    return -1;
  }
  if (running_) {
    auto now_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now_time - start_time_).count();
  } else {
    if (end_time_ !=
        std::chrono::time_point<std::chrono::high_resolution_clock>()) {
      return std::chrono::duration<double>(end_time_ - start_time_).count();
    } else {
      LoggerSystem::GetInstance().Log(
          LoggerSystem::Level::kError,
          "Error! The timer has not been stopped properly, cannot calculate "
          "elapsed time.");
      std::cerr << "Error! The timer has not been stopped properly, cannot "
                   "calculate elapsed time."
                << std::endl;
      return -1;
    }
  }
}
Timer::Timer()
    : running_(false), start_time_(std::chrono::high_resolution_clock::now()) {
  static bool detection_system = false;
  if (!detection_system) {
    auto resolution =
        std::chrono::high_resolution_clock::period::num /
        static_cast<double>(std::chrono::high_resolution_clock::period::den);
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kInfo,
        "std::chrono::high_resolution_clock resolution: " +
            std::to_string(resolution) + " seconds");
  }
}
double Timer::ElapsedMilliseconds() const {
  return ElapsedSeconds() * 1000.0;
}
bool Timer::IsRunning() const {
  return running_;
}
