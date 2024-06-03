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

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void FrameBufferSizeCallback(GLFWwindow* window,int width,int height)
{
  glViewport(0,0,width,height);
}

void ProcessInput(GLFWwindow* window)
{
  if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
  {
	glfwSetWindowShouldClose(window,true);
  }
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window= glfwCreateWindow(800,600,"LearnOpenGL", nullptr, nullptr);

  if (window== nullptr)
  {
	cout<<"Failed to create GLFW window"<<endl;
	glfwTerminate();
	return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
  {
	cout<<"Failed to initialize GLAD"<<endl;
	return -1;
  }

  glfwSetFramebufferSizeCallback(window,FrameBufferSizeCallback);
  
  while(!glfwWindowShouldClose(window))
  {

	ProcessInput(window);

	glClearColor(0.2f,0.3f,0.3f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwPollEvents();
	glfwSwapBuffers(window);
  }
  
  glfwTerminate();
  return 0;
}