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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_SHADER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_SHADER_H_

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_set>

#include "glad/glad.h"
#include "glm/glm.hpp"

/**
 * Create a simple shader class where all shader behavior can be performed. 
 * OpenGL types and GLM types are supported in this class. There is no risk 
 * of memory leaks; all memory is reclaimed in the destructor.This class does 
 * not support copy construction or copy operations.
 * 
 * Use reference:
 * Shader shader("text.vert","text.frag");
 * shader.Bind();
 * shader.SetInt("text",1);
 * Shader.UnBind();
 */
class Shader {
 private:
  enum class ShaderErrorType {
    kProgram = 0,
    kVertex,
    kFragment,
    kGeometry,
    kCompute,
    kTessellationControl,
    kTessellationEvaluation
  };

 public:
  GLuint GetID() const;

  /**
   * Start the shader. Note that any function that wants to use a shader must
   * use it first, otherwise no other function will work.
   */
  void Bind();

  /**
   * Remove shaders from use by using Bind() first.
   */
  void UnBind();

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param value For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  void SetBool(const std::string& name, bool value);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param value For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  void SetInt(const std::string& name, int value);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param value For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  void SetFloat(const std::string& name, float value);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param x new x value.
   * @param y new y value.
   */
  void SetVec2(const std::string& name, float x, float y);

  /**
   * Specify the value of a uniform variable for the current program object
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of elements that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array, 
   * and 1 or more if it is an array.
   * @param value  specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetVec2(const std::string& name, GLsizei count, const GLfloat* value);

  /**
   * Specify the value of a uniform variable for the current program object
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of elements that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array, 
   * and 1 or more if it is an array.
   * @param value  specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetVec2(const std::string& name, GLsizei count, const glm::vec2& value);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param value specifies a pointer to an array of count values that will be 
   * used to update the specified uniform variable.
   */
  void SetVec2(const std::string& name, const glm::vec2& value);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param x new x value.
   * @param y new y value.
   * @param z new z value.
   */
  void SetVec3(const std::string& name, float x, float y, float z);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param value specifies a pointer to an array of count values that will be 
   * used to update the specified uniform variable.
   */
  void SetVec3(const std::string& name, const glm::vec3& value);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param x new x value.
   * @param y new y value.
   * @param z new z value.
   * @param w new w value.
   */
  void SetVec4(const std::string& name, float x, float y, float z, float w);

  /**
   * Specify the value of a uniform variable for the current program object.If 
   * it fails or can't find uniform then it emits a message in the log file.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param value specifies a pointer to an array of count values that will be 
   * used to update the specified uniform variable.
   */
  void SetVec4(const std::string& name, const glm::vec4& value);

  /**
   * Specify the value of a uniform variable for the current program object.The 
   * default population is glUniformMatrix2fv(glGetUniformLocation(
   * this->id,name.c_str),1,GL_FALSE,&mat2[0][0]). Note that using this
   * function is just a convenience; for more detailed manipulation use:
   * void SetMat2(const std::string& name, GLsizei count, GLboolean transpose,
   * const glm::mat2& mat2);or void SetMat2(const std::string& name, 
   * GLsizei count, GLboolean transpose,const GLfloat* value);
   * @param name Specifies the name of the uniform variable to be modified.
   * @param mat2 specifies a pointer to an array of count values that will be 
   * used to update the specified uniform variable.
   */
  void SetMat2(const std::string& name, const glm::mat2& mat2);

