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

#include <gtest/gtest.h>
#include <thread>
#include "include/LoggerSystem.h"
#include "include/Time/TimeUtils.h"

TEST(TimeUtilsTest, GetCurrentTime) {
  auto now = TimeUtils::GetInstance().GetCurrentTime();
  auto system_now = std::chrono::system_clock::now();
  auto diff = TimeUtils::GetInstance().GetDurationInSeconds(now, system_now);
  LoggerSystem::GetInstance().Log(
      LoggerSystem::Level::kInfo,
      "Test TimeUtils::GetInstance().GetDurationInSeconds: " +
          std::to_string(diff));
  ASSERT_LT(diff, 1.0);  // Time difference should be less than 1 second
}

TEST(TimeUtilsTest, FormatTime) {
  auto now = TimeUtils::GetInstance().GetCurrentTime();
  std::string formatted_time = TimeUtils::GetInstance().FormatTime(now);
  ASSERT_FALSE(formatted_time.empty());
  ASSERT_EQ(formatted_time.size(),
            19);  // The length of formatting time should be 19
}

TEST(TimeUtilsTest, GetDurationInSeconds) {
  auto start = TimeUtils::GetInstance().GetCurrentTime();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  auto end = TimeUtils::GetInstance().GetCurrentTime();
  double duration = TimeUtils::GetInstance().GetDurationInSeconds(start, end);
  ASSERT_GE(duration, 1.0);
  ASSERT_LT(duration, 1.1);  // Considering the imprecision of sleep
}

TEST(TimeUtilsTest, GetTimeStamp) {
  double timestamp = TimeUtils::GetInstance().GetTimeStamp();
  ASSERT_GT(timestamp, 0.0);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
