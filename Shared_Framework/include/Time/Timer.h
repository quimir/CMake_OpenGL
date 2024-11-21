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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_TIMER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_TIMER_H_

#include "TimeUtils.h"
#include "Core/MacroDefinition.h"

/**
 * Timer tool, which can achieve a simple high precision timing function.
 */
class Timer {
 public:
  Timer();

  /**
   * Starts the timer and generates a cerr warning if the previous timer 
   * started and did not complete.
   */
  void StartTimer();

  /**
   * Stop the timer, a cerr warning is issued if the previous timer has stopped 
   * and no more timers are active.
   */
  void StopTimer();

  /**
   * Returns the difference between the end and start time. If the start timer 
   * is not started, a cerr error is emitted and -1 is returned. If a tick is 
   * not completed when the function is called, the current time - the time the 
   * timer started - is returned.
   * @return -1 | present time - start timer time | end timer time - start timer 
   * time.
   */
  double ElapsedSeconds() const;

  /**
   * Returns the difference between the end and start time in milliseconds.
   * @return the difference between the end and start time in milliseconds.
   */
  double ElapsedMilliseconds() const;
  
  bool IsRunning() const;

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_time_;
  bool running_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_TIMER_H_
