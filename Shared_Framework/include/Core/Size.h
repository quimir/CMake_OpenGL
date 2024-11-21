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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_CORE_SIZE_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_CORE_SIZE_H_

#include "MacroDefinition.h"

/**
 * @class The Size class defines the size of a two-dimensional object using 
 * integer point precision.
 * 
 * A size is specified by a Width() and a Height(). It can be set in the 
 * constructor and changed using the SetWidth(), SetHeight(), or Scale() 
 * functions, or using arithmetic operators.\n\n
 * 
 * A size can also be manipulated directly by retrieving references to the width
 * and height using the RWidth() and RHeight() functions.\n\n
 * 
 * Finally, the width and height can be swapped using the Transpose() function.
 * The IsValid() function determines if a size is valid (a valid size has both 
 * width and height greater than or equal to zero). The IsEmpty() function 
 * returns true if either of the width and height is less than, or equal to, 
 * zero, while the IsNull() function returns true only if both the width and 
 * the height is zero.                                                                                                                                                                                                                      Use the expandedTo() function to retrieve a size which holds the maximum height and width of this size and a given size. Similarly, the boundedTo() function returns a size which holds the minimum height and width of this size and a given size.
 */
class SHARED_FRAMEWORK_API Size {
 public:
  /**
   * This enum type defines what happens to the aspect ratio when scaling an 
   * rectangle.
   */
  enum class AspectRatioMode{
    // The size is scaled freely. The aspect ratio is not preserved.
    kIgnoreAspectRatio,
    // The size is scaled to a rectangle as large as possible inside a given 
    // rectangle, preserving the aspect ratio.
    kKeepAspectRatio,
    // The size is scaled to a rectangle as small as possible outside a given 
    // rectangle, preserving the aspect ratio.
    kKeepAspectRatioByExpanding
  };
 public:
  /**
   * Constructs a size with the given width and height.
   */
   Size(int width, int height)noexcept;

  /**
   * Constructs a size with an invalid width and height (i.e., isValid() 
   * returns false).
   */
  Size()noexcept;

  /**
   * Returns true if either of the width and height is less than or equal to 0; 
   * otherwise returns false.
   */
  bool IsEmpty() const noexcept;

  /**
   * Returns true if both the width and height is 0; otherwise returns false.
   */
  bool IsNull() const noexcept;

  /**
   * Returns true if both the width and height is equal to or greater than 0; 
   * otherwise returns false.
   */
  bool IsValid() const noexcept;

  /**
   * Returns a size holding the minimum width and height of this size and the 
   * given otherSize.
   * @param other_size The Size value to be included.
   */
  Size BoundedTo(const Size& other_size) const noexcept;

  /**
   * Returns a size holding the maximum width and height of this size and the 
   * given otherSize.
   * @param other_size The Size value to be included.
   */
  Size ExpandedTo(const Size& other_size) const noexcept;

  /**
   * Scales the size to a rectangle with the given width and height.
   * @param width The width of the zoom.
   * @param height Zoom the height.
   */
  void Scale(int width, int height, AspectRatioMode mode) noexcept;

  /**
   * This is an overloaded function.
   * Scales the size to a rectangle with the given size, 
   * @param s The given Size.
   */
  void Scale(const Size& s, AspectRatioMode mode) noexcept;

  /**
   * Returns a Size with width and height swapped.
   */
  Size Transposed() const noexcept;

  /**
   * Swaps the width and height values
   */
  void Transpose() noexcept;

  /**
   * Returns the width.
   */
  int Width() const noexcept;
  
  /**
   * Sets the width to the given width.
   */
  void SetWidth(int width)noexcept;
  
  /**
   * Returns the height.
   */
  int Height() const noexcept;
  
  /**
   * Sets the height to the given height.
   */
  void SetHeight(int height) noexcept;
  
  /**
   * Returns a reference to the width.
   */
  int& RWidth();
  
  /**
   * Returns a reference to the height.
   */
  int& RHeight();

 private:
  int width_;
  
  int height_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CORE_SIZE_H_
