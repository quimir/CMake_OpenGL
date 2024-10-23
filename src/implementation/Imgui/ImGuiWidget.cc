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

#include "include/Imgui/ImGuiWidget.h"
#include <sstream>
#include "imgui/imgui_internal.h"
#include "include/Exception.h"
#include "include/LoggerSystem.h"
#include "include/OpenGLStateManager.h"
#include "include/ImGui/Fonts/Language.h"
#include "include/FilePathSystem.h"

ImGuiWidget::ImGuiWidget(GLFWwindow* window, int window_width,
                         int window_height)
    : Widget(0, 0, window_width, window_height), window_(window) {
  try {
    if (!OpenGLStateManager::GetInstance().IsEnableOpenGL()) {
      throw Exception(
          LoggerSystem::Level::kError,
          "Serious error! Imgui Windows are not allowed to be built without "
          "initializing OpenGL. Please initialize OpenGL before building ImGui "
          "Window");
    }
    if (nullptr == window_) {
      throw Exception(
          LoggerSystem::Level::kError,
          "Error! GLFW window pointer has not been initialized, please "
          "initialize it before setting ImGui.");
    }
  } catch (Exception& e) {
    std::cerr << e.what() << std::endl;
    exit(0);
  }
  Initialized(window);
}
void ImGuiWidget::Initialized(GLFWwindow* window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  this->io_ = &ImGui::GetIO();

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  auto major_version = ExtractMajorVersion(version);
  ImGui_ImplOpenGL3_Init(std::string("#version " + major_version).c_str());

//  if (Language::GetInstance().CurrentLanguage() == Language::Type::kChinese) {
//    io_->Fonts->AddFontFromFileTTF(
//        FilePathSystem::GetInstance()
//            .GetPath("src/fontlib/ZhanKuWenYiTi-2.ttf")
//            .c_str(),
//        13.0f, nullptr, io_->Fonts->GetGlyphRangesChineseSimplifiedCommon());
//  }
}
std::string ImGuiWidget::ExtractMajorVersion(
    const std::string& opengl_version_string) {
  std::istringstream iss(opengl_version_string);

  int major, minor, release;
  iss >> major;
  iss.ignore();
  iss >> minor;
  iss.ignore();
  iss >> release;

  std::ostringstream oss;
  oss << major << minor << release;

  return oss.str();
}
ImGuiWidget::~ImGuiWidget() {
  Cleanup();
}
void ImGuiWidget::Cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
void ImGuiWidget::BeginFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
void ImGuiWidget::EndFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImGuiWidget::Render() {
  ImGui::Begin("Hello,world!");
  ImGui::Text("This is some useful text.");
  ImGui::End();
}
ImGuiIO* ImGuiWidget::GetIo() const {
  return io_;
}
GLFWwindow* ImGuiWidget::GetOpenGLWindow() {
  return window_;
}
