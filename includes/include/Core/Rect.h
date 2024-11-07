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

/**
 * @class The Rect class defines a rectangle in the plane using integer 
 * precision. 
 * 
 * A rectangle is normally expressed as a top-left corner and a size. The size 
 * (width and height) of a Rect is always equivalent to the mathematical 
 * rectangle that forms the basis for its rendering.\n\n
 * 
 * A Rect can be constructed with a set of left, top, width and height integers,
 * or from a Point and a Size. The following code creates two identical 
 * rectangles.
 * @code
 * Rect r1(100,200,11,16);
 * Rect r2(Point(100,200),Size(11,16));
 * @endcode
 * 
 * The Rect class provides a collection of functions that return the various 
 * rectangle coordinates, and enable manipulation of these. Rect also provides 
 * functions to move the rectangle relative to the various coordinates. 
 * In addition there is a MoveTo() function that moves the rectangle, leaving 
 * its top left corner at the given coordinates. Alternatively, the Translate() 
 * function moves the rectangle the given offset relative to the current 
 * position, and the Translated() function returns a translated copy of this 
 * rectangle.\n\n
 * 
 * The Size() function returns the rectangle's dimensions as a Size. The 
 * dimensions can also be retrieved separately using the Width() and Height() 
 * functions. To manipulate the dimensions use the SetSize(), SetWidth() or 
 * SetHeight() functions. Alternatively, the size can be changed by applying 
 * either of the functions setting the rectangle coordinates, for example, 
 * SetBottom() or SetRight().\n\n
 * 
 * The Contains() function tells whether a given point is inside the rectangle 
 * or not, and the Intersects() function returns true if this rectangle 
 * intersects with a given rectangle. The Rect class also provides the 
 * Intersected() function which returns the intersection rectangle, and the 
 * United() function which returns the rectangle that encloses the given.\n\n
 * 
 * The IsEmpty() function returns true if Left() > Right() or Top() > Bottom(). 
 * Note that an empty rectangle is not valid: The IsValid() function returns 
 * true if Left() <= Right() and Top() <= Bottom(). A null rectangle (IsNull() 
 * == true) on the other hand, has both width and height set to 0.
 */
class Rect {
 public:
  /**
   * Constructs a rectangle with the given topLeft and bottomRight corners, 
   * both included.\n\n
   * 
   * If bottomRight is to higher and to the left of topLeft, the rectangle 
   * defined is instead non-inclusive of the corners.
   * @param top_left The left point of the rectangle.
   * @param bottom_right Rectangular underground far right.
   */
  Rect(const Point& top_left, const Point& bottom_right) noexcept;

  /**
   * Constructs a rectangle with the given topLeft corner and the given size.
   */
  Rect(const Point& top_left, const Size& size) noexcept;

  /**
   * Constructs a rectangle with (x, y) as its top-left corner and the given 
   * width and height.
   */
  Rect(int x, int y, int width, int height) noexcept;

  /**
   * Adds dx1, dy1, dx2 and dy2 respectively to the existing coordinates of the 
   * rectangle.
   */
  void Adjust(int dx1, int dy1, int dx2, int dy2) noexcept;

  /**
   * Returns a new rectangle with dx1, dy1, dx2 and dy2 added respectively to 
   * the existing coordinates of this rectangle. 
   */
  Rect Adjusted(int dx1, int dy1, int dx2, int dy2) const noexcept;

  /**
   * Returns the y-coordinate of the rectangle's bottom edge.
   */
  int Bottom() const noexcept;

  /**
   * Returns the position of the rectangle's bottom-left corner. 
   */
  Point BottomLeft() const noexcept;

  /**
   * Returns the position of the rectangle's bottom-right corner.
   */
  Point BottomRight() const noexcept;

  /**
   * Returns the center point of the rectangle.
   */
  Point Center() const noexcept;

  /**
   * This is an overloaded function.\n\n
   * 
   * Returns true if the given point is inside or on the edge of the rectangle, 
   * otherwise returns false. If proper is true, this function only returns
   * true if the given point is inside the rectangle (i.e., not on the edge).
   * @param x The x-coordinate of the point to be determined.
   * @param y The y-coordinate of the point to be determined.
   * @param proper If true then we need to determine whether the point is on 
   * the edge of the rectangle.
   */
  bool Contains(int x, int y, bool proper) const noexcept;

  /**
   * This is an overloaded function.\n\n
   * 
   * Returns true if the point (x, y) is inside this rectangle, otherwise 
   * returns false.
   * @param x The x-coordinate of the point to be determined.
   * @param y The y-coordinate of the point to be determined.
   */
  bool Contains(int x, int y) const noexcept;

  /**
   * Returns true if the given point is inside or on the edge of the rectangle, 
   * otherwise returns false. If proper is true, this function only returns true 
   * if the given point is inside the rectangle (i.e., not on the edge).
   * @param point Points that need to be decided.
   * @param proper If true then we need to determine whether the point is on 
   * the edge of the rectangle.
   */
  bool Contains(Point& point, bool proper = false) const noexcept;

