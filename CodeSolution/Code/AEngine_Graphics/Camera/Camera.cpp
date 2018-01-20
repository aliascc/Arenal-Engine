/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Camera.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Camera::Camera(const std::wstring& name, const glm::vec3& position, const glm::vec3& target, const glm::vec3& UP, const glm::ivec2& dimensions, float fov, float znear, float zfar)
    : UniqueAEObjectNamed(name)
    , m_Position(position)
    , m_Target(target)
    , m_UPVector(UP)
    , m_ZNear(znear)
    , m_ZFar(zfar)
    , m_FoV(fov)
    , m_Dimensions(dimensions)
{
    //////////////////////////////////////////
    //Create View Matrix
    CreateView();

    //////////////////////////////////////////
    //Create Projection Matrix
    CreateProjection();

    //////////////////////////////////////////
    //Update Frustum
    m_Frustum.UpdateFrustum(m_ViewMatrix, m_ProjectionMatrix);
}

Camera::~Camera()
{
}

void Camera::CreateView()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_UPVector);
}

void Camera::CreateProjection()
{
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FoV), (float)m_Dimensions.x/(float)m_Dimensions.y, m_ZNear, m_ZFar);
}

AEResult Camera::Update(const TimerParams& timerParams)
{
    //////////////////////////////////////////
    //Update Frustum
    m_Frustum.UpdateFrustum(m_ViewMatrix, m_ProjectionMatrix);

    return AEResult::Ok;
}

void Camera::SetNewPositionAndRotation(const glm::vec3& position, const glm::vec3& rotation)
{
    ////////////////////////////////
    //Update Position
    m_Position = position;

    ////////////////////////////////
    //Update everything
    glm::mat4 rotationMatrix = AEMathHelpers::Mat4Identity;

    //Camera Yaw
    glm::mat4 rotationMatrixY = AEMathHelpers::CreateRotationY(rotation.y);

    //Camera Pitch
    glm::mat4 rotationMatrixX = AEMathHelpers::CreateRotationX(rotation.x);

    rotationMatrix = rotationMatrixY * rotationMatrixX;

    //Create vector pointing the direction the camera is facing
    glm::vec4 transformedReference = (rotationMatrix * glm::vec4(m_FwrVector, 1.0f));

    //Calculate the position the camera is looking at
    m_Target = m_Position + (glm::vec3)transformedReference;

    //Update View Matrix
    CreateView();
}

float Camera::GetAspectRatio() const
{
    if (m_Dimensions.y == 0)
    {
        return 0.0f;
    }

    return ((float)m_Dimensions.x / (float)m_Dimensions.y);
}

glm::mat4 Camera::GetWorldTransform() const
{
    return AEMathHelpers::CreateTransform(m_Position, m_Rotation, AEMathHelpers::Vec3fOne);
}

glm::vec3 Camera::GetDirectionVector() const
{
    glm::vec3 cameraDirection = AEMathHelpers::CalculateDirectionFromAngles(m_Rotation, m_FwrVector);

    return cameraDirection;
}