  /**
   * Specifies the location of the uniform variable to be modified.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of matrices that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array of 
   * matrices, and 1 or more if it is an array of matrices.
   * @param transpose For the matrix commands, specifies whether to transpose 
   * the matrix as the values are loaded into the uniform variable.
   * @param mat2 specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat2(const std::string& name, GLsizei count, GLboolean transpose,
               const glm::mat2& mat2);

  /**
   * Specifies the location of the uniform variable to be modified.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of matrices that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array of 
   * matrices, and 1 or more if it is an array of matrices.
   * @param transpose For the matrix commands, specifies whether to transpose 
   * the matrix as the values are loaded into the uniform variable.
   * @param value specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat2(const std::string& name, GLsizei count, GLboolean transpose,
               const GLfloat* value);

  /**
   * Specify the value of a uniform variable for the current program object.The 
   * default population is glUniformMatrix3fv(glGetUniformLocation(
   * this->id,name.c_str),1,GL_FALSE,&mat3[0][0]). Note that using this
   * function is just a convenience; for more detailed manipulation use:
   * void SetMat3(const std::string& name, GLsizei count, GLboolean transpose,
   * const GLfloat* value);or void SetMat3(const std::string& name, 
   * GLsizei count, GLboolean transpose,const glm::mat3& mat3);
   * @param name Specifies the name of the uniform variable to be modified.
   * @param mat3 specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat3(const std::string& name, const glm::mat3& mat3);

  /**
   * Specifies the location of the uniform variable to be modified.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of matrices that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array of 
   * matrices, and 1 or more if it is an array of matrices.
   * @param transpose For the matrix commands, specifies whether to transpose 
   * the matrix as the values are loaded into the uniform variable.
   * @param value specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat3(const std::string& name, GLsizei count, GLboolean transpose,
               const GLfloat* value);

  /**
   * Specifies the location of the uniform variable to be modified.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of matrices that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array of 
   * matrices, and 1 or more if it is an array of matrices.
   * @param transpose For the matrix commands, specifies whether to transpose 
   * the matrix as the values are loaded into the uniform variable.
   * @param mat3 specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat3(const std::string& name, GLsizei count, GLboolean transpose,
               const glm::mat3& mat3);

  /**
   * Specify the value of a uniform variable for the current program object.The 
   * default population is glUniformMatrix4fv(glGetUniformLocation(
   * this->id,name.c_str),1,GL_FALSE,&mat4[0][0]). Note that using this
   * function is just a convenience; for more detailed manipulation use:
   * void SetMat4(const std::string& name, GLsizei count, GLboolean transpose,
   * const GLfloat* value);or void SetMat4(const std::string& name, 
   * GLsizei count, GLboolean transpose,const glm::mat4& mat4);
   * @param name Specifies the name of the uniform variable to be modified.
   * @param mat4 specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat4(const std::string& name, const glm::mat4& mat4);

  /**
   * Specifies the location of the uniform variable to be modified.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of matrices that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array of 
   * matrices, and 1 or more if it is an array of matrices.
   * @param transpose For the matrix commands, specifies whether to transpose 
   * the matrix as the values are loaded into the uniform variable.
   * @param value specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat4(const std::string& name, GLsizei count, GLboolean transpose,
               const GLfloat* value);

  /**
   * Specifies the location of the uniform variable to be modified.
   * @param name Specifies the name of the uniform variable to be modified.
   * @param count specifies the number of matrices that are to be modified. 
   * This should be 1 if the targeted uniform variable is not an array of 
   * matrices, and 1 or more if it is an array of matrices.
   * @param transpose For the matrix commands, specifies whether to transpose 
   * the matrix as the values are loaded into the uniform variable.
   * @param mat4 specifies a pointer to an array of count values that will 
   * be used to update the specified uniform variable.
   */
  void SetMat4(const std::string& name, GLsizei count, GLboolean transpose,
               const glm::mat4& mat4);

  /**
   * launches one or more compute work groups. Each work group is processed by 
   * the active program object for the compute shader stage. While the 
   * individual shader invocations within a work group are executed as a unit, 
   * work groups are executed completely independently and in unspecified order. 
   * num_groups_x, num_groups_y and num_groups_z specify the number of local 
   * work groups that will be dispatched in the X, Y and Z dimensions, 
   * respectively.
   * 
   * Please see: 
   * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDispatchCompute.xhtml
   * @param num_groups_x The number of work groups to be launched in the X 
   * dimension.
   * @param num_groups_y The number of work groups to be launched in the Y 
   * dimension.
   * @param num_groups_z The number of work groups to be launched in the Z 
   * dimension.
   */
  void SetDispatchCompute(GLuint num_groups_x, GLuint num_groups_y,
                          GLuint num_groups_z);

  /**
   * defines a barrier ordering the memory transactions issued prior to the 
   * command relative to those issued after the barrier. For the purposes of 
   * this ordering, memory transactions performed by shaders are considered to 
   * be issued by the rendering command that triggered the execution of the 
   * shader. barriers is a bitfield indicating the set of operations that 
   * are synchronized with shader stores; 
   * @param barriers must be a bitwise combination of any of 
   * GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT, GL_ELEMENT_ARRAY_BARRIER_BIT, 
   * GL_UNIFORM_BARRIER_BIT, GL_TEXTURE_FETCH_BARRIER_BIT, 
   * GL_SHADER_IMAGE_ACCESS_BARRIER_BIT, GL_COMMAND_BARRIER_BIT, 
   * GL_PIXEL_BUFFER_BARRIER_BIT, GL_TEXTURE_UPDATE_BARRIER_BIT, 
   * GL_BUFFER_UPDATE_BARRIER_BIT, GL_FRAMEBUFFER_BARRIER_BIT, 
   * GL_TRANSFORM_FEEDBACK_BARRIER_BIT, GL_ATOMIC_COUNTER_BARRIER_BIT, or 
   * GL_SHADER_STORAGE_BARRIER_BIT.
   * 
   * Function details please check: 
   * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMemoryBarrier.xhtml
   */
  void SetMemoryBarrier(GLbitfield barriers);