  /**
   * This is an overloaded function.\n\n
   * 
   * Returns true if the given rectangle is inside this rectangle. otherwise 
   * returns false. If proper is true, this function only returns true if the 
   * rectangle is entirely inside this rectangle (not on the edge).
   * @param rect_angle The rectangle that needs to be determined.
   * @param proper If true then we need to determine whether the rectangle is 
   * on the edge of the rectangle.
   */
  bool Contains(Rect& rect_angle, bool proper = false) const noexcept;

  /**
   * Extracts the position of the rectangle's top-left corner to *x1 and *y1, 
   * and the position of the bottom-right corner to *x2 and *y2.
   */
  void GetCoords(int* x1, int* y1, int* x2, int* y2) const noexcept;

  /**
   * Extracts the position of the rectangle's top-left corner to *x and *y, 
   * and its dimensions to *width and *height.
   */
  void GetRect(int* x1, int* y1, int* width, int* height) const noexcept;

  /**
   * Returns the height of the rectangle.
   */
  int GetHeight() const noexcept;

  /**
   * Returns the intersection of this rectangle and the given rectangle.
   */
  Rect Intersected(const Rect& rect_angle) const noexcept;

  /**
   * Returns true if this rectangle intersects with the given rectangle 
   * (i.e., there is at least one pixel that is within both rectangles), 
   * otherwise returns false.\n\n
   * 
   * The intersection rectangle can be retrieved using the intersected() 
   * function.
   */
  bool Intersects(const Rect& rect_angle) const noexcept;

  /**
   * Returns a normalized rectangle; i.e., a rectangle that has a non-negative 
   * width and height.\n\n
   * 
   * If width() < 0 the function swaps the left and right corners, and it swaps 
   * the top and bottom corners if height() < 0. The corners are at the same 
   * time changed from being non-inclusive to inclusive.
   */
  Rect Normalized() const noexcept;

  /**
   * Returns true if the rectangle is empty, otherwise returns false.\n\n
   * 
   * An empty rectangle has a left() > right() or top() > bottom(). An empty 
   * rectangle is not valid (i.e., isEmpty() == !isValid()).\n\n
   * 
   * Use the normalized() function to retrieve a rectangle where the corners 
   * are swapped.
   */
  bool IsEmpty() const noexcept;

  /**
   * Returns true if the rectangle is a null rectangle, otherwise returns false.
   * \n\n
   * 
   * A null rectangle has both the width and the height set to 0 (i.e., right() 
   * == left() - 1 and bottom() == top() - 1). A null rectangle is also empty, 
   * and hence is not valid.
   */
  bool IsNull() const noexcept;

  /**
   * Returns true if the rectangle is valid, otherwise returns false.\n\n
   * 
   * A valid rectangle has a left() <= right() and top() <= bottom(). Note that 
   * non-trivial operations like intersections are not defined for invalid 
   * rectangles. A valid rectangle is not empty (i.e., isValid() == !isEmpty()).
   */
  bool IsValid() const noexcept;

  /**
   * Moves the rectangle vertically, leaving the rectangle's bottom edge at the 
   * given y coordinate. The rectangle's size is unchanged.
   * @param y Moving y coordinate.
   */
  void MoveBottom(int y) noexcept;

  /**
   * Moves the rectangle, leaving the bottom-left corner at the given position. 
   * The rectangle's size is unchanged.
   * @param position Move to coordinates.
   */
  void MoveBottomLeft(const Point& position) noexcept;

  /**
   * Moves the rectangle, leaving the bottom-right corner at the given position. 
   * The rectangle's size is unchanged.
   * @param position Move to coordinates.
   */
  void MoveBottomRight(const Point& position) noexcept;

  /**
   * Moves the rectangle, leaving the center point at the given position. The 
   * rectangle's size is unchanged.
   * @param position Move to coordinates.
   */
  void MoveCenter(const Point& position) noexcept;

  /**
   * Moves the rectangle horizontally, leaving the rectangle's left edge at the 
   * given x coordinate. The rectangle's size is unchanged.
   * @param x Move to x coordinate.
   */
  void MoveLeft(int x) noexcept;

  /**
   * Moves the rectangle horizontally, leaving the rectangle's right edge at 
   * the given x coordinate. The rectangle's size is unchanged.
   * @param x Move to x coordinate.
   */
  void MoveRight(int x) noexcept;

  /**
   * Moves the rectangle, leaving the top-left corner at the given position 
   * (x, y). The rectangle's size is unchanged.
   * @param x Move to x coordinate.
   * @param y Move to y coordinate.
   */
  void MoveTo(int x, int y) noexcept;

  /**
   * Moves the rectangle, leaving the top-left corner at the given position.
   * @param position Move to coordinates.
   */
  void MoveTo(const Point& position) noexcept;

  /**
   * Moves the rectangle vertically, leaving the rectangle's top edge at the 
   * given y coordinate. The rectangle's size is unchanged.
   * @param y Move to y coordinate.
   */
  void MoveTop(int y) noexcept;

