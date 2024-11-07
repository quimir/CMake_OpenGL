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
#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CORE_MACRODEFINITION_H_