  /**
   * Build an OpenGL shader. It must have a path for a vertex shader and a path 
   * for a fragment shader. After linking, the registered shader ID is stored in 
   * id_. If an error occurs, the error message is displayed on the console and 
   * in the log file
   * @param vertex_path Path to vertex shader path. Required because each shader 
   * must have a gl_Position.
   * @param fragment_path Fragment shader path, required.
   * @param geometry_path Geometry shader path, where fillable items are 
   * integrated into the shader if filled in.
   * @param tess_control_path Tess control shader path, where fillable items are 
   * integrated into the shader if filled in.
   * @param tess_evaluation_path Tess evaluation shader path, where fillable 
   * items are integrated into the shader if filled in.
   * @param compute_path Compute shader path, where fillable items are 
   * integrated into the shader if filled in.
   */
  explicit Shader(const char* vertex_path, const char* fragment_path,
                  const char* geometry_path = nullptr,
                  const char* tess_control_path = nullptr,
                  const char* tess_evaluation_path = nullptr,
                  const char* compute_path = nullptr);

  /**
   * Build an OpenGL shader. It must have a path for a vertex shader and a path 
   * for a fragment shader. After linking, the registered shader ID is stored in 
   * id_. If an error occurs, the error message is displayed on the console and 
   * in the log file
   * @param vertex_path Path to vertex shader path. Required because each shader 
   * must have a gl_Position.
   * @param fragment_path Fragment shader path, required.
   * @param geometry_path Geometry shader path, where fillable items are 
   * integrated into the shader if filled in.
   * @param tess_control_path Tess control shader path, where fillable items are 
   * integrated into the shader if filled in.
   * @param tess_evaluation_path Tess evaluation shader path, where fillable 
   * items are integrated into the shader if filled in.
   * @param compute_path Compute shader path, where fillable items are 
   * integrated into the shader if filled in.
   */
  Shader(const std::string& vertex_path, const std::string& fragment_path,
         const std::string& geometry_path = std::string(),
         const std::string& tess_control_path = std::string(),
         const std::string& tess_evaluation_path = std::string(),
         const std::string& compute_path = std::string());

  ~Shader();

  Shader(const Shader& other) = delete;

  Shader& operator=(const Shader& other) = delete;

 private:
  /**
   * Determines if there are any errors in the shader build and prints them to 
   * a log file.
   * @param shader Shader ID.
   * @param error_type See Shader::ShaderErrorType for details.
   */
  void CheckCompileErrors(GLuint shader, ShaderErrorType error_type);
  
  /**
   * Convert the ShaderErrorType variable to a string for output, using an 
   * all-capital name without the k.
   * @param type Type of ShaderErrorType.
   * @return Converted string.
   */
  std::string ShaderErrorTypeToString(ShaderErrorType type);

  /**
   * Find the position of the uniform in the shader. If an error is found it is 
   * logged and reported to the log file, otherwise the location of the uniform 
   * in the shader is returned.
   * @param uniform_name Uniform name
   * @return Returns the position of uniform in the shader on success,
   * or -1 otherwise.
   */
  GLint CheckUniformExists(const std::string& uniform_name);

  /**
   * Find the position of the uniform block in the shader. If an error is found 
   * it is logged and reported to the log file, otherwise the location of the 
   * uniform block in the shader is returned.
   * @param block_name Uniform block name
   * @return Returns the position of uniform block in the shader on success,
   * or -1 otherwise.
   */
  GLuint CheckUniformBlockExists(const std::string& block_name);

  /**
   * Determine the OpenGL version.When using subdivide shaders and compute 
   * shaders, you need to specify that the OpenGL version allows the use of 
   * these shaders if the OpenGL version is passed, or throw an exception if 
   * it is not.
   * @param major_number The large version takes x of x.y as the input target.
   * @param minor_number The minor version takes y of x.y as the input target.
   */
  void CheckOpenGLVersion(int major_number, int minor_number) const;

 private:
  // Record the ID of the shader registered with OpenGL.
  GLuint id_;
  // Record the wrong name for the Uniform.
  std::unordered_set<std::string> uniform_warnings_;
  // Record the wrong name for the Uniform block.
  std::unordered_set<std::string> uniform_block_warnings_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_SHADER_H_
