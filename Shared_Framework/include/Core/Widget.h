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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_CORE_WIDGET_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_CORE_WIDGET_H_

#include "Rect.h"
#include "MacroDefinition.h"

class SHARED_FRAMEWORK_API Widget {
 public:
  explicit Widget(const Rect& rect);

  explicit Widget(int x, int y, int width, int height);

  void ReSetWidget(int x, int y, int width, int height);

  void ReSetWidget(const Rect& rect_angle);

  const Rect& GetRect() const;

  Rect& RGetRect();

  void SetRect(const Rect& rect);

  int GetWidth() const;

  int GetHeight() const;

  void SetWidth(int width);

  void SetHeight(int height);
  
  void ReSizeWidget(int width,int height);

 private:
  Rect rect_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CORE_WIDGET_H_
