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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXARRAY_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXARRAY_H_

#include "glad/glad.h"

/**
 * Vertex array buffer data, this class will OpenGL vertex array simple encapsulation. 
 * glGenVertexArrays() is automatically scheduled to bind to this class when it is 
 * created. OpenGL is also checked, and if OpenGL is not created, the class is not 
 * allowed.And the class automatically dispatches glDeleteVertexArrays to unbind the 
 * OpenGL vertex array if it goes out of scope. It's important to note that if you 
 * want to use this data again you need to use: ReGenVertexArrays() This re-registers 
 * the vertex array in OpenGL.
 * 
 * Usage example:
 * @code
 * shader.vertex:
 * #verison 330 core
 * layout (location = 0) in vec3 aPos;
 * 
 * void main()
 * {
 * gl_Position = vec4(aPos, 1.0);
 * }
 * 
 * main.cpp:
 * VertexArray vao;
 * vao.Bind();
 * vao.AddBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
 * vao.UnBind();
 * @endcode
 */
class VertexArray {
 public:
  VertexArray();

  ~VertexArray();

  /**
   * Re-register the class with OpenGL.
   */
  void ReGenVertexArrays();

  /**
   * bind a vertex array object.when this function is called, a unique 
   * representation of the class is bound to OpenGL.
   */
  void Bind();

  /**
   * Unbind the vertex array object.It binds 0 to OpenGL, and in OpenGL 4.6 
   * binding to 0 means unbinding the vertex array object.
   */
  void UnBind();

  /**
   * Define an array of generic vertex attribute data. please note that this 
   * function is called when will call together 
   * glEnableVertexAttribArray (index).
   * @param index Specifies the index of the generic vertex attribute to be 
   * modified.
   * @param size Specifies the number of components per generic vertex 
   * attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant 
   * GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
   * @param type Specifies the data type of each component in the array. 
   * The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, 
   * GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by 
   * glVertexAttribPointer and glVertexAttribIPointer. Additionally 
   * GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, 
   * GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are 
   * accepted by glVertexAttribPointer. GL_DOUBLE is also accepted by 
   * glVertexAttribLPointer and is the only token accepted by the type 
   * parameter for that function. The initial value is GL_FLOAT.
   * @param normalized For glVertexAttribPointer, specifies whether 
   * fixed-point data values should be normalized (GL_TRUE) or converted 
   * directly as fixed-point values (GL_FALSE) when they are accessed.
   * @param stride Specifies the byte offset between consecutive generic vertex 
   * attributes. If stride is 0, the generic vertex attributes are understood 
   * to be tightly packed in the array. The initial value is 0.
   * @param pointer Specifies a offset of the first component of the first 
   * generic vertex attribute in the array in the data store of the buffer 
   * currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
   */
  void AddBuffer(GLuint index, GLint size, GLenum type, GLboolean normalized,
                 GLsizei stride, const void* pointer) const;

  /**
   * Define an array of Int vertex attribute data.
   * @param index Specifies the index of the generic vertex attribute to be 
   * modified.
   * @param size Specifies the number of components per generic vertex 
   * attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant 
   * GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
   * @param type Specifies the data type of each component in the array. 
   * The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, 
   * GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by 
   * glVertexAttribPointer and glVertexAttribIPointer. Additionally 
   * GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, 
   * GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are 
   * accepted by glVertexAttribPointer. GL_DOUBLE is also accepted by 
   * glVertexAttribLPointer and is the only token accepted by the type 
   * parameter for that function. The initial value is GL_FLOAT.
   * @param stride Specifies the byte offset between consecutive generic vertex 
   * attributes. If stride is 0, the generic vertex attributes are understood 
   * to be tightly packed in the array. The initial value is 0.
   * @param pointer Specifies a offset of the first component of the first 
   * generic vertex attribute in the array in the data store of the buffer 
   * currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
   */
  void AddIntBuffer(GLuint index, GLint size, GLenum type, GLsizei stride,
                    const void* pointer) const;

  /**
   * Define an array of Long vertex attribute data.
   * @param index Specifies the index of the generic vertex attribute to be 
   * modified.
   * @param size Specifies the number of components per generic vertex 
   * attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant 
   * GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
   * @param type Specifies the data type of each component in the array. 
   * The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, 
   * GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by 
   * glVertexAttribPointer and glVertexAttribIPointer. Additionally 
   * GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, 
   * GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are 
   * accepted by glVertexAttribPointer. GL_DOUBLE is also accepted by 
   * glVertexAttribLPointer and is the only token accepted by the type 
   * parameter for that function. The initial value is GL_FLOAT.
   * @param stride Specifies the byte offset between consecutive generic 
   * vertex attributes. If stride is 0, the generic vertex attributes are 
   * understood to be tightly packed in the array. The initial value is 0.
   * @param pointer Specifies a offset of the first component of the first 
   * generic vertex attribute in the array in the data store of the buffer 
   * currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
   */
  void AddLongBuffer(GLuint index, GLint size, GLenum type, GLsizei stride,
                     const void* pointer) const;

  GLuint GetVaoId() const;

 private:
  /**
   * Bind this ID to the OpenGL vertex array. This ID is also unique to the 
   * class in OpenGL.
   */
  GLuint vao_id_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_VERTEXARRAY_H_
