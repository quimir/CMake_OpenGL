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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXBUFFER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXBUFFER_H_

#include <glad/glad.h>
#include <vector>

/**
 * The Buffer class encapsulates the basic operations of OpenGL buffer objects.
 * It provides a convenient way to create, bind, set data, and delete OpenGL 
 * buffers.
 * 
 * Usage example:
 * @code
 * Buffer buffer;
 * buffer.Bind();
 * buffer.SetData(data,size,usage);s
 * buffer.UnBind();
 * @endcode
 */
class Buffers {
 public:
  /**
   * Constructor to create a new Buffer instance.
   * @param type The type of buffer, e.g:GL_ARRAY_BUFFER or 
   * GL_ATOMIC_COUNTER_BUFFER...
   * Please check the details.
   * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
   */
  explicit Buffers(GLenum type = GL_ARRAY_BUFFER);

  /**
   * Destructor for the Buffers class. Cleans up the allocated OpenGL buffer.
   */
  ~Buffers();

  /**
   * Retrieves the type of the buffer.
   * @return The GLenum representing the buffer type.
   */
  GLenum GetType() const;

  /**
   * Re sets the type of the buffer.
   * @param type The new GLenum type for the buffer.
   */
  void ReSetType(GLenum type);

  /**
   * Bind the buffer to the OpenGL context. This method must be called before 
   * the buffer data can be set or used.
   */
  void Bind();

  /**
   * Unbind the currently bound buffer. After calling this method, the buffer 
   * is no longer used by the OpenGL context.
   */
  void UnBind();

  /**
   * Sets the buffer's data.
   * @param data A pointer to data.
   * @param size The size of the data in bytes.
   * @param usage Specify the data usage mode, e.g., GL_STATIC_DRAW, 
   * GL_DYNAMIC_DRAW, etc.
   */
  void SetData(const void* data, GLsizeiptr size, GLenum usage) const;

  /**
   * Set the buffer data, and use the template method to handle std::vector.
   * @tparam T Data type.
   * @param data A std::vector containing the data
   * @param usage Specify the data usage mode, e.g., GL_STATIC_DRAW, 
   * GL_DYNAMIC_DRAW, etc.
   */
  template <typename T>
  void SetData(const std::vector<T>& data, GLenum usage) const;

  /**
   * Regenerates the buffer with the specified type or the current type if 
   * UINT_MAX is passed.
   * @param type The GLenum type for the new buffer, or UINT_MAX to use the 
   * current type.
   */
  void ReGenBuffers(GLenum type = UINT_MAX);

 private:
  GLuint buffer_id_;  // Buffer object id.
  GLenum type_;       // Buffer types.
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXBUFFER_H_
