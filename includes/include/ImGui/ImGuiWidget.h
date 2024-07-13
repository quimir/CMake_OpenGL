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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUIWIDGET_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUIWIDGET_H_

#include <string>

#include "GLFW/glfw3.h"
#include "ImGuiconfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "include/Widget.h"

/**
 * Create a simple ImGui window, implemented with OpenGL3 and GLFW. You can use 
 * the OpenGL you create to automatically create the version of OpenGL you need 
 * for ImGui. You can build your ImGui window with Render() and then Run(). 
 * Because it is a window, copy construction and copy construction are not 
 * supported. There is no risk of memory leaks, and the destructor reclaims all 
 * imGUIs and all resources of the class when the class dies.
 * 
 * Use reference:
 * ImGuiWidget imgui_widget(this->window_);
 * imgui_widget.Run();
 * 
 * OR:
 * ImGuiWidget imgui_widget(this->window_);
 * imgui_widget.BeginFrame();
 * imgui_widget.Render();
 * imgui_widget.EndFrame();
 */
class ImGuiWidget : public Widget {
 public:
  explicit ImGuiWidget(GLFWwindow* window, int window_width, int window_height);

  ImGuiWidget(const ImGuiWidget& other) = delete;

  ImGuiWidget& operator=(const ImGuiWidget& other) = delete;

  virtual ~ImGuiWidget();
  
  virtual void BeginFrame();

  virtual void EndFrame();

  virtual void Render();

  void Run();

  ImGuiIO* GetIo() const;

 private:
  /**
   * Build the ImGui window.
   * @param window GLFW window pointer.
   */
  void Initialize(GLFWwindow* window);

  void Cleanup();

  /**
   * Get the version of OpenGL. The version of OpenGL is queried as GL_VERSION, 
   * because it will have major,minor, and release as its version information. 
   * Therefore, you just need to extract "major"+"minor"+"release" to get the 
   * version of OpenGL3 you need for ImGui.
   * @param opengl_version_string OpenGL version information. This is captured 
   * by glGetString(GL_VERSION).
   * @return User OpenGL version number information required by ImGui.
   */
  std::string ExtractMajorVersion(const std::string& opengl_version_string);

 private:
  ImGuiIO* io_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUIWIDGET_H_
