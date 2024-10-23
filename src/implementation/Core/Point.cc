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

//
// Created by Acer on 2024/8/18.
//

#include "include/Core/Point.h"
#include <cmath>

int& Point::Rx() {
  return xp_;
}
int& Point::Ry() {
  return yp_;
}
void Point::SetX(int x) {
  if (xp_ != x)
    xp_ = x;
}
void Point::SetY(int y) {
  if (yp_ != y)
    yp_ = y;
}
Point Point::Transposed() const {
  return {yp_, xp_};
}
int Point::ManhattanLength() const {
  return std::abs(xp_) + std::abs(yp_);
}
int Point::x() const {
  return xp_;
}
int Point::y() const {
  return yp_;
}
Point& Point::operator*=(float factor) {
  xp_ = static_cast<int>(std::round(static_cast<float>(xp_) * factor));
  yp_ = static_cast<int>(std::round(static_cast<float>(yp_) * factor));
  return *this;
}
Point& Point::operator*=(double factor) {
  xp_ = static_cast<int>(std::round(xp_ * factor));
  yp_ = static_cast<int>(std::round(yp_ * factor));
  return *this;
}
bool Point::operator<(const Point& rhs) const {
  if (xp_ < rhs.xp_)
    return true;
  if (rhs.xp_ < xp_)
    return false;
  return yp_ < rhs.yp_;
}

bool Point::operator>(const Point& rhs) const {
  return rhs < *this;
}

bool Point::operator<=(const Point& rhs) const {
  return !(rhs < *this);
}

bool Point::operator>=(const Point& rhs) const {
  return !(*this < rhs);
}
bool Point::operator==(const Point& rhs) const {
  return xp_ == rhs.xp_ && yp_ == rhs.yp_;
}
bool Point::operator!=(const Point& rhs) const {
  return !(rhs == *this);
}
Point& Point::operator+(const Point& p) {
  xp_ += p.xp_;
  yp_ += p.yp_;
  return *this;
}
Point& Point::operator-(const Point& p) {
  xp_ -= p.xp_;
  yp_ -= p.yp_;
  return *this;
}
bool Point::IsNull() const {
  return (xp_ == 0 && yp_ == 0);
}
template <typename T>
Point& Point::operator/=(T& p) {
  xp_ /= static_cast<int>(std::round(xp_ / p));
  yp_ /= static_cast<int>(std::round(yp_ / p));
  return *this;
}
