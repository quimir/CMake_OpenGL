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

#include "include/Core/Size.h"
#include <algorithm>

Size::Size(int width, int height) : width_(width), height_(height) {}
Size::Size() : width_(0), height_(0) {}
bool Size::IsEmpty() const {
  return (width_ <= 0 || height_ <= 0);
}
bool Size::IsNull() const {
  return (width_ == 0 && height_ == 0);
}
bool Size::IsValid() const {
  return (width_ >= 0 && height_ >= 0);
}
Size Size::BoundedTo(const Size& other_size) const {
  return Size(std::min(width_, other_size.width_),
              std::min(height_, other_size.height_));
}
Size Size::ExpandedTo(const Size& other_size) const {
  return Size(std::max(width_, other_size.width_),
              std::max(height_, other_size.height_));
}
void Size::Scale(int width, int height) {
  width_ = width;
  height_ = height;
}
void Size::Scale(const Size& s) {
  Scale(s.width_, s.height_);
}
Size Size::Transposed() const {
  return {height_, width_};
}
void Size::Transpose() {
  std::swap(width_, height_);
}
int Size::Width() const {
  return width_;
}
void Size::SetWidth(int width) {
  width_ = width;
}
int Size::Height() const {
  return height_;
}
void Size::SetHeight(int height) {
  height_ = height;
}
int& Size::RWidth() {
  return width_;
}
int& Size::RHeight() {
  return height_;
}
