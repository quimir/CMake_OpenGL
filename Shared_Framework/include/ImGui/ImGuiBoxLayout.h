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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIBOXLAYOUT_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIBOXLAYOUT_H_

#include "ImGuiLayout.h"

class ImGuiBoxLayout : public ImGuiLayout {
 public:
  enum class Direction {
    kLeftToRight,
    kRightToLeft,
    kTopToBottom,
    kBottomToTop
  };

 public:
  Size MinimumSize() const override;

  Size MaximumSize() const override;

  void SetSpacing(int new_spacing) override;

  void SetGeometry(const Rect& r) override;

  void AddItem(ImGuiLayoutItem* item) override;

  int Count() const override;

  ImGuiLayout* ItemAt(int index) const override;

 private:
  Point CalculateStartPosition(const Rect& r) const;

 private:
  Direction direction_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUIBOXLAYOUT_H_
