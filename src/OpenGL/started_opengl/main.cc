﻿/*******************************************************************************
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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "OpenGLMainWindow.h"
#include "FilePathSystem.h"
#include "LoggerSystem.h"
#include "OpenGLMessage.h"
#include "Shader.h"

using namespace std;

int main() {
  OpenGLMainWindow* main_window =
      new OpenGLMainWindow(800, 600, "Camera circle");
  main_window->Run();
  
  return 0;
}