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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_SHAREDFRAMEWORKEXPORT_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_SHAREDFRAMEWORKEXPORT_H_

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

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_SHAREDFRAMEWORKEXPORT_H_
