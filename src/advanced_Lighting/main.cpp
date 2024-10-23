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

#include "OpenGLMainWindow.h"
#include "ShadowMappingDepthWindow.h"

int main() {
  //  auto* opengl_main_window =
  //      new OpenGLMainWindow(640, 480, "Advanced Lighting", nullptr, nullptr);
  //  opengl_main_window->Run();
  auto* shadow_mapping_depth_window =
      new ShadowMappingDepthWindow(800, 600, "Shadow Mapping Depth", nullptr,
                                   nullptr);
  shadow_mapping_depth_window->Run();
  return 0;
}
