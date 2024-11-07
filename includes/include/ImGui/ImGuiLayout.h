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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUILAYOUT_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUILAYOUT_H_

#include "ImGuiLayoutItem.h"
#include <vector>
#include <memory>
#include "include/ImGui/ImGuiWidget.h"
#include "include/Core/MacroDefinition.h"

class ImGuiLayout : public ImGuiLayoutItem {
 public:
  ImGuiLayout();

  void AddWidget(ImGuiWidget* w);

  Size MinimumSize() const override;

  Size MaximumSize() const override;

  virtual void SetSpacing(int new_spacing);

  virtual int GetSpacing() const;

  bool IsEmpty() const override;

  void SetGeometry(const Rect& r) override;

  Rect GetGeometry() const override;

  void SetPosition(const Point& point) override;

  Point GetPosition() const override;

  Size GetSize() const override;

  virtual void AddItem(ImGuiLayoutItem* item) = 0;

  virtual int Count() const = 0;

  virtual int IndexOf(const ImGuiWidget* widget) const;

  virtual int IndexOf(const ImGuiLayoutItem* layout_item) const;

  virtual ImGuiLayout* ItemAt(int index) const = 0;

  void RemoveItem(ImGuiLayoutItem* item);

  void RemoveWidget(ImGuiWidget* widget);

  ~ImGuiLayout() override;

  void Update();

  bool IsEnabled() const;

  void Enable();

  void Disable();

 private:
  DISABLE_COPY(ImGuiLayout)

 protected:
  std::vector<ImGuiLayoutItem*> layout_items_;

  int spacing_;

  bool enabled_;

 private:
  std::vector<ImGuiWidget*> widget_items_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_IMGUI_IMGUILAYOUT_H_
