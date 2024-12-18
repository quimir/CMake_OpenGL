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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLWINDOW_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLWINDOW_H_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "FrameBuffer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Time/RenderTimer.h"
#include "Core/Widget.h"

#include "Core/MacroDefinition.h"

/**
 * OpenGL window, which encapsulates some of the most basic methods of 
 * generating OpenGL. It is recommended that all Windows that use OpenGL 
 * inherit from this class. If you use GLFW and GLAD,this automatically build 
 * the OpenGL for you and store the address of the resulting OpenGL window in 
 * window_. To use this class, you must implement void InitializeGL(), void 
 * ResizeGL(int width,int height), and void PaintGL() by calling void first 
 * InitializeGL(), Then call void ResizeGL(int width,int height), and finally 
 * loop through void PaintGL(). See the comments for more details about these 
 * three functions. 
 * 
 * Please note that you must add the following to the subclass 
 * constructor: glfwSetWindowUserPointer(this->window_,this); 
 * Otherwise, the OpenGL window handle is still called from the superclass.
 * 
 * Use reference:
 * @code
 * class OpenGLMainWindow:public OpenGLWindow
 * {};
 * 
 * OpenGLMainWindow *window;
 * window->Run();
 * @endcode
 */
class OpenGLWindow : public Widget {
 public:
  enum class OpenGLType { kCore, kES, kCompatibility, kUnknown };

  struct OpenGLVersion {
    int major;             // Major version number
    int minor;             // Divided version number
    OpenGLType type;       // OpenGL types
    std::string renderer;  // Display card information.
    std::string vendor;    // Display card manufacturers.
  };

  struct OpenGLWindowMode {
    const char* name;
    int physical_size_x;
    int physical_size_y;
    std::vector<GLFWvidmode> opengl_vide_mode;
  };

