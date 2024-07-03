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
#include "include/LoggerSystem.h"
#include <fstream>
#include <filesystem>

// Test log file creation and writing
TEST(LoggerSystemTest, LogCreationAndWriting) {
  // Set the logfile directory
  std::string log_dir = "test_logs/";
  LoggerSystem &logger = LoggerSystem::GetInstance();
  logger.SetMaxSize(1024 * 1024); // 1MB
  logger.SetMaxAge(std::chrono::seconds(3600)); // 1 hour

  // Log file write test
  logger.Log(LoggerSystem::Level::kInfo, "This is a test info message");
  logger.Log(LoggerSystem::Level::kWarning, "This is a test warning message");
  logger.Log(LoggerSystem::Level::kError, "This is a test error message");

  // Verify that the log file exists
  bool log_exists = false;
  for (const auto &entry : std::filesystem::directory_iterator(log_dir)) {
	if (entry.path().extension() == ".log") {
	  log_exists = true;
	  break;
	}
  }
  ASSERT_TRUE(log_exists);

  // Delete the test log file
  for (const auto &entry : std::filesystem::directory_iterator(log_dir)) {
	if (entry.path().extension() == ".log") {
	  std::filesystem::remove(entry.path());
	}
  }
}

// Test log file rotation
TEST(LoggerSystemTest, LogRotation) {
  // Set the logfile directory
  std::string log_dir = "test_logs/";
  LoggerSystem &logger = LoggerSystem::GetInstance();
  logger.SetMaxSize(50); // Set small size for fast rotation
  logger.SetMaxAge(std::chrono::seconds(3600)); // 1 hour

  // Write to the log multiple times to trigger file rotation
  for (int i = 0; i < 10; ++i) {
	logger.Log(LoggerSystem::Level::kInfo, "This is a test info message");
  }

  // Verify that multiple logfiles are generated
  int log_count = 0;
  for (const auto &entry : std::filesystem::directory_iterator(log_dir)) {
	if (entry.path().extension() == ".log") {
	  ++log_count;
	}
  }
  ASSERT_GT(log_count, 1);

  // Delete the test log file
  for (const auto &entry : std::filesystem::directory_iterator(log_dir)) {
	if (entry.path().extension() == ".log") {
	  std::filesystem::remove(entry.path());
	}
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
