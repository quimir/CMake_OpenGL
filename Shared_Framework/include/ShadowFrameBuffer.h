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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_SHADOWFRAMEBUFFER_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_SHADOWFRAMEBUFFER_H_

#include "FrameBuffer.h"

class ShadowFrameBuffer : public FrameBuffer {
 public:
  enum class ShadowType { kBasic, kPoint, kCSM };

 public:
  ShadowFrameBuffer(ShadowType shadow_type, GLint width, GLint height,
                    int cascade_count = 1);

  int GetCascadeCount() const;

  void SetCascadeCount(int cascade_count);

 private:
  void Initialize();

 private:
  ShadowType shadow_type_;

  int cascade_count_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_SHADOWFRAMEBUFFER_H_