  /**
   * Moves the rectangle, leaving the top-left corner at the given position. 
   * The rectangle's size is unchanged.
   * @param position Move to coordinates.
   */
  void MoveTopLeft(const Point& position) noexcept;

  /**
   * Moves the rectangle, leaving the top-right corner at the given position. 
   * The rectangle's size is unchanged.
   * @param position Move to coordinates.
   */
  void MoveTopRight(const Point& position) noexcept;

  /**
   * Returns the x-coordinate of the rectangle's left edge. Equivalent to x().
   */
  int Left() const noexcept;

  /**
   * Returns the y-coordinate of the rectangle's top edge. Equivalent to y().
   */
  int Top() const noexcept;

  /**
   * Returns the position of the rectangle's top-left corner.
   */
  Point TopLeft() const noexcept;

  /**
   * Returns the position of the rectangle's top-right corner.
   */
  Point TopRight() const noexcept;

  /**
   * Returns the x-coordinate of the rectangle's right edge.
   */
  int Right() const noexcept;

  /**
   * Returns the size of the rectangle.
   */
  Size size() const noexcept;

  /**
   * Returns the area of the rectangle.
   */
  int Area() const noexcept;

  /**
   * Moves the rectangle dx along the x axis and dy along the y axis, relative 
   * to the current position. Positive values move the rectangle to the right 
   * and down.
   * @param dx The cumulative X-axis value.
   * @param dy The cumulative Y-axis value.
   */
  void Translate(int dx, int dy) noexcept;

  /**
   * This is an overloaded function.\n\n
   * 
   * Moves the rectangle offset.x() along the x axis and offset.y() along the y 
   * axis, relative to the current position.
   * @param offset The coordinates of the sum.
   */
  void Translate(const Point& offset) noexcept;

  /**
   * Returns a copy of the rectangle that is translated dx along the x axis and 
   * dy along the y axis, relative to the current position. Positive values 
   * move the rectangle to the right and down.
   * @param dx The cumulative X-axis value.
   * @param dy The cumulative Y-axis value.
   */
  Rect Translated(int dx, int dy) const noexcept;

  /**
   * This is an overloaded function.\n\n
   * 
   * Returns a copy of the rectangle that is translated offset.x() along the x 
   * axis and offset.y() along the y axis, relative to the current position.
   * @param offset The coordinates of the sum.
   */
  Rect Translated(const Point& offset) const noexcept;

  /**
   * Returns a copy of the rectangle that has its width and height exchanged:
   * @code
   * Rect r = {15, 51, 42, 24};
   * r = r.transposed(); // r == {15, 51, 24, 42}
   * @endcode
   */
  Rect Transposed() const noexcept;

  /**
   * Returns the x-coordinate of the rectangle's left edge. Equivalent to 
   * left().
   */
  int GetX() const noexcept;

  /**
   * Sets the left edge of the rectangle to the given x coordinate. May change 
   * the width, but will never change the right edge of the rectangle.
   * 
   * Equivalent to setLeft().
   * @param x The new x value.
   */
  void SetX(int x) noexcept;

  /**
   * Returns the y-coordinate of the rectangle's top edge. Equivalent to top().
   */
  int GetY() const noexcept;

  /**
   * Gets the coordinates of the rectangle.
   */
  Point GetPoint() const noexcept;

  /**
   * Set the rectangle's new coordinates.
   * @param new_point The value of the new coordinate.
   */
  void SetPoint(Point new_point) noexcept;

  /**
   * This is an overloaded function.\n\n
   * 
   * Take the new x and y as the coordinates of the rectangle.
   * @param x New x coordinates.
   * @param y New y coordinates.
   */
  void SetPoint(int x, int y) noexcept;

  /**
   * Sets the top edge of the rectangle to the given y coordinate. May change 
   * the height, but will never change the bottom edge of the rectangle.\n\n
   * 
   * Equivalent to setTop().
   * @param y The new y value.
   */
  void SetY(int y) noexcept;

  /**
   * Returns the width of the rectangle.
   */
  int GetWidth() const noexcept;

  /**
   * Sets the width of the rectangle to the given width. The right edge is 
   * changed, but not the left one.
   */
  void SetWidth(int width) noexcept;

  /**
   * Sets the height of the rectangle to the given height. The bottom edge is 
   * changed, but not the top one.
   */
  void SetHeight(int height) noexcept;

  /**
   * Returns the size of the rectangle.\n\n
   * 
   * note: Same as size().
   */
  Size GetSize() const noexcept;

  void SetSize(Size new_size) noexcept;

  void SetSize(int width, int height) noexcept;

  void Reset(int x, int y, int width, int height) noexcept;

  /**
   * Returns a rectangle spanning the two points p1 and p2, including both and 
   * everything in between.
   * @param p1 first point of.
   * @param p2 second point of.
   */
  static Rect Span(const Point& p1, const Point& p2) noexcept;

 private:
  int x_;
  int y_;
  int width_;
  int height_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CORE_RECT_H_
