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

#include "MacroDefinition.h"

/**
 * @class The Point class defines a point in the plane using integer precision.
 * 
 * A point is specified by a x coordinate and an y coordinate which can be 
 * accessed using the x() and y() functions. The IsNull() function returns 
 * true if both x and y are set to 0. The coordinates can be set (or altered) 
 * using the SetX() and SetY() functions, or alternatively the Rx() and Ry() 
 * functions which return references to the coordinates (allowing direct 
 * manipulation).\n\n
 * 
 * Given a point p, the following statements are all equivalent:
 * @code
 * Point p;
 * p.SetX(p.x()+1);
 * p+=Point(1,0);
 * p.rx()++;
 * @endcode
 * 
 * A Point object can also be used as a vector: Addition and subtraction are 
 * defined as for vectors (each component is added separately).\n\n
 * 
 * A Point object can also be divided or multiplied by an int or a real.In 
 * addition, the Point class provides the ManhattanLength() function which 
 * gives an inexpensive approximation of the length of the Point object 
 * interpreted as a vector.
 */
class SHARED_FRAMEWORK_API Point {
 public:
  /**
   * Constructs a null point, i.e. with coordinates (0, 0)
   */
  Point() : xp_(0), yp_(0){};

  /**
   * Constructs a point with the given coordinates (xpos, ypos).
   * @param x xpos
   * @param y ypos
   */
  Point(int x, int y) : xp_(x), yp_(y){};

  /**
   * Returns true if both the x and y coordinates are set to 0, otherwise 
   * returns false.
   */
  bool IsNull() const noexcept;

  /**
   * Returns true if both the x and y coordinates are set to 0, otherwise 
   * returns false.Same as IsNull(), but named uniformly.
   */
  bool IsEmpty() const noexcept;

  /**
   * Returns a reference to the x coordinate of this point.
   * @return References to x coordinates.
   */
  int& Rx() noexcept;

  /**
   * Returns a reference to the x coordinate of this point.
   * @return References to y coordinates.
   */
  int& Ry() noexcept;

  /**
   * Sets the x coordinate of this point to the given x coordinate.
   * @param x The new x coordinate value.
   */
  void SetX(int x) noexcept;

  /**
   * Sets the y coordinate of this point to the given y coordinate.
   * @param y The new y coordinate value.
   */
  void SetY(int y) noexcept;

  /**
   * Returns a point with x and y coordinates.
   */
  Point Transposed() const noexcept;

  /**
   * Returns the sum of the absolute values of x() and y(), traditionally 
   * known as the "Manhattan length" of the vector from the origin to the point. 
   */
  int ManhattanLength() const noexcept;

  /**
   * Returns the x coordinate of this point.
   */
  int x() const noexcept;

  /**
   * Returns the y coordinate of this point.
   */
  int y() const noexcept;

  /**
   * Returns the dot product of p1 and p2.
   * @param p1 first point of.
   * @param p2 second point of.
   * @return The dot product between p1 and p2.
   */
  static int DotProduct(const Point& p1, const Point& p2);

  /**
   * Multiplies this point's coordinates by the given factor, and returns a 
   * reference to this point.
   * 
   * Note that the result is rounded to the nearest integer as points are 
   * held as integers. 
   * @param factor multiplier.
   * @return Multiply the object after itself.
   */
  Point& operator*=(float factor);

  /**
   * Multiplies this point's coordinates by the given factor, and returns a 
   * reference to this point.
   * 
   * Note that the result is rounded to the nearest integer as points are 
   * held as integers. 
   * @param factor multiplier.
   * @return Multiply the object after itself.
   */
  Point& operator*=(double factor);

  /**
   * Compare whether the current point is less than the given point, in 
   * lexicographical order (compare x and then y)
   * @param rhs Point of comparison
   * @return Return true if the point being compared is > the current point, 
   * false otherwise.
   */
  bool operator<(const Point& rhs) const;

  /**
   * Compares whether the current point is greater than the given point.
   * @param rhs The point being compared.
   * @return Return true if the point being compared is < the current point, 
   * false otherwise.
   */
  bool operator>(const Point& rhs) const;

  /**
   * Compares whether the current point is less than or equal to the given 
   * point.
   * @param rhs The point being compared.
   * @return Return true if the point being compared is >= the current point, 
   * false otherwise.
   */
  bool operator<=(const Point& rhs) const;

  /**
   * Compares whether the current point is greater than or equal to the given 
   * point.
   * @param rhs The point being compared.
   * @return Return true if the point being compared is <= the current point, 
   * false otherwise.
   */
  bool operator>=(const Point& rhs) const;

  /**
   * Determine whether two points are equal.
   * @param rhs The point being compared.
   * @return Return true for equality, false for otherwise.
   */
  bool operator==(const Point& rhs) const;

  /**
   * Determine whether two points are not equal.
   * @param rhs The point being compared.
   * @return Return true for not equality, false for otherwise.
   */
  bool operator!=(const Point& rhs) const;

  /**
   * Add the given points to update the coordinates of the current point. 
   */
  Point& operator+(const Point& p);

  /**
   * Subtract from the given point and update the coordinates of the current 
   * point.
   */
  Point& operator-(const Point& p);

  template <typename T>
  /**
   * Scale according to the given parameters, update the coordinates of the 
   * current point (overload to any type).
   */
  Point& operator/=(T& p);

 private:
  int xp_;

  int yp_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_POINT_H_
