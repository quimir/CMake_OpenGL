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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLMESSAGE_INL_
#define CMAKE_OPEN_INCLUDES_INCLUDE_OPENGLMESSAGE_INL_

#include "OpenGLMessage.h"

template<typename Func, typename... Args>
auto OpenGLMessage::CheckedCall(const char *file,
								int line,
								Func func,
								Args... args) -> decltype(func(args...)) {
  if constexpr (std::is_void_v<decltype(func(args...))>) {
	func(args...);
	OpenGLCheckError_(file, line);
  } else {
	auto result = func(args...);
	OpenGLCheckError_(file, line);
	return result;
  }
}

template<typename Func, typename... Args>
void OpenGLMessage::GetOpenGLCheckError(Func func, Args...args) {
  CheckedCall(__FILE__, __LINE__, func, std::forward<Args>(args)...);
}
template <typename Func, typename... Args>
void OpenGLMessage::GetOpenGLCheckError(const char* file, int line, Func func,
                                        Args... args) {
  CheckedCall(file,line,func,std::forward<Args>(args)...);
}


#endif