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

#include "ImGui/ImGuiLayout.h"
#include "Exception.h"
#include <iostream>

void ImGuiLayout::AddWidget(ImGuiWidget* w) {
  widget_items_.emplace_back(w);
}
Size ImGuiLayout::MinimumSize() const {
  Size min_size;
  for (const auto& item : widget_items_) {
    auto item_size = item->GetRect();
    min_size.SetWidth(std::max(min_size.Width(), item_size.GetWidth()));
    min_size.SetHeight(min_size.Height() + item_size.GetHeight() + spacing_);
  }

  return min_size;
}
Size ImGuiLayout::MaximumSize() const {
  Size max_size;
  for (const auto& item : widget_items_) {
    auto item_size = item->GetRect();
    max_size.SetWidth(std::min(max_size.Width(), item_size.GetWidth()));
    max_size.SetHeight(item_size.GetHeight() + item_size.GetHeight() +
                       spacing_);
  }

  return max_size;
}
void ImGuiLayout::SetSpacing(int new_spacing) {
  spacing_ = new_spacing;
  Update();
}
int ImGuiLayout::GetSpacing() const {
  return spacing_;
}
bool ImGuiLayout::IsEmpty() const {
  return !(widget_items_.empty() || layout_items_.empty());
}
void ImGuiLayout::SetGeometry(const Rect& r) {
  auto layout_origin = r.GetPoint();
  auto layout_width = r.GetSize().Width();

  // Start position of the current row.
  Point current_pos = layout_origin;
  int max_row_height =
      0;  // The maximum height of the current row, used for line wrapping.

  for (auto& item : widget_items_) {
    Size item_size = item->GetRect().GetSize();

    // If the component exceeds the current line width, wrap to the next line.
    if (current_pos.x() + item_size.Width() >
        layout_origin.x() + layout_width) {
      current_pos.SetX(layout_origin.x());
      current_pos.SetY(current_pos.y() + max_row_height + spacing_);
      max_row_height = 0;
    }

    // Set the component locations.
    item->RGetRect().SetPoint(current_pos);

    // Update the current row position and row height.
    current_pos.SetX(current_pos.x() + item_size.Width() + spacing_);
    max_row_height = std::max(max_row_height, item_size.Height());
  }
}
Rect ImGuiLayout::GetGeometry() const {
  if (IsEmpty()) {
    return {0, 0, 0, 0};
  }

  Point min_position = widget_items_[0]->GetRect().GetPoint();
  Point max_position = min_position;

  for (const auto& item : widget_items_) {
    Rect geometry = item->GetRect();
    Point item_pos = geometry.GetPoint();
    Size item_size = geometry.GetSize();

    min_position = Point(std::min(min_position.x(), item_pos.x()),
                         std::min(min_position.y(), min_position.y()));

    max_position =
        Point(std::max(max_position.x(), item_pos.x() + item_size.Width()),
              std::max(max_position.y(), item_pos.y() + item_size.Height()));
  }

  Size overall_size = Size((max_position - min_position).x(),
                           (max_position - min_position).y());

  return {min_position, overall_size};
}
void ImGuiLayout::SetPosition(const Point& point) {
  Rect current_geometry = GetGeometry();

  Rect new_geometry(point, current_geometry.GetSize());
  SetGeometry(new_geometry);
}
Point ImGuiLayout::GetPosition() const {
  return GetGeometry().GetPoint();
}
Size ImGuiLayout::GetSize() const {
  Size now_size;
  for (const auto& item : widget_items_) {
    now_size.SetWidth(std::max(now_size.Width(), item->GetWidth()));
    now_size.SetHeight(std::max(now_size.Height(), item->GetHeight()));
  }

  return now_size;
}
int ImGuiLayout::IndexOf(const ImGuiWidget* widget) const {
  if ((widget != nullptr) && (!widget_items_.empty())) {
    for (size_t i = 0; i < widget_items_.size(); ++i) {
      if (widget_items_[i] == widget) {
        return static_cast<int>(i);
      }
    }
  }

  return -1;
}
int ImGuiLayout::IndexOf(const ImGuiLayoutItem* layout_item) const {
  if ((layout_item != nullptr) && (!layout_items_.empty())) {
    for (size_t i = 0; i < layout_items_.size(); ++i) {
      if (layout_items_[i] == layout_item) {
        return static_cast<int>(i);
      }
    }
  }

  return -1;
}
void ImGuiLayout::RemoveItem(ImGuiLayoutItem* item) {
  try {
    if (layout_items_.empty()) {
      throw Exception(
          LoggerSystem::Level::kWarning,
          "No ImGui Layout iterator is stored in it, forbidden to remove!");
    }

    if (item != nullptr) {
      for (size_t i = 0; i < layout_items_.size(); ++i) {
        if (layout_items_[i] == item) {
          layout_items_.erase(layout_items_.begin() + (int)i);
        }
      }

      throw Exception(
          LoggerSystem::Level::kWarning,
          "The iterator was not found in ImGuiLayoutItem, remove the error!");
    }
  } catch (Exception& e) {
#ifdef _DEBUG
    std::cerr << e.what() << std::endl;
#endif
  }
}
void ImGuiLayout::RemoveWidget(ImGuiWidget* widget) {
  try {
    if (widget_items_.empty()) {
      throw Exception(LoggerSystem::Level::kWarning,
                      "No widget storing ImGui, do not remove it!");
    }

    if (widget != nullptr) {
      for (int i = 0; i < widget_items_.size(); ++i) {
        if (widget_items_[i] == widget) {
          widget_items_.erase(widget_items_.begin() + i);
        }
      }

      throw Exception(
          LoggerSystem::Level::kWarning,
          "The iterator was not found in widget, remove the error!");
    }
  } catch (Exception& e) {
#ifdef _DEBUG
    std::cerr << e.what() << std::endl;
#endif
  }
}
ImGuiLayout::~ImGuiLayout() {
  for (auto item : widget_items_) {
    delete item;
  }

  for (auto item : layout_items_) {
    delete item;
  }
}
void ImGuiLayout::Update() {
  Rect current_geometry = GetGeometry();
  SetGeometry(current_geometry);
}
bool ImGuiLayout::IsEnabled() const {
  return enabled_;
}
void ImGuiLayout::Enable() {
  enabled_ = true;
}
void ImGuiLayout::Disable() {
  enabled_ = false;
}
ImGuiLayout::ImGuiLayout() : enabled_(true),spacing_(10) {}
