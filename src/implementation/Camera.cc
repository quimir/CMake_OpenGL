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

#include "include/Camera.h"
#include "include/Exception.h"
#include "include/LoggerSystem.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

const glm::vec3& Camera::GetWorldUp() const {
  return world_up_;
}

void Camera::SetWorldUp(const glm::vec3& world_up) {
  world_up_ = world_up;
  UpdateCameraVectors();
}

glm::float32 Camera::GetYaw() const {
  return yaw_;
}

void Camera::SetYaw(glm::float32 yaw) {
  yaw_ = yaw;
  UpdateCameraVectors();
}

glm::float32 Camera::GetPitch() const {
  return pitch_;
}

void Camera::SetPitch(glm::float32 pitch) {
  pitch_ = pitch;
  UpdateCameraVectors();
}

glm::float32 Camera::GetMovementSpeed() const {
  return movement_speed_;
}

void Camera::SetMovementSpeed(glm::float32 movement_speed) {
  movement_speed_ = movement_speed;
}

glm::float32 Camera::GetMouseSensitivity() const {
  return mouse_sensitivity_;
}

void Camera::SetMouseSensitivity(glm::float32 mouse_sensitivity) {
  mouse_sensitivity_ = mouse_sensitivity;
}

glm::float32 Camera::GetZoom() const {
  return zoom_;
}

void Camera::SetZoom(glm::float32 zoom) {
  zoom_ = zoom;
}

const glm::vec3& Camera::GetPosition() const {
  return position_;
}

void Camera::SetPosition(const glm::vec3& position) {
  position_ = position;
}

const glm::vec3& Camera::GetFront() const {
  return front_;
}

void Camera::SetFront(const glm::vec3& front) {
  front_ = front;
  UpdateCameraVectors();
}

const glm::vec3& Camera::GetUp() const {
  return up_;
}

void Camera::SetUp(const glm::vec3& up) {
  up_ = up;
}

const glm::vec3& Camera::GetRight() const {
  return right_;
}

void Camera::SetRight(const glm::vec3& right) {
  right_ = right;
  UpdateCameraVectors();
}

glm::float32 Camera::GetFarPlane() const {
  return far_plane_;
}

void Camera::SetFarPlane(glm::float32 far_plane) {
  far_plane_ = far_plane;
}

glm::float32 Camera::GetNearPlane() const {
  return near_plane_;
}

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::float32 yaw,
               glm::float32 pitch, glm::float32 near_plane,
               glm::float32 far_plane) {
  Initialized(position, up, yaw, pitch, near_plane, far_plane);
}

void Camera::UpdateCameraVectors() {
  // Quaternions are used to avoid gimbal deadlocks.
  glm::quat quaternion_yaw = glm::angleAxis(glm::radians(yaw_), world_up_);
  glm::quat quaternion_pitch =
      glm::angleAxis(glm::radians(pitch_), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat orientation=quaternion_yaw*quaternion_pitch;
  front_=glm::normalize(orientation*glm::vec3(0.0f,0.0f,-1.0f));
  
  // Update using Euler Angle
//  glm::vec3 front;
//  front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
//  front.y = sin(glm::radians(pitch_));
//  front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
//  this->front_ = glm::normalize(front);

  /* 
   * Also re-calculate the right and up vector
   */

  // Normalize the vectors, because their length gets closer to 0 the more you look up or
  // down which results in slower movement.
  this->right_ = glm::normalize(glm::cross(front_, world_up_));
  this->up_ = glm::normalize(glm::cross(right_, front_));
}

Camera::Camera(glm::float32 pos_x, glm::float32 pos_y, glm::float32 pos_z,
               glm::float32 up_x, glm::float32 up_y, glm::float32 up_z,
               glm::float32 yaw, glm::float32 pitch, glm::float32 near_plane,
               glm::float32 far_plane) {
  Initialized(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), yaw,
              pitch, near_plane, far_plane);
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(this->position_, this->position_ + this->front_,
                     this->up_);
}

void Camera::ProcessKeyboard(Camera::CameraMovement direction,
                             glm::float64 delta_time) {
  if (!this->enabled_)
    return;
  glm::float32 velocity =
      movement_speed_ * static_cast<glm::float32>(delta_time);
  switch (direction) {
    case CameraMovement::kForward:
      position_ += front_ * velocity;
      break;
    case CameraMovement::kBackward:
      position_ -= front_ * velocity;
      break;
    case CameraMovement::kLeft:
      position_ -= right_ * velocity;
      break;
    case CameraMovement::kRight:
      position_ += right_ * velocity;
      break;
  }
}

