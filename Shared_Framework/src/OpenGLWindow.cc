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

#include "OpenGLWindow.h"
#include "LoggerSystem.h"
#include "OpenGLException.h"
#include <cstdlib>
#include "ImGui/OpenGLLogMessage.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
// Cancel the glad macro definition and give it to Window first.
#ifdef APIENTRY
#undef APIENTRY
#endif

#include <Windows.h>
#include <direct.h>

// Cancel the Windows macro definition and revert to the glad macro definition.
#ifdef APIENTRY
#undef APIENTRY
#define APIENTRY __stdcall
#endif
#else
#include <unistd.h>
#include <limits.h>
#endif

OpenGLWindow::OpenGLVersion OpenGLWindow::opengl_version_{};

#ifdef _WIN32
// Ensure that NV independent graphics cards are prioritized in the notebook.
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// Ensure that AMD independent graphics cards are prioritized in the notebook.
extern "C" {
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

OpenGLWindow::OpenGLWindow(int width, int height, const char* title,
                           GLFWmonitor* monitor, GLFWwindow* share)
    : Widget(0, 0, width, height),
      cursor_(nullptr),
      render_timer_(),
      mouse_state_(true),
      window_(nullptr),
      primary_monitor_(nullptr),
      VSYNC_value_(0) {
  try {
    InitGLFW();
    InitWindow(width, height, title, monitor, share);
    InitGLAD();
    frame_buffer_ = new FrameBuffer(width, height);
  } catch (OpenGLException& e) {
#ifdef _WIN32
    ErrorMessageBox(
        std::string(
            "A fatal error occurred while creating an OpenGL window.The "
            "cause of the error is: ") +
            e.what(),
        "Generate an OpenGL error", MB_ICONERROR | MB_DEFBUTTON1);
#else
    ErrorMessageBox(
        std::string(
            "A fatal error occurred while creating an OpenGL window.The "
            "cause of the error is: ") +
            e.what(),
        "Generate an OpenGL error");
#endif
#ifdef _DEBUG
    std::cerr << "A fatal error occurred while creating an OpenGL window.The "
                 "cause of the error is: "
              << e.what() << std::endl;
#endif
    exit(0);
  }
}

OpenGLWindow::~OpenGLWindow() {
  if (this->render_timer_.IsRunning()) {
    this->render_timer_.StopTimer();
    this->render_timer_.Cleanup();
  }
  delete this->frame_buffer_;
  Cleanup();
}

void OpenGLWindow::Run() {
  if (this->cursor_) {
    glfwSetCursor(window_, this->cursor_);
  }
  InitializeGL();
  MainLoop();
}

void OpenGLWindow::InitGLFW() {
  opengl_version_ = QueryOpenGLVersion();
  LoggerSystem::GetInstance().Log(
      LoggerSystem::Level::kInfo,
      "OpenGL Renderer: " + opengl_version_.renderer);
  LoggerSystem::GetInstance().Log(LoggerSystem::Level::kInfo,
                                  "OpenGL Vendor: " + opengl_version_.vendor);
}

void OpenGLWindow::InitWindow(int width, int height, const char* title,
                              GLFWmonitor* monitor, GLFWwindow* share) {
  if (!glfwInit()) {
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to initialize GLFW");
  }

  switch (opengl_version_.type) {
    case OpenGLType::kCore:
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_.major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_.minor);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
      break;
    case OpenGLType::kES:
      glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_.major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_.minor);
      glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
      break;
    case OpenGLType::kCompatibility:
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_.major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_.minor);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
      break;
    case OpenGLType::kUnknown:
      throw OpenGLException(LoggerSystem::Level::kError,
                            "Unsupported OpenGL version type");
  }

  window_ = glfwCreateWindow(width, height, title, monitor, share);

  if (!this->window_) {
    glfwTerminate();
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to create GLFW window");
  }

  primary_monitor_ = glfwGetPrimaryMonitor();

  int count;
  const GLFWvidmode* modes = glfwGetVideoModes(primary_monitor_, &count);

  if (modes == nullptr) {
    throw OpenGLException(
        LoggerSystem::Level::kWarning,
        "Failed to initialize OpenGL video Settings. Please try again.");
  }

  opengl_window_mode_.name = glfwGetMonitorName(primary_monitor_);
  glfwGetMonitorPhysicalSize(primary_monitor_,
                             &opengl_window_mode_.physical_size_x,
                             &opengl_window_mode_.physical_size_y);

  for (int i = 0; i < count; ++i) {
    const GLFWvidmode& mode = modes[i];
    opengl_window_mode_.opengl_vide_mode.push_back(mode);
  }

  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
  glfwSetCursorEnterCallback(window_, CursorEnterCallback);
  glfwSetWindowUserPointer(window_, this);
}

