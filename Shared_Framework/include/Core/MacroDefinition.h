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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_CORE_MACRODEFINITION_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_CORE_MACRODEFINITION_H_

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/

#define DISABLE_COPY(Class)     \
  Class(const Class&) = delete; \
  Class& operator=(const Class&) = delete;

#define DISABLE_COPY_MOVE(Class) \
  DISABLE_COPY(Class)            \
  Class(Class&&) = delete;       \
  Class& operator=(Class&&) = delete;

/**
 * This macro definition is used to define the export symbol of the 
 * dynamic/static link library. It can create export symbols based on 
 * different platforms.
 * 
 * Used in functions: SHARED_FRAMEWORK_API ${function type} ${function name} 
 * ${function parameters}. For example: SHARED_FRAMEWORK_API void 
 * exported_function();
 * 
 * Used in classes: class SHARED_FRAMEWORK_API ${class name}. For example: 
 * class SHARED_FRAMEWORK_API ExportedClass
 */
#ifdef STATIC_BUILD
#define SHARED_FRAMEWORK_API
#else
#if defined(_WIN32) || defined(_WIN64)
#ifdef SHARED_FRAMEWORK_EXPORTS
#define SHARED_FRAMEWORK_API __declspec(dllexport)
#else
#define SHARED_FRAMEWORK_API __declspec(dllimport)
#endif
#else
#ifdef SHARED_FRAMEWORK_EXPORTS
#define SHARED_FRAMEWORK_API __attribute__((visibility("default")))
#else
#define SHARED_FRAMEWORK_API
#endif
#endif
#endif

/**
 * This macro definition needs to be used with SHARED_FRAMEWORK_API, which 
 * provides a C language declared dynamic/static link export method for the 
 * function.
 */
#ifdef __cplusplus
#define SHARED_FRAMEWORK_C_FUNCTION extern "C" SHARED_FRAMEWORK_API
#else
#define SHARED_FRAMEWORK_C_FUNCTION SHARED_FRAMEWORK_API
#endif

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CORE_MACRODEFINITION_H_
