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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_POINT_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_POINT_H_

class Point {
 public:
  constexpr Point() : xp_(0), yp_(0){};

  constexpr Point(int x, int y) : xp_(x), yp_(y){};

  bool IsNull() const;

  int& Rx();

  int& Ry();

  void SetX(int x);

  void SetY(int y);

  Point Transposed() const;

  int ManhattanLength() const;

  int x() const;

  int y() const;

  Point& operator*=(float factor);

  Point& operator*=(double factor);

  bool operator<(const Point& rhs) const;

  bool operator>(const Point& rhs) const;

  bool operator<=(const Point& rhs) const;

  bool operator>=(const Point& rhs) const;

  bool operator==(const Point& rhs) const;

  bool operator!=(const Point& rhs) const;

  Point& operator+(const Point& p);

  Point& operator-(const Point& p);

  template <typename T>
  Point& operator/=(T& p);

 private:
  int xp_;
  int yp_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_POINT_H_
