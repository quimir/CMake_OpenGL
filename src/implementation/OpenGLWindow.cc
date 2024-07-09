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

#include "include/OpenGLWindow.h"
#include "include/LoggerSystem.h"

OpenGLWindow::OpenGLWindow(int width, int height, const char* title,
                           GLFWmonitor* monitor, GLFWwindow* share)
    : Widget(width,height),frame_buffer_(nullptr) {
  InitGLFW();
  InitWindow(width, height, title, monitor, share);
  InitGLAD();
  frame_buffer_ = new FrameBuffer(width, height);
}

OpenGLWindow::~OpenGLWindow() {
  delete this->frame_buffer_;
  Cleanup();
}

void OpenGLWindow::Run() {
  InitializeGL();
  MainLoop();
}

void OpenGLWindow::InitGLFW() {
  if (!glfwInit()) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
                                    "Failed to initialize GLFW");
    throw std::runtime_error("Failed to initialize GLFW");
  }
}

void OpenGLWindow::InitWindow(int width, int height, const char* title,
                              GLFWmonitor* monitor, GLFWwindow* share) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width, height, title, monitor, share);

  if (!this->window_) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
                                    "Failed to create GLFW window");
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
  glfwSetWindowUserPointer(window_, this);
}

void OpenGLWindow::InitGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
                                    "Failed to initialize GLAD");
    throw std::runtime_error("Failed to initialize GLAD");
  }

  LoggerSystem::GetInstance().Log(
      LoggerSystem::Level::kInfo,
      std::string("Success Build OpenGL,now OpenGL Version is: ") +
          (const char*)glGetString(GL_VERSION));
}

void OpenGLWindow::MainLoop() {
  while (!glfwWindowShouldClose(window_)) {
    MakeContextCurrent();

    ProcessInput(this->window_);
    
    this->frame_buffer_->Bind();
    RenderToFramebuffer();
    this->frame_buffer_->UnBind();
    
    PaintGL();

    // Swap front and back buffers to display rendered content
    glfwSwapBuffers(window_);
    // Handle all waiting events
    glfwPollEvents();
  }
}

void OpenGLWindow::Cleanup() {
  glfwDestroyWindow(this->window_);
  glfwTerminate();
}

void OpenGLWindow::FrameBufferSizeCallback(GLFWwindow* window, int width,
                                           int height) {
  glViewport(0, 0, width, height);
  auto* self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
  if (self) {
    self->frame_buffer_->Resize(width,height);
    self->ResizeGL(width, height);
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
    LoggerSystem::GetInstance().Log(
        LoggerSystem::Level::kError,
        "Cannot make context current: window is null");
    throw std::runtime_error("Cannot make context current: window is null");
  }
}
void OpenGLWindow::RenderToFramebuffer() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
