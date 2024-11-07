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

#ifndef CMAKE_OPEN_SRC_ADVANCED_LIGHTING_SHADOWMAPPINGDEPTHWINDOW_H_
#define CMAKE_OPEN_SRC_ADVANCED_LIGHTING_SHADOWMAPPINGDEPTHWINDOW_H_

#include "include/Experimental/OpenGLCameraWindow.h"
#include "include/OpenGLException.h"
#include "include/TextureLoader.h"
#include "include/VertexArray.h"
#include "include/Buffers.h"
#include "include/Shader.h"
#include "ImGuiMainWindow.h"
#include "PointShadow.h"
#include "ShadowMapping.h"
#include "include/ImGui/ImGuiWindow.h"

class ShadowMappingDepthWindow : public OpenGLCameraWindow {
 protected:
 public:
  ShadowMappingDepthWindow(int width, int height, const char* title,
                           GLFWmonitor* monitor, GLFWwindow* share);

  ~ShadowMappingDepthWindow() override;

 protected:
  void InitializeGL() override;
  void ResizeGL(int width, int height) override;
  void PaintGL() override;
  
  PointShadow* point_shadow_;
  ShadowMapping* shadow_mapping_;
  ImGuiWindow imgui_window_;
};

#endif  //CMAKE_OPEN_SRC_ADVANCED_LIGHTING_SHADOWMAPPINGDEPTHWINDOW_H_
