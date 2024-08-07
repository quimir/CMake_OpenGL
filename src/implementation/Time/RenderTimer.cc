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

#include "include/Time/RenderTimer.h"

constexpr int kMaxFrameSamples =
    100;  // Number of frames to average for FPS calculation
static bool Initialized = false;
std::chrono::time_point<std::chrono::system_clock> RenderTimer::program_starts_;
RenderTimer::RenderTimer(const std::deque<std::chrono::time_point<
                             std::chrono::high_resolution_clock>>& frame_times)
    : frame_times_(frame_times) {
  if (!Initialized) {
    program_starts_ = TimeUtils::GetInstance().GetCurrentTime();
    Initialized = true;
  }
}
void RenderTimer::FrameEnd() {
  auto now_time = std::chrono::high_resolution_clock::now();
  frame_times_.push_back(now_time);
  if (frame_times_.size() > kMaxFrameSamples) {
    frame_times_.pop_front();
  }
}
double RenderTimer::GetFPS() const {
  if (frame_times_.size() < 2) {
    return 0.0;
  }
  double duration =
      std::chrono::duration<double>(frame_times_.back() - frame_times_.front())
          .count();
  return (static_cast<double>(frame_times_.size()) - 1) / duration;
}
double RenderTimer::GetRenderDelay() const {
  if (frame_times_.size() < 2) {
    return 0.0;
  }
  auto last_frame_time = std::chrono::duration<double>(
                             frame_times_.back() - *(frame_times_.end() - 2))
                             .count();
  return last_frame_time;
}
double RenderTimer::GetProgramRunTime() const {
  auto now_time = TimeUtils::GetInstance().GetCurrentTime();
  return TimeUtils::GetInstance().DeltaTimeToDouble(program_starts_, now_time);
}
void RenderTimer::Cleanup() {
  frame_times_ =
      std::deque<std::chrono::time_point<std::chrono::high_resolution_clock>>();
}