void OpenGLWindow::InitGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to initialize GLAD");
  }

  // Disable Vsync. NVIDIA GeForce will enable vertical synchronization by
  // default.
  SetVSYNC(VSYNC_value_);

  LoggerSystem::GetInstance().Log(
      LoggerSystem::Level::kInfo,
      std::string("Success Build OpenGL,now OpenGL Version is: ") +
          std::to_string(opengl_version_.major) + "." +
          std::to_string(opengl_version_.minor) + ".0");
}

void OpenGLWindow::MainLoop() {
  while (!glfwWindowShouldClose(window_)) {
    render_timer_.StartTimer();
    MakeContextCurrent();

    ProcessInput(this->window_);

    PaintGL();

    // Swap front and back buffers to display rendered content
    glfwSwapBuffers(window_);
    // Handle all waiting events
    glfwPollEvents();
    render_timer_.StopTimer();
    render_timer_.FrameEnd();
  }
}

void OpenGLWindow::Cleanup() {
  if (this->cursor_) {
    glfwDestroyCursor(this->cursor_);
    this->mouse_state_ = true;
  }
  glfwDestroyWindow(this->window_);
  glfwTerminate();
}

void OpenGLWindow::FrameBufferSizeCallback(GLFWwindow* window, int width,
                                           int height) {
  int x_pos, y_pos;
  glfwGetWindowPos(window, &x_pos, &y_pos);
  glViewport(0, 0, width, height);
  auto* self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
  if (self) {
    self->frame_buffer_->Resize(width, height);
    self->ResizeGL(width, height);
    self->ReSetWidget(x_pos, y_pos, width, height);
  }
}

void OpenGLWindow::ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void OpenGLWindow::MakeContextCurrent() {
  if (this->window_ != nullptr) {
    glfwMakeContextCurrent(this->window_);
  } else {
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Cannot make context current: window is null");
  }
}
void OpenGLWindow::DisplayMouse() {
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
  if (!this->mouse_state_)
    this->mouse_state_ = true;
}
void OpenGLWindow::HideMouse() {
  if (!this->mouse_state_) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kWarning,
                                    "The mouse state is now hidden, so there "
                                    "is no need to reset it to hide it.");
    return;
  }
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  this->mouse_state_ = false;
}
void OpenGLWindow::NormalMouse() {
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  if (!this->mouse_state_)
    this->mouse_state_ = true;
}
void OpenGLWindow::EnableRawMouseMotion() {
  HideMouse();
  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kInfo,
                                    "The user enables raw mouse motion");
  } else {
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kWarning,
        "Cannot be opened raw mouse motion , because this computer does not "
        "support the original mouse movement.");
    DisplayMouse();
  }
}
bool OpenGLWindow::SetCursor(const GLFWimage* image, int x_hot, int y_hot) {
  try {
    this->cursor_ = glfwCreateCursor(image, x_hot, y_hot);
    if (!this->cursor_) {
      throw OpenGLException(LoggerSystem::Level::kWarning,
                            "User initialized custom mouse error! The "
                            "existence of image will be rechecked!");
    }
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(e.what());
  }
  return true;
}
void OpenGLWindow::CursorEnterCallback(GLFWwindow* window, int entered) {
  if (entered) {
    // The cursor entered the content area of the window
  } else {
    // The cursor left the content area of the window
  }
}

const RenderTimer& OpenGLWindow::GetRenderTimer() const {
  return render_timer_;
}

void OpenGLWindow::ResizeGL(int width, int height) {}
void OpenGLWindow::InitializeGL() {}
void OpenGLWindow::PaintGL() {}

