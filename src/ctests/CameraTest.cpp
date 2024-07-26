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
#include "include/LoggerSystem.h"

using namespace std;

class CameraTest : public ::testing::Test {
 protected:
  Camera* camera;

  void SetUp() override { camera = new Camera(); }

  void TearDown() override { delete camera; }
};

// Test default constructor
TEST_F(CameraTest, DefaultConstructor) {
  EXPECT_EQ(camera->GetPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
  EXPECT_EQ(camera->GetWorldUp(), glm::vec3(0.0f, 1.0f, 0.0f));
  EXPECT_FLOAT_EQ(camera->GetYaw(), cameraconstsetting::kYaw);
  EXPECT_FLOAT_EQ(camera->GetPitch(), cameraconstsetting::kPitch);
  EXPECT_FLOAT_EQ(camera->GetNearPlane(), cameraconstsetting::kNear);
  EXPECT_FLOAT_EQ(camera->GetFarPlane(), cameraconstsetting::kFar);
}

// Test parameterized constructor
TEST_F(CameraTest, ParameterizedConstructor) {
  Camera cam(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0f,
             45.0f, 0.1f, 100.0f);
  EXPECT_EQ(cam.GetPosition(), glm::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(cam.GetWorldUp(), glm::vec3(0.0f, 1.0f, 0.0f));
  EXPECT_FLOAT_EQ(cam.GetYaw(), -45.0f);
  EXPECT_FLOAT_EQ(cam.GetPitch(), 45.0f);
  EXPECT_FLOAT_EQ(cam.GetNearPlane(), 0.1f);
  EXPECT_FLOAT_EQ(cam.GetFarPlane(), 100.0f);
}

// Test GetViewMatrix
TEST_F(CameraTest, GetViewMatrix) {
  glm::mat4 viewMatrix = camera->GetViewMatrix();
  glm::mat4 expectedMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      EXPECT_NEAR(viewMatrix[i][j], expectedMatrix[i][j], 1e-5);
    }
  }
}

// Test ProcessKeyboard
TEST_F(CameraTest, ProcessKeyboard) {
  glm::vec3 initialPosition = camera->GetPosition();
  camera->ProcessKeyboard(Camera::CameraMovement::kForward, 1.0);
  glm::vec3 newPosition = camera->GetPosition();
  EXPECT_NE(initialPosition, newPosition);
}

// Test ProcessMouseMovement
TEST_F(CameraTest, ProcessMouseMovement) {
  float initialYaw = camera->GetYaw();
  float initialPitch = camera->GetPitch();
  camera->ProcessMouseMovement(10.0f, 5.0f);
  EXPECT_NE(camera->GetYaw(), initialYaw);
  EXPECT_NE(camera->GetPitch(), initialPitch);
}

// Test ProcessMouseScroll
TEST_F(CameraTest, ProcessMouseScroll) {
  float initialZoom = camera->GetZoom();
  camera->ProcessMouseScroll(-1.0f);
  if (initialZoom >= 45.0f) {
    EXPECT_EQ(camera->GetZoom(), 45.0f);
  } else {
    EXPECT_LT(camera->GetZoom(), initialZoom);
  }

  camera->ProcessMouseScroll(1.0f);
  EXPECT_GT(camera->GetZoom(), camera->GetZoom() - 1.0f);
}

// Test getters and setters
TEST_F(CameraTest, GettersAndSetters) {
  camera->SetPosition(glm::vec3(5.0f, 5.0f, 5.0f));
  EXPECT_EQ(camera->GetPosition(), glm::vec3(5.0f, 5.0f, 5.0f));

  camera->SetYaw(90.0f);
  EXPECT_FLOAT_EQ(camera->GetYaw(), 90.0f);

  camera->SetPitch(45.0f);
  EXPECT_FLOAT_EQ(camera->GetPitch(), 45.0f);

  camera->SetMovementSpeed(10.0f);
  EXPECT_FLOAT_EQ(camera->GetMovementSpeed(), 10.0f);

  camera->SetMouseSensitivity(0.5f);
  EXPECT_FLOAT_EQ(camera->GetMouseSensitivity(), 0.5f);

  camera->SetZoom(30.0f);
  EXPECT_FLOAT_EQ(camera->GetZoom(), 30.0f);

  camera->SetNearPlane(0.5f);
  EXPECT_FLOAT_EQ(camera->GetNearPlane(), 0.5f);

  camera->SetFarPlane(200.0f);
  EXPECT_FLOAT_EQ(camera->GetFarPlane(), 200.0f);
}

// Test Enable and Disable
TEST_F(CameraTest, EnableAndDisable) {
  EXPECT_TRUE(camera->IsEnabled());
  camera->DisEnable();
  EXPECT_FALSE(camera->IsEnabled());
  camera->Enable();
  EXPECT_TRUE(camera->IsEnabled());
}

// Test ResetCamera
TEST_F(CameraTest, ResetCamera) {
  camera->SetPosition(glm::vec3(5.0f, 5.0f, 5.0f));
  camera->ResetCamera();
  EXPECT_EQ(camera->GetPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}