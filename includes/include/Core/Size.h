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

class Size {
 public:
  Size(int width, int height);

  Size();

  bool IsEmpty() const;

  bool IsNull() const;

  bool IsValid() const;

  Size BoundedTo(const Size& other_size) const;

  Size ExpandedTo(const Size& other_size) const;

  void Scale(int width, int height);

  void Scale(const Size& s);

  Size Transposed() const;

  void Transpose();

  int Width() const;
  
  void SetWidth(int width);
  
  int Height() const;
  
  void SetHeight(int height);
  
  int& RWidth();
  
  int& RHeight();

 private:
  int width_;
  int height_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CORE_SIZE_H_
