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

#include <thread>
#include "chrono"
#include "gtest/gtest.h"
#include "LoggerSystem.h"
#include "Time/Timer.h"

TEST(ChronoTest, CheckResolution) {
  using namespace std::chrono;

  auto resolution = high_resolution_clock::period::num /
                    static_cast<double>(high_resolution_clock::period::den);

  LoggerSystem::GetInstance().Log(
      LoggerSystem::Level::kInfo,
      "std::chrono::high_resolution_clock resolution: " +
          std::to_string(resolution) + " seconds.");

  ASSERT_LT(
      resolution,
      1e-6);  // Assume that a high-resolution clock should have a resolution less than 1 microsecond
}

TEST(ChronoTest, CheckStability) {
  using namespace std::chrono;

  auto start = high_resolution_clock::now();
  std::this_thread::sleep_for(seconds(1));
  auto end = high_resolution_clock::now();

  duration<double> elapsed = end - start;

  LoggerSystem::GetInstance().Log(
      LoggerSystem::Level::kInfo,
      "Elapsed time for 1 second sleep: " + std::to_string(elapsed.count()) +
          " seconds.");

  ASSERT_NEAR(
      elapsed.count(), 1.0,
      0.02);  // Assume that a 1-second sleep time should be within 0.02 seconds
}

TEST(TimerTest, TimerFunctionality) {
  Timer timer;

  timer.StartTimer();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  timer.StopTimer();

  double elapsed = timer.ElapsedSeconds();
  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kInfo,
                                  "Elapsed time for 100 milliseconds sleep: " +
                                      std::to_string(elapsed) + " seconds.");
  ASSERT_NEAR(
      elapsed, 0.1,
      0.02);  // Assume that a 100ms sleep time should be within 0.01 seconds
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
