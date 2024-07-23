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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_RENDERTIMER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_RENDERTIMER_H_

#include <deque>
#include "Timer.h"


class RenderTimer : public Timer {
 public:
  explicit RenderTimer(
      const std::deque<
          std::chrono::time_point<std::chrono::high_resolution_clock>>&
          frame_times = std::deque<
              std::chrono::time_point<std::chrono::high_resolution_clock>>());
  
  double GetFPS() const;

  double GetRenderDelay() const;

  void FrameEnd();

  double GetProgramRunTime() const;

 private:
  static std::chrono::time_point<std::chrono::system_clock> program_starts_;
  std::deque<std::chrono::time_point<std::chrono::high_resolution_clock>>
      frame_times_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_RENDERTIMER_H_
