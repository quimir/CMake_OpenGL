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

#include "include/ImGui/OpenGLLogMessage.h"
OpenGLLogMessage& OpenGLLogMessage::GetInstance() {
  static OpenGLLogMessage instance;
  return instance;
}
void OpenGLLogMessage::AddLog(const std::string& message) {
  std::lock_guard<std::mutex> lock(mutex_);
  logs_.push_back(message);
  has_error_ = true;
}
void OpenGLLogMessage::Clear() {
  std::lock_guard<std::mutex> lock_guard(mutex_);
  logs_.clear();
  has_error_ = false;
}
const std::vector<std::string>& OpenGLLogMessage::GetLogs() const {
  return logs_;
}
bool OpenGLLogMessage::IsHasError() const {
  return has_error_;
}