OpenGLWindow::OpenGLVersion OpenGLWindow::QueryOpenGLVersion() {
  if (!glfwInit()) {
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to create hidden GLFW window");
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwDestroyWindow(window);
    glfwTerminate();
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to initialize GLAD");
  }

  const char* version_str = (const char*)glGetString(GL_VERSION);
  const char* renderer_str = (const char*)glGetString(GL_RENDERER);
  const char* vendor_str = (const char*)glGetString(GL_VENDOR);

  if (!version_str || !renderer_str || !vendor_str) {
    glfwDestroyWindow(window);
    glfwTerminate();
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to retrieve OpenGL information");
  }

  std::string version_copy = std::string(version_str);
  std::string renderer_copy = std::string(renderer_str);
  std::string vendor_copy = std::string(vendor_str);

  int major = 0, minor = 0;
  OpenGLType type = OpenGLType::kUnknown;

  /**
   * Extract information from OpenGL.
   */
  if (version_copy.find("OpenGL ES") != std::string::npos) {
    type = OpenGLType::kES;
    std::size_t start = version_copy.find("OpenGL ES") + 9;
    char* endPtr;
    major = strtol(version_copy.c_str() + start, &endPtr, 10);
    if (*endPtr == '.') {
      minor = strtol(endPtr + 1, &endPtr, 10);
    }
  } else {
    char* endPtr;
    major = strtol(version_str, &endPtr, 10);
    if (*endPtr == '.') {
      minor = strtol(endPtr + 1, &endPtr, 10);
      if (version_copy.find("Compatibility Profile") != std::string::npos) {
        type = OpenGLType::kCompatibility;
      } else {
        type = OpenGLType::kCore;
      }
    }
  }

  if (major == 0 && minor == 0) {
    glfwDestroyWindow(window);
    glfwTerminate();
    throw OpenGLException(LoggerSystem::Level::kError,
                          "Failed to parse OpenGL version");
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return {major, minor, type, renderer_copy, vendor_copy};
}
const OpenGLWindow::OpenGLVersion& OpenGLWindow::GetOpenglVersion() {
  return opengl_version_;
}
void OpenGLWindow::ResetOpenGLWindow(int major, int minor,
                                     OpenGLType opengl_type, int width,
                                     int height, const char* title,
                                     GLFWmonitor* monitor, GLFWwindow* share) {
  Cleanup();
  opengl_version_ =
      OpenGLVersion{major, minor, opengl_type, opengl_version_.renderer,
                    opengl_version_.vendor};
  InitWindow(width, height, title, monitor, share);
  InitGLAD();
  frame_buffer_ = new FrameBuffer(width, height);
  int x_pos, y_pos;
  glfwGetWindowPos(window_, &x_pos, &y_pos);
  ReSetWidget(x_pos, y_pos, width, height);
}
const OpenGLWindow::OpenGLWindowMode& OpenGLWindow::GetOpenglWindowMode()
    const {
  return opengl_window_mode_;
}
std::string OpenGLWindow::OpenGLVersionToString(
    OpenGLWindow::OpenGLType opengl_type) {
  switch (opengl_type) {
    case OpenGLType::kCore:
      return "Core";
    case OpenGLType::kES:
      return "ES";
    case OpenGLType::kCompatibility:
      return "Compatibility";
    default:
      return "Unknown";
  }
}
void OpenGLWindow::SetVSYNC(int value) {
  try {
    if (value < -1)
      throw OpenGLException(
          LoggerSystem::Level::kWarning,
          "Error! Setting unreasonable vertical buffer value is not allowed!");
    VSYNC_value_ = value;
    glfwSwapInterval(VSYNC_value_);
  } catch (OpenGLException& e) {
    OpenGLLogMessage::GetInstance().AddLog(e.what());
  }
}

int OpenGLWindow::ErrorMessageBox(const std::string& message,
                                  const std::string& title,
                                  unsigned int uType) {
  int result;
#ifdef _WIN32
  result = MessageBox(nullptr, reinterpret_cast<LPCSTR>(message.c_str()),
                      reinterpret_cast<LPCSTR>(title.c_str()), uType);
#elif __linux__
  std::string command =
      "zenity --info --title=\"" + title + "\" --text=\"" + message + "\"";
  result = system(command.c_str());
#elif __APPLE__
  std::string command =
      "osascript -e 'tell app \"System Events\" to display dialog \"" +
      message + "\" with title \"" + title + "\" buttons {\"OK\"}'";
  result = system(command.c_str());
#endif

  return result;
}