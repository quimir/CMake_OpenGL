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

#include <gtest/gtest.h>
#include "include/Camera.h"

using namespace std;

class CameraTest : public ::testing::Test {
 protected:
  CameraTest() {
    // Create the Camera instance of the test case
    camera_ = new Camera(glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0, 0);
  }

  ~CameraTest() override { delete camera_; }

 protected:
  Camera* camera_;
};

TEST_F(CameraTest, CameraConstructor) {
  // Test the Camera constructor
  ASSERT_EQ(camera_->GetPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
  ASSERT_EQ(camera_->GetWorldUp(), glm::vec3(0.0f, 1.0f, 0.0f));
  ASSERT_EQ(camera_->GetYaw(), 0.0f);
  ASSERT_EQ(camera_->GetPitch(), 0.0f);
}

TEST_F(CameraTest, CameraViewMatrix) {
  // Test GetViewMatrix function
  glm::mat4 viewMatrix = camera_->GetViewMatrix();
  ASSERT_EQ(viewMatrix[3][0], 0.0f);
  ASSERT_EQ(viewMatrix[3][1], 0.0f);
  ASSERT_EQ(viewMatrix[3][2], -1.0f);
}

TEST_F(CameraTest, CameraProcessKeyboard) {
  // Test ProcessKeyboard function
  camera_->ProcessKeyboard(Camera::CameraMovement::kForward, 1.0f);
  ASSERT_EQ(camera_->GetPosition(), glm::vec3(0.0f, 0.0f, -1.0f));
}

TEST_F(CameraTest, CameraProcessMouseMovement) {
  // Test ProcessMouseMovement function
  camera_->ProcessMouseMovement(1.0f, 1.0f, true);
  ASSERT_NEAR(camera_->GetFront().x, 0.0f, 0.001f);
  ASSERT_NEAR(camera_->GetFront().y, 0.0f, 0.001f);
  ASSERT_NEAR(camera_->GetFront().z, -1.0f, 0.001f);

  ASSERT_NEAR(camera_->GetRight().x, 1.0f, 0.001f);
  ASSERT_NEAR(camera_->GetRight().y, 0.0f, 0.001f);
  ASSERT_NEAR(camera_->GetRight().z, 0.0f, 0.001f);

  ASSERT_NEAR(camera_->GetUp().x, 0.0f, 0.001f);
  ASSERT_NEAR(camera_->GetUp().y, 1.0f, 0.001f);
  ASSERT_NEAR(camera_->GetUp().z, 0.0f, 0.001f);
}

TEST_F(CameraTest, CameraProcessMouseScroll) {
  // Test ProcessMouseScroll
  camera_->ProcessMouseScroll(1.0f);
  ASSERT_EQ(camera_->GetZoom(), 1.0f);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}