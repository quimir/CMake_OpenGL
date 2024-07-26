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
#include <string>

/**
 * Default camera values
 */
namespace cameraconstsetting {
constexpr glm::float32 kYaw = -90.0f;
constexpr glm::float32 kPitch = 0.0f;
constexpr glm::float32 kSpeed = 2.5f;
constexpr glm::float32 kSensitivity = 0.1f;
constexpr glm::float32 kZoom = 45.0f;
constexpr glm::float32 kNear = 0.1f;
constexpr glm::float32 kFar = 100.0f;
}  // namespace cameraconstsetting

/**
 * The Camera class implements the camera control and the calculation of the 
 * view matrix. It uses Euler angles to control the position and orientation 
 * of the camera and provides methods to update the camera vector and handle 
 * keyboard and mouse input. This class supports copy construction and copy 
 * construction. There are no memory leaks.
 */
class Camera {
 public:
  /**
   * Manage the orientation of the camera.
   */
  enum class CameraMovement { kForward, kBackward, kLeft, kRight };

 public:
  /**
   * Constructor that initializes the camera's position, orientation, and view 
   * matrix.
   * @param position The position of the camera in world space.
   * @param up The world spatial orientation of the camera upward.
   * @param yaw Euler Angle of the camera around the Y-axis.
   * @param pitch Euler Angle of the camera around the X-axis.
   * @param near_plane Close clipping plane.
   * @param far_plane Far cut the plane.
   */
  explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                  glm::float32 yaw = cameraconstsetting::kYaw,
                  glm::float32 pitch = cameraconstsetting::kPitch,
                  glm::float32 near_plane = cameraconstsetting::kNear,
                  glm::float32 far_plane = cameraconstsetting::kFar);

  /**
   * Accepts floating-point arguments to initialize the camera's position, 
   * orientation, and view matrix.
   * @param pos_x The x-coordinate of the camera in world space.
   * @param pos_y The y-coordinate of the camera in world space.
   * @param pos_z The z-coordinate of the camera in world space.
   * @param up_x The x-coordinate of the camera pointing up.
   * @param up_y The y-coordinate of the camera pointing up.
   * @param up_z The z-coordinate of the camera pointing up.
   * @param yaw Euler Angle of the camera around the Y-axis.
   * @param pitch Euler Angle of the camera around the X-axis.
   * @param near_plane Close clipping plane.
   * @param far_plane Far cut the plane.
   */
  Camera(glm::float32 pos_x, glm::float32 pos_y, glm::float32 pos_z,
         glm::float32 up_x, glm::float32 up_y, glm::float32 up_z,
         glm::float32 yaw, glm::float32 pitch,
         glm::float32 near_plane = cameraconstsetting::kNear,
         glm::float32 far_plane = cameraconstsetting::kFar);

  /**
   * Method to obtain the camera view matrix.
   * @return Camera view matrix.
   */
  glm::mat4 GetViewMatrix() const;

  /**
   * The keyboard input is processed to move the camera according to the 
   * specified direction and time increment.
   * @param direction moving direction.
   * @param delta_time delta time.
   */
  void ProcessKeyboard(Camera::CameraMovement direction,
                       glm::float64 delta_time);
  /**
   * The keyboard input is processed to move the camera according to the 
   * specified direction and time increment.
   * @param direction moving direction.
   * @param delta_time delta time.
   */
  void ProcessKeyboard(Camera::CameraMovement direction,
                       glm::float32 delta_time);

  /**
   * Handles mouse movement and updates the camera orientation based on the 
   * mouse offset.
   * @param x_offset Horizontal mouse offset.
   * @param y_offset Vertical mouse offset.
   * @param constrain_pitch Whether to limit the pitch Angle.
   */
  void ProcessMouseMovement(glm::float32 x_offset, glm::float32 y_offset,
                            bool constrain_pitch = true);

  /**
   * Handles mouse movement and updates the camera orientation based on the 
   * mouse offset.
   * @param x_offset Horizontal mouse offset.
   * @param y_offset Vertical mouse offset.
   * @param constrain_pitch Whether to limit the pitch Angle.
   */
  void ProcessMouseMovement(glm::float64 x_offset, glm::float64 y_offset,
                            bool constrain_pitch = true);

  /**
   * Handles mouse wheel scrolling and resizes the camera view by the amount of 
   * scrolling.
   * @param y_offset Amount of mouse wheel scrolling.
   */
  void ProcessMouseScroll(glm::float32 y_offset);

  const glm::vec3& GetWorldUp() const;

  void SetWorldUp(const glm::vec3& world_up);

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

  const glm::vec3& GetPosition() const;

  void SetPosition(const glm::vec3& position);

  const glm::vec3& GetFront() const;

  void SetFront(const glm::vec3& front);

  const glm::vec3& GetUp() const;

  void SetUp(const glm::vec3& up);

  const glm::vec3& GetRight() const;

  void SetRight(const glm::vec3& right);

  glm::float32 GetNearPlane() const;

  void SetNearPlane(glm::float32 near_plane);

  glm::float32 GetFarPlane() const;

  void SetFarPlane(glm::float32 far_plane);

  void Enable();

  void DisEnable();

  bool IsEnabled() const;

  /**
   * The method of obtaining the camera projection matrix.
   * @param width Viewport width.
   * @param height Viewport height.
   * @return Camera projection matrix.
   */
  glm::mat4 GetProjectionMatrix(const float width, const float height) const;

  /**
   * Reset the camera's properties.
   */
  void ResetCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f),
                   glm::float32 yaw = cameraconstsetting::kYaw,
                   glm::float32 pitch = cameraconstsetting::kPitch,
                   glm::float32 near_plane = cameraconstsetting::kNear,
                   glm::float32 far_plane = cameraconstsetting::kFar);

 private:
  /**
   * Update the camera vectors.
   * The method uses Euler angles to calculate the forward, right, and up 
   * vectors of the camera.These vectors are calculated based on the position 
   * and orientation of the camera. They are used to compute the camera view 
   * matrix during rendering.The updated vector is stored in a private member 
   * variable of the Camera class.
   */
  void UpdateCameraVectors();

  void Initialized(glm::vec3 position, glm::vec3 world_up, glm::float32 yaw,
                   glm::float32 pitch, glm::float32 near_plane,
                   glm::float32 far_plane);

  /**
   * Convert glm::vec3 to a string.
   * This method returns a formatted string containing the values of the X, Y, 
   * and Z components of the glm::vec3 object. Returns a string of the format: 
   * "X: [x-value] Y: [y-value] Z: [z-value]", where [x-value], [y-value], 
   * and [z-value] Comma-separated values for the X, Y, and Z components of 
   * the glm::vec3 object.
   * @param other glm::vec3 object to transform.
   * @return Transformed string representation.
   */
  std::string GlmVec3ToString(glm::vec3 other) const;

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
  glm::float32 near_plane_;  // Near clipping plane distance
  glm::float32 far_plane_;   // Far clipping plane distance

  bool enabled_;
};

#endif  //CMAKE_OPEN_INCLUDES_INCLUDE_CAMERA_H_