void Camera::ProcessMouseMovement(glm::float32 x_offset, glm::float32 y_offset,
                                  bool constrain_pitch) {
  if (!this->enabled_)
    return;
  x_offset *= mouse_sensitivity_;
  y_offset *= mouse_sensitivity_;

  yaw_ -= x_offset;
  pitch_ += y_offset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrain_pitch) {
    if (this->pitch_ > 89.0f) {
      this->pitch_ = 89.0f;
    }
    if (this->pitch_ < -89.0f) {
      this->pitch_ = -89.0f;
    }
  }

  // Update front, right and up vectors using the updated Euler angles
  UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(glm::float32 y_offset) {
  if (!this->enabled_)
    return;
  zoom_ -= static_cast<glm::float32>(y_offset);
  if (zoom_ < 1.0f) {
    zoom_ = 1.0f;
  }
  if (zoom_ > 45.0f) {
    zoom_ = 45.0f;
  }
}
void Camera::ProcessKeyboard(Camera::CameraMovement direction,
                             glm::float32 delta_time) {
  if (!enabled_)
    return;
  glm::float32 velocity = movement_speed_ * delta_time;
  switch (direction) {
    case CameraMovement::kForward:
      position_ += front_ * velocity;
      break;
    case CameraMovement::kBackward:
      position_ -= front_ * velocity;
      break;
    case CameraMovement::kLeft:
      position_ -= right_ * velocity;
      break;
    case CameraMovement::kRight:
      position_ += right_ * velocity;
      break;
  }
}
void Camera::ProcessMouseMovement(glm::float64 x_offset, glm::float64 y_offset,
                                  bool constrain_pitch) {
  if (!enabled_)
    return;
  x_offset *= mouse_sensitivity_;
  y_offset *= mouse_sensitivity_;

  yaw_ += static_cast<glm::float32>(x_offset);
  pitch_ += static_cast<glm::float32>(y_offset);

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrain_pitch) {
    if (this->pitch_ > 89.0f) {
      this->pitch_ = 89.0f;
    }
    if (this->pitch_ < -89.0f) {
      this->pitch_ = -89.0f;
    }
  }

  // Update front, right and up vectors using the updated Euler angles
  UpdateCameraVectors();
}
void Camera::Initialized(glm::vec3 position, glm::vec3 world_up,
                         glm::float32 yaw, glm::float32 pitch,
                         glm::float32 near_plane, glm::float32 far_plane) {
  this->front_ = glm::vec3(0.0f, 0.0f, -1.0f);
  movement_speed_ = cameraconstsetting::kSpeed;
  mouse_sensitivity_ = cameraconstsetting::kSensitivity;
  zoom_ = cameraconstsetting::kZoom;
  enabled_ = true;
  near_plane_ = near_plane;
  far_plane_ = far_plane;
  position_ = position;
  world_up_ = world_up;
  yaw_ = yaw;
  pitch_ = pitch;
  UpdateCameraVectors();
}
bool Camera::IsEnabled() const {
  return enabled_;
}
void Camera::Enable() {
  this->enabled_ = true;
}
void Camera::DisEnable() {
  this->enabled_ = false;
}

void Camera::SetNearPlane(glm::float32 near_plane) {
  try {
    if (near_plane > far_plane_) {
      near_plane = far_plane_ - 1;
      throw Exception(
          LoggerSystem::Level::kWarning,
          "Warning! The parameter set is wrong, and the distance of the near "
          "clipping plane is larger than the distance of the far clipping "
          "plane. "
          "The distance of the near clipping plane is adjusted as follows. " +
              std::to_string(near_plane));
    }
    near_plane_ = near_plane;
  } catch (Exception& e) {
    std::cerr
        << "The camera near plane error occurred for the following reasons: "
        << e.what() << std::endl;
    exit(0);
  }
}

glm::mat4 Camera::GetProjectionMatrix(const float width,
                                      const float height) const {
  return glm::perspective(glm::radians(zoom_), width / height, near_plane_,
                          far_plane_);
}

void Camera::ResetCamera(glm::vec3 position, glm::vec3 world_up,
                         glm::float32 yaw, glm::float32 pitch,
                         glm::float32 near_plane, glm::float32 far_plane) {
  Initialized(position, world_up, yaw, pitch, near_plane, far_plane);
}
void Camera::RotateYaw(glm::float32 angle) {
  if (!enabled_)
    return;

  this->yaw_ += angle;
  UpdateCameraVectors();
}
