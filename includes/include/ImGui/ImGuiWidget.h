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

#include "ImGuiconfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../Core/Widget.h"

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
 * imgui_widget.BeginFrame();
 * imgui_widget.Render();
 * imgui_widget.EndFrame();
 */
class ImGuiWidget : public Widget {
 public:
  /**
   * To initialize the ImGui widget because it is attached to OpenGL3 and GLFW 
   * it is necessary to enter the window pointer of GLFW. It will check if 
   * OpenGL is empty and if it is, it will not allow the build.
   * @param window OpenGL window pointer.
   * @param window_width The width of the window.
   * @param window_height The length of the window.
   */
  explicit ImGuiWidget(GLFWwindow* window, int window_width, int window_height);

  ImGuiWidget(const ImGuiWidget& other) = delete;

  ImGuiWidget& operator=(const ImGuiWidget& other) = delete;

  virtual ~ImGuiWidget();

  /**
   * Start a new ImGui frame, must be called at the beginning of each render 
   * frame.
   * 
   * This function initializes ImGui integration with OpenGL and GLFW to prepare 
   * ImGui for drawing new frames.
   */
  virtual void BeginFrame();

  /**
   * To end the current ImGui frame, must be called at the end of each render 
   * frame. 
   * 
   * This function is responsible for rendering all the drawing data collected 
   * by ImGui and rendering them to the screen using OpenGL.
   */
  virtual void EndFrame();

  /**
   * Render ImGui frames.
   * 
   * This function needs to be called between BeginFrame() and EndFrame() to 
   * implement custom ImGui rendering logic.Subclasses can override this
   * function to add specific rendering code, such as drawing ImGui controls,
   * handling user input, etc.
   * 
   * Example call order:
   * BeginFrame();
   * // Custom rendering logic
   * EndFrame();
   */
  virtual void Render();

  ImGuiIO* GetIo() const;

  GLFWwindow* GetOpenGLWindow();

 private:
  /**
   * Build the ImGui window.
   * @param window GLFW window pointer.
   */
  void Initialized(GLFWwindow* window);

  virtual void Cleanup();

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
  GLFWwindow* window_;

  ImGuiIO* io_;

  static bool is_create_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUIWIDGET_H_
