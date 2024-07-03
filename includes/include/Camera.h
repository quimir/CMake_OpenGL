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

#ifndef CMAKE_OPEN_INCLUDES_INCLUDE_CAMERA_H_
#define CMAKE_OPEN_INCLUDES_INCLUDE_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Default camera values
 */
namespace cameraconstsetting {
constexpr glm::float32 kYaw = -90.0f;
constexpr glm::float32 kPitch = 0.0f;
constexpr glm::float32 kSpeed = 2.5f;
constexpr glm::float32 kSensitivity = 0.1f;
constexpr glm::float32 kZoom = 45.0f;
}

class Camera {
 public:
  enum class CameraMovement {
	kForward,
	kBackward,
	kLeft,
	kRight
  };

 public:
  explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
				  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
				  glm::float32 yaw = cameraconstsetting::kYaw,
				  glm::float32 pitch = cameraconstsetting::kPitch);

  Camera(glm::float32 pos_x,
		 glm::float32 pos_y,
		 glm::float32 pos_z,
		 glm::float32 up_x,
		 glm::float32 up_y,
		 glm::float32 up_z,
		 glm::float32 yaw,
		 glm::float32 pitch);

  glm::mat4 GetViewMatrix() const;

  void ProcessKeyboard(Camera::CameraMovement direction, glm::float64 delta_time);

  void ProcessKeyboard(Camera::CameraMovement direction, glm::float32 delta_time);

  void ProcessMouseMovement(glm::float32 x_offset,
							glm::float32 y_offset,
							bool constrain_pitch = true);

  void ProcessMouseMovement(glm::float64 x_offset,
							glm::float64 y_offset,
							bool constrain_pitch = true);

  void ProcessMouseScroll(glm::float32 y_offset);

  const glm::vec3 &GetWorldUp() const;

  void SetWorldUp(const glm::vec3 &world_up);

  glm::float32 GetYaw() const;

  void SetYaw(glm::float32 yaw);

  glm::float32 GetPitch() const;

  void SetPitch(glm::float32 pitch);

  glm::float32 GetMovementSpeed() const;

  void SetMovementSpeed(glm::float32 movement_speed);

  glm::float32 GetMouseSensitivity() const;

  void SetMouseSensitivity(glm::float32 mouse_sensitivity);

  glm::float32 GetZoom() const;

  void SetZoom(glm::float32 zoom);

  const glm::vec3 &GetPosition() const;

  void SetPosition(const glm::vec3 &position);

  const glm::vec3 &GetFront() const;

  void SetFront(const glm::vec3 &front);

  const glm::vec3 &GetUp() const;

  void SetUp(const glm::vec3 &up);

  const glm::vec3 &GetRight() const;

  void SetRight(const glm::vec3 &right);

 private:
  void UpdateCameraVectors();

 private:
  // Camera attributes
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;
  // Euler angles
  glm::float32 yaw_;
  glm::float32 pitch_;
  // Camera options
  glm::float32 movement_speed_;
  glm::float32 mouse_sensitivity_;
  glm::float32 zoom_;

};

#endif //CMAKE_OPEN_INCLUDES_INCLUDE_CAMERA_H_
