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

#include "include/Core/Rect.h"
#include <algorithm>

Rect::Rect(const Point& top_left, const Point& bottom_right) noexcept
    : x_(top_left.x()),
      y_(top_left.y()),
      width_(bottom_right.x() - top_left.x()),
      height_(bottom_right.y() - top_left.y()) {}
Rect::Rect(const Point& top_left, const Size& size) noexcept
    : x_(top_left.x()),
      y_(top_left.y()),
      width_(size.Width()),
      height_(size.Height()) {}
Rect::Rect(int x, int y, int width, int height) noexcept
    : x_(x), y_(y), width_(width), height_(height) {}
void Rect::Adjust(int dx1, int dy1, int dx2, int dy2) noexcept {
  x_ += dx1;
  y_ += dy1;
  width_ += dx2;
  height_ += dy2;
}
Rect Rect::Adjusted(int dx1, int dy1, int dx2, int dy2) const noexcept {
  return {x_ + dx1, y_ + dy1, width_ + dx2, height_ + dy2};
}
int Rect::Bottom() const noexcept {
  return y_ + height_;
}
Point Rect::BottomLeft() const noexcept {
  return {Left(), Bottom()};
}
int Rect::Left() const noexcept {
  return x_;
}
int Rect::Top() const noexcept {
  return y_;
}
Point Rect::BottomRight() const noexcept {
  return {Right(), Bottom()};
}
Point Rect::Center() const noexcept {
  return {(x_ + width_) / 2, (y_ + height_) / 2};
}
bool Rect::Contains(int x, int y, bool proper) const noexcept{
  if (proper) {
    return x > x_ && x < width_ && y > y_ && y < height_;
  } else {
    return x >= x_ && x <= width_ && y >= y_ && y <= height_;
  }
}
bool Rect::Contains(Point& point, bool proper) const noexcept {
  return Contains(point.x(), point.y(), proper);
}
bool Rect::Contains(Rect& rect_angle, bool proper) const noexcept {
  if (proper) {
    return rect_angle.x_ > x_ && rect_angle.width_ < width_ &&
           rect_angle.y_ > y_ && rect_angle.height_ < height_;
  } else {
    return rect_angle.x_ >= x_ && rect_angle.width_ < width_ &&
           rect_angle.y_ >= y_ && rect_angle.height_ < height_;
  }
}
bool Rect::Contains(int x, int y) const noexcept{
  return x >= x_ && x <= width_ && y >= y_ && y <= height_;
}
void Rect::GetCoords(int* x1, int* y1, int* x2, int* y2) const noexcept {
  if (x1)
    *x1 = x_;
  if (y1)
    *y1 = y_;
  if (x2)
    *x2 = x_ + width_;
  if (y2)
    *y2 = y_ + height_;
}
void Rect::GetRect(int* x1, int* y1, int* width, int* height) const noexcept {
  if (x1)
    *x1 = x_;
  if (y1)
    *y1 = y_;
  if (width)
    *width = width_;
  if (height)
    *height = height_;
}
int Rect::GetHeight() const noexcept {
  return height_;
}
Rect Rect::Intersected(const Rect& rect_angle) const noexcept {
  int x1 = std::max(x_, rect_angle.x_);
  int y1 = std::max(y_, rect_angle.y_);
  int x2 = std::min(width_, rect_angle.width_);
  int y2 = std::min(height_, rect_angle.height_);

  // Check if the intersection is valid (i.e., the rectangles actually intersect)
  if (x1 < x2 && y1 < y2) {
    return {x1, y1, x2 - x1, y2 - y1};
  } else {
    return {0, 0, 0, 0};  // Return an empty rectangle if no intersection
  }
}
bool Rect::Intersects(const Rect& rect_angle) const noexcept {
  return !(rect_angle.width_ <= x_ || rect_angle.x_ >= width_ ||
           rect_angle.height_ <= y_ || rect_angle.y_ >= height_);
}
Rect Rect::Normalized() const noexcept {
  int left = std::min(x_, width_);
  int right = std::max(x_, width_);
  int top = std::min(y_, height_);
  int bottom = std::max(y_, height_);

  return {left, top, right, bottom};
}
bool Rect::IsEmpty() const noexcept {
  return Left() > Right() || Top() > Bottom();
}
int Rect::Right() const noexcept {
  return x_ + width_;
}
Point Rect::TopLeft() const noexcept {
  return {Left(), Top()};
}
bool Rect::IsNull() const noexcept {
  return (Right() == Left() - 1 && Bottom() == Top() - 1) || (IsEmpty()) ||
         (Right() == Left() && Bottom() == Top());
}
bool Rect::IsValid() const noexcept {
  return Left() <= Right() && Top() <= Bottom();
}
void Rect::MoveBottom(int y) noexcept {
  y_ = y - height_;
}
void Rect::MoveBottomLeft(const Point& position) noexcept {
  x_ = position.x() + width_;
  y_ = position.y() + height_;
}
void Rect::MoveBottomRight(const Point& position) noexcept {
  x_ = position.x() - width_;
  y_ = position.y() - height_;
}
void Rect::MoveCenter(const Point& position) noexcept {
  x_ = position.x() + width_ / 2;
  y_ = position.y() + height_ / 2;
}
void Rect::MoveLeft(int x) noexcept {
  x_ = x;
}
void Rect::MoveRight(int x) noexcept {
  x_ += x;
}
void Rect::MoveTo(int x, int y) noexcept {
  x_ = x;
  y_ = y;
}
void Rect::MoveTo(const Point& position) noexcept {
  MoveTo(position.x(), position.y());
}
void Rect::MoveTop(int y) noexcept {
  y_ = y;
}
void Rect::MoveTopLeft(const Point& position) noexcept {
  MoveTo(position.x(), position.y());
}
void Rect::MoveTopRight(const Point& position) noexcept {
  x_ = position.x() - width_;
  y_ = position.y();
}
Size Rect::size() const noexcept {
  return {width_, height_};
}
int Rect::Area() const noexcept {
  return width_ * height_;
}
void Rect::Translate(int dx, int dy) noexcept {
  x_ += dx;
  y_ += dy;
}
void Rect::Translate(const Point& offset) noexcept {
  x_ += offset.x();
  y_ += offset.y();
}
Rect Rect::Translated(int dx, int dy) const noexcept {
  return {x_ + dx, y_ + dy, width_, height_};
}
Rect Rect::Translated(const Point& offset) const noexcept {
  return {x_ + offset.x(), y_ + offset.y(), width_, height_};
}
Rect Rect::Transposed() const noexcept {
  return {x_, y_, height_, width_};
}
int Rect::GetX() const noexcept {
  return x_;
}
void Rect::SetX(int x) noexcept {
  x_ = x;
}
int Rect::GetY() const noexcept {
  return y_;
}
void Rect::SetY(int y) noexcept {
  y_ = y;
}
int Rect::GetWidth() const noexcept {
  return width_;
}
void Rect::SetWidth(int width) noexcept {
  width_ = width;
}
void Rect::SetHeight(int height) noexcept {
  height_ = height;
}
void Rect::Reset(int x, int y, int width, int height) noexcept {
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;
}
Point Rect::GetPoint() const noexcept {
  return {x_, y_};
}
void Rect::SetPoint(Point new_point) noexcept {
  x_ = new_point.x();
  y_ = new_point.y();
}

void Rect::SetPoint(int x, int y) noexcept {
  x_ = x;
  y_ = y;
}
Size Rect::GetSize() const noexcept {
  return {width_, height_};
}

void Rect::SetSize(Size new_size) noexcept {
  width_ = new_size.Width();
  height_ = new_size.Height();
}
void Rect::SetSize(int width, int height) noexcept {
  width_ = width;
  height_ = height;
}
Point Rect::TopRight() const noexcept {
  return {Right(), Top()};
}
Rect Rect::Span(const Point& p1, const Point& p2) noexcept {
  int point_min_x = std::min(p1.x(), p2.x());
  int point_min_y = std::min(p1.y(), p2.y());
  int point_max_x = std::min(p1.x(), p2.x());
  int point_max_y = std::min(p1.y(), p2.y());

  return {Point(point_min_x, point_max_y),
          Size(point_max_x - point_min_x, point_max_y - point_min_y)};
}
