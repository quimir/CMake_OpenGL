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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUILAYOUTITEM_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUILAYOUTITEM_H_

#include "imgui/imgui.h"
#include "../Core/Rect.h"
#include "Core/MacroDefinition.h"

class ImGuiLayoutItem {
 public:
  enum class ImGuiAlignment { kAlignLeft = 1, kAlignRight, kCenter };

 public:
  explicit ImGuiLayoutItem(ImGuiAlignment alignment = ImGuiAlignment::kAlignLeft);

  virtual ~ImGuiLayoutItem() = default;

  virtual void SetPosition(const Point& pos) = 0;

  virtual Point GetPosition() const = 0;

  virtual Size GetSize() const = 0;

  virtual Size MinimumSize() const = 0;

  virtual Size MaximumSize() const = 0;

  virtual bool IsEmpty() const = 0;

  ImGuiAlignment GetAlignment() const;

  void SetAlignment(ImGuiAlignment alignment);

  virtual void SetGeometry(const Rect& r) = 0;

  virtual Rect GetGeometry() const = 0;

 protected:
  ImGuiAlignment alignment_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUILAYOUTITEM_H_
