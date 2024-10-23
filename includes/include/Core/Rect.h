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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_CORE_RECT_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_CORE_RECT_H_

#include "Point.h"
#include "Size.h"

class Rect {
 public:
  Rect(const Point& top_left, const Point& bottom_right);

  Rect(const Point& top_left, const Size& size);

  Rect(int x, int y, int width, int height);

  void Adjust(int dx1, int dy1, int dx2, int dy2);

  Rect Adjusted(int dx1, int dy1, int dx2, int dy2) const;

  Point BottomLeft() const;

  Point BottomRight() const;

  Point Center() const;

  bool Contains(int x, int y, bool proper) const;

  bool Contains(int x, int y) const;

  bool Contains(Point& point, bool proper = false) const;

  bool Contains(Rect& rect_angle, bool proper = false) const;

  void GetCoords(int* x1, int* y1, int* x2, int* y2) const;

  void GetRect(int* x1, int* y1, int* width, int* height) const;

  int GetHeight() const;

  Rect Intersected(const Rect& rect_angle) const;

  bool Intersects(const Rect& rect_angle) const;

  Rect Normalized() const;

  bool IsEmpty() const;

  bool IsNull() const;

  bool IsValid() const;

  void MoveBottom(int y);

  void MoveBottomLeft(const Point& position);

  void MoveBottomRight(const Point& position);

  void MoveCenter(const Point& position);

  void MoveLeft(int x);

  void MoveRight(int x);

  void MoveTo(int x, int y);

  void MoveTo(const Point& position);

  void MoveTop(int y);

  void MoveTopLeft(const Point& position);

  void MoveTopRight(const Point& position);

  int Left() const;

  int Top() const;

  Point TopLeft() const;

  int Right() const;

  int Bottom() const;

  Size size() const;

  int Area() const;

  void Translate(int dx, int dy);

  void Translate(const Point& offset);

  Rect Translated(int dx, int dy) const;

  Rect Translated(const Point& offset) const;

  Rect Transposed() const;

  int GetX() const;

  void SetX(int x);

  int GetY() const;

  void SetY(int y);

  int GetWidth() const;

  void SetWidth(int width);

  void SetHeight(int height);
  
  void Reset(int x,int y,int width,int height);

 private:
  int x_;
  int y_;
  int width_;
  int height_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CORE_RECT_H_
