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

#include "Core/Size.h"
#include <algorithm>

Size::Size(int width, int height) noexcept
    : width_(width), height_(height) {}
Size::Size() noexcept : width_(0), height_(0) {}
bool Size::IsEmpty() const noexcept {
  return (width_ <= 0 || height_ <= 0);
}
bool Size::IsNull() const noexcept {
  return (width_ == 0 && height_ == 0);
}
bool Size::IsValid() const noexcept {
  return (width_ >= 0 && height_ >= 0);
}
Size Size::BoundedTo(const Size& other_size) const noexcept {
  return {std::min(width_, other_size.width_),
          std::min(height_, other_size.height_)};
}
Size Size::ExpandedTo(const Size& other_size) const noexcept {
  return {std::max(width_, other_size.width_),
          std::max(height_, other_size.height_)};
}
void Size::Scale(int width, int height, AspectRatioMode mode) noexcept {
  switch (mode) {
    case AspectRatioMode::kIgnoreAspectRatio:
      width_ = width;
      height_ = height;
      break;
    case AspectRatioMode::kKeepAspectRatio: {
      auto aspect_ratio = static_cast<float>(width_) / height_;
      if (width / static_cast<float>(height) > aspect_ratio) {
        this->height_ = height;
        this->width_ = static_cast<int>(height * aspect_ratio);
      } else {
        this->width_ = width;
        this->height_ = static_cast<int>(width / aspect_ratio);
      }
      break;
    }
    case AspectRatioMode::kKeepAspectRatioByExpanding: {
      float aspect_ratio = static_cast<float>(width_) / height_;
      if (width / static_cast<float>(height) > aspect_ratio) {
        this->width_ = width;
        this->height_ = static_cast<int>(width / aspect_ratio);
      } else {
        this->height_ = height;
        this->width_ = static_cast<int>(height * aspect_ratio);
      }
    } break;
  }
}
void Size::Scale(const Size& s, AspectRatioMode mode) noexcept {
  Scale(s.width_, s.height_, mode);
}
Size Size::Transposed() const noexcept {
  return {height_, width_};
}
void Size::Transpose() noexcept {
  std::swap(width_, height_);
}
int Size::Width() const noexcept{
  return width_;
}
void Size::SetWidth(int width) noexcept{
  width_ = width;
}
int Size::Height() const noexcept{
  return height_;
}
void Size::SetHeight(int height) noexcept{
  height_ = height;
}
int& Size::RWidth() {
  return width_;
}
int& Size::RHeight() {
  return height_;
}
