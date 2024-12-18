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

#include "SkeletalAnimation.h"
#include "LoggerSystem.h"

int main() {
  LoggerSystem::GetInstance().EnableLogWrapping(80);
  auto* skeletal_animation =
      new SkeletalAnimation(800, 600, "skeletal_animation", nullptr, nullptr);
  skeletal_animation->Run();
  return 0;
}