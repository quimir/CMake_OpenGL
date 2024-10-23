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

#include "include/Core/Widget.h"
Widget::Widget(const Rect& rect) : rect_(rect) {}
Widget::Widget(int x, int y, int width, int height)
    : rect_(x, y, width, height) {}
void Widget::ResetWidget(const Rect& rect_angle) {
  rect_ = rect_angle;
}
void Widget::ResetWidget(int x, int y, int width, int height) {
  rect_.Reset(x, y, width, height);
}
const Rect& Widget::GetRect() const {
  return rect_;
}
void Widget::SetRect(const Rect& rect) {
  rect_ = rect;
}
Rect& Widget::RGetRect() {
  return rect_;
}
int Widget::GetWidth() const {
  return rect_.GetWidth();
}
void Widget::SetWidth(int width) {
  rect_.SetWidth(width);
}
int Widget::GetHeight() const {
  return rect_.GetHeight();
}
void Widget::SetHeight(int height) {
  rect_.SetHeight(height);
}

void Widget::ReSizeWidget(int width, int height) {
  rect_.SetWidth(width);
  rect_.SetHeight(height);
}