 public:
  /**
   * Initialize OpenGL, build OpenGL successfully if normal and output the 
   * build details to the log file. Otherwise, an error message is printed.
   * @param width The desired width, in screen coordinates, of the window. 
   * This must be greater than zero.
   * @param height The desired height, in screen coordinates, of the window. 
   * This must be greater than zero.
   * @param title The initial, UTF-8 encoded window title.
   * @param monitor The monitor to use for full screen mode, or NULL for 
   * windowed mode.
   * @param share The window whose context to share resources with, or NULL to
   * not share resources.
   */
  OpenGLWindow(int width, int height, const char* title,
               GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

  /**
   * Perform OpenGL rendering and painting. When you start this function, it 
   * calls InitializeGL() to initialize your data, and then draws OpenGL. 
   * It's a for loop statement that goes on until ProcessInput is specifically 
   * set unless you set the request to close.
   */
  void Run();

  /**
   * Prepares for rendering OpenGL content for this widget by making the 
   * corresponding context current and binding the framebuffer object in 
   * that context.
   * 
   * It is not necessary to call this function in most cases, because it is 
   * called automatically before invoking paintGL().
   */
  void MakeContextCurrent();

  /**
   * Override this destructor if you are adding your own members, otherwise 
   * your members may not be destroyed correctly, and do not remove 
   * OpenGL::~OpenGLWindow() unless you really know the implementation of 
   * the function.
   */
  virtual ~OpenGLWindow();

  /**
   * Displays the mouse, and the mouse cannot move beyond the window, only 
   * within the window. This is equivalent to using: glfwSetInputMode(
   * window_, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
   */
  void DisplayMouse();

  /**
   * Hide the mouse, the mouse is not visible, the movement of the mouse is not 
   * allowed beyond the display of the screen. This is equivalent to using:
   * glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   */
  void HideMouse();

  /**
   * Display the mouse with no restrictions on the mouse state at all, which 
   * means that the mouse can now exceed the display screen. Using this mode 
   * suggests using the other capture mouse API and pausing if the mouse 
   * exceeds the display screen.This is equivalent to using: glfwSetInputMode(
   * window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   */
  void NormalMouse();

  /**
   * When the cursor is disabled, raw (unscaled and unaccelerated) mouse motion 
   * can be enabled if available.
   * 
   * Raw mouse motion is closer to the actual motion of the mouse across a 
   * surface. It is not affected by the scaling and acceleration applied to the 
   * motion of the desktop cursor. That processing is suitable for a cursor 
   * while raw motion is better for controlling for example a 3D camera. 
   * Because of this, raw mouse motion is only provided when the cursor is 
   * disabled.
   * 
   * If the query finds that the original mouse input is not supported, then 
   * the mouse setting is reverted to DisplayMouse() and the error details are 
   * entered into the log file.
   */
  void EnableRawMouseMotion();

  /**
   * Create a new custom cursor image and store it in cursor_ which can be set 
   * for the window using glfwSetCursor. The cursor is destroyed by 
   * glfwDestroyCursor. Any remaining cursors are destroyed by glfwTerminate. 
   * Pixels are 32-bit, small in-bit, non-duplicated RGBA, that is, 8 bits per 
   * channel with the red channel in front. Cursor hotspots are in pixels, 
   * relative to the top left corner of the cursor image. As with all other 
   * coordinate systems in GLFW, the X-axis points to the right and the Y-axis 
   * points down.
   * @param image The desired cursor image.
   * @param x_hot The desired x-coordinate, in pixels, of the cursor hotspot.
   * @param y_hot The desired y-coordinate, in pixels, of the cursor hotspot.
   * @return Returns true on success, false otherwise.
   */
  bool SetCursor(const GLFWimage* image, int x_hot, int y_hot);

  const RenderTimer& GetRenderTimer() const;

  static const OpenGLVersion& GetOpenglVersion();

  void ResetOpenGLWindow(int major, int minor, OpenGLType opengl_type,
                         int width, int height, const char* title,
                         GLFWmonitor* monitor, GLFWwindow* share);

  const OpenGLWindowMode& GetOpenglWindowMode() const;

  /**
   * Sets the interval of OpenGL context exchange, that is, the number of times 
   * you wait for a screen update after calling glfwSwapBuffers, before 
   * swapping the buffers and returning. Also known as vertical synchronization 
   * function.
   * @param value Vsync value. 0 indicates that Vsync is disabled, and -1 
   * indicates adaptive Vsync (This value requires the support of the graphics 
   * card vendor). Negative values other than -1 are undefined and are optimized 
   * to 0 or 1 by default if the graphics card manufacturer does not support 
   * them.
   */
  void SetVSYNC(int value);

  static std::string OpenGLVersionToString(OpenGLType opengl_type);

 protected:
  /**
   * This virtual function is called for the first time after the first call 
   * to the Run function and before PaintGL and ResizeGL. This function 
   * should set up any required OpenGL resources and state.
   * 
   * There is no need to call MakeCurrent() because this has already been done 
   * when this function is called. Note however that the framebuffer is not 
   * yet available at this stage, so avoid issuing draw calls from here. 
   * Defer such calls to PaintGL() instead.
   */
  virtual void InitializeGL();

  /**
   * This virtual function is called whenever the widget has been resized. 
   * Reimplement it in a subclass. The new size is passed in w and h.
   * @param width Change the width of the screen
   * @param height Change the screen length
   */
  virtual void ResizeGL(int width, int height);

  /**
   * This virtual function is called whenever the widget needs to be painted. 
   * Reimplement it in a subclass.
   * 
   * There is no need to call MakeCurrent() because this has already been done 
   * when this function is called.
   * 
   * Before invoking this function, the context and the framebuffer are bound, 
   * and the viewport is set up by a call to glViewport(). No other state is 
   * set and no clearing or drawing is performed by the framework.
   */
  virtual void PaintGL();

  /**
   * The user's key is processed, and the default is that the user presses 
   * esc to exit the process. You can override this virtual function to create 
   * your own user keystroke handling.It's going to be called before PaintGL().
   * @param window OpenGL window Pointers that do not need to be filled in are 
   * already written automatically when called.
   */
  virtual void ProcessInput(GLFWwindow* window);

 private:
  /**
   * Initialize GLFW and throw an exception if an error occurs.
   */
  void InitGLFW();

  /**
   * Creates a window and its associated context.
   * @param width The desired width, in screen coordinates, of the window.
   * This must be greater than zero.
   * @param height The desired height, in screen coordinates, of the window.
   * This must be greater than zero.
   * @param title The initial, UTF-8 encoded window title.
   * @param monitor The monitor to use for full screen mode, or `NULL` for
   * windowed mode.
   * @param share The window whose context to share resources with, or `NULL`
   * to not share resources.
   */
  void InitWindow(int width, int height, const char* title,
                  GLFWmonitor* monitor, GLFWwindow* share);

  /**
   * Initialize GLAD and throw an exception if an error occurs.
   */
  void InitGLAD();

  void MainLoop();

  void Cleanup();

  static void FrameBufferSizeCallback(GLFWwindow* window, int width,
                                      int height);

  static void CursorEnterCallback(GLFWwindow* window, int entered);

  /**
   * This function queries the native OpenGL information and returns the native 
   * OpenGL information. The idea is to create a hidden OpenGL window and 
   * initialize GLFW and GLAD. Bind the context of OpenGL to it. Its OpenGL 
   * information is extracted by the information extractor.
   * @return Native OpenGL information.
   */
  OpenGLVersion QueryOpenGLVersion();
  
  static int ErrorMessageBox(const std::string& message, const std::string& title,
                       unsigned int uType = 0);

  DISABLE_COPY(OpenGLWindow)

 protected:
  FrameBuffer* frame_buffer_;
  GLFWwindow* window_;
  GLFWcursor* cursor_;
  bool mouse_state_;
  GLFWmonitor* primary_monitor_;

  int VSYNC_value_;

 private:
  // Render timer, which keeps track of the time until the render ends.
  RenderTimer render_timer_;

  static OpenGLVersion opengl_version_;

  OpenGLWindowMode opengl_window_mode_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLWINDOW_H_
