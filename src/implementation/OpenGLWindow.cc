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

OpenGLWindow::OpenGLWindow(int width, int height, const char *title) {
  InitGLFW();
  InitWindow(width, height, title);
  InitGLAD();
}

OpenGLWindow::~OpenGLWindow() {
  Cleanup();
}

void OpenGLWindow::Run() {
  InitializeGL();
  MainLoop();
}

void OpenGLWindow::InitGLFW() {
  if (!glfwInit()) {
	std::cerr << "Failed to initialize GLFW" << std::endl;
	LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
									"Failed to initialize GLFW");
	exit(EXIT_FAILURE);
  }
}

void OpenGLWindow::InitWindow(int width, int height, const char *title) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (!this->window_) {
	LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
									"Failed to create GLFW window");
	glfwTerminate();
	exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
  glfwSetWindowUserPointer(window_, this);
}

void OpenGLWindow::InitGLAD() {
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
	LoggerSystem::GetInstance().Log(LoggerSystem::Level::kError,
									"Failed to initialize GLAD");
	exit(EXIT_FAILURE);
  }
}

void OpenGLWindow::MainLoop() {
  while (!glfwWindowShouldClose(window_)) {
	ProcessInput(this->window_);

	PaintGL();

	glfwSwapBuffers(window_);
	glfwPollEvents();
  }
}

void OpenGLWindow::Cleanup() {
  glfwDestroyWindow(this->window_);
  glfwTerminate();
}

void OpenGLWindow::FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  auto *self = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
  if (self) {
	self->ResizeGL(width, height);
  }
}

void OpenGLWindow::ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	glfwSetWindowShouldClose(window, true);
  }
}
