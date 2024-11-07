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

#include "include/ImGui/ImGuiBoxLayout.h"
Size ImGuiBoxLayout::MinimumSize() const {
  return ImGuiLayout::MinimumSize();
}
Size ImGuiBoxLayout::MaximumSize() const {
  return ImGuiLayout::MaximumSize();
}
void ImGuiBoxLayout::SetSpacing(int new_spacing) {
  ImGuiLayout::SetSpacing(new_spacing);
}
void ImGuiBoxLayout::SetGeometry(const Rect& r) {
  if (!IsEnabled())
    return;

  Point current_pos = CalculateStartPosition(r);
  int max_cross_size = 0;
}
void ImGuiBoxLayout::AddItem(ImGuiLayoutItem* item) {
  layout_items_.emplace_back(item);
}
int ImGuiBoxLayout::Count() const {
  return static_cast<int>(layout_items_.size());
}
ImGuiLayout* ImGuiBoxLayout::ItemAt(int index) const {
  if (index >= 0 && index < layout_items_.size()) {
    return dynamic_cast<ImGuiLayout*>(layout_items_[index]);
  }

  return nullptr;
}
Point ImGuiBoxLayout::CalculateStartPosition(const Rect& r) const {
  if (direction_ == Direction::kLeftToRight)
    return r.TopLeft();
  if (direction_ == Direction::kRightToLeft)
    return r.TopRight();
  if (direction_ == Direction::kTopToBottom)
    return r.TopLeft();

  return r.BottomLeft();
}
