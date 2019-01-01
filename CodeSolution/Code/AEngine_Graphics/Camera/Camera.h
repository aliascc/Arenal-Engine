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

#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\Frustum.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class Camera abstract : public Named
{
    protected:
        //Variables
        glm::mat4 m_ViewMatrix = AEMathHelpers::Mat4Identity;
        glm::mat4 m_ProjectionMatrix = AEMathHelpers::Mat4Identity;
        glm::vec3 m_Position = AEMathHelpers::Vec3fZero;
        glm::vec3 m_Target = AEMathHelpers::Vec3fZero;
        glm::vec3 m_UPVector = AEMathHelpers::Vec3fUp;
        glm::ivec2 m_Dimensions = AEMathHelpers::Vec2iZero;

        glm::vec3 m_Rotation = AEMathHelpers::Vec3fZero;

        glm::vec3 m_FwrVector = AEMathHelpers::Vec3fFwrZP;

        float m_ZNear = 0.0f;
        float m_ZFar = 0.0f;
        float m_FoV = 0.0f;

        Frustum m_Frustum;

        //Private Methods
        void CreateView();
        void CreateProjection();

    public:
        //Constructor Destructor.
        Camera(const std::string& name, const glm::vec3& position = AEMathHelpers::Vec3fZero, const glm::vec3& target = AEMathHelpers::Vec3fZero, const glm::vec3& UP = AEMathHelpers::Vec3fUp, const glm::ivec2& dimensions = AEMathHelpers::Vec2iZero, float fov = 0.0f, float znear = 1.0f, float zfar = 1000.0f);

        virtual ~Camera();

        //Gets
        inline const glm::mat4&    GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

        inline const glm::mat4& GetViewMatrix() const
        {
            return m_ViewMatrix;
        }

        inline const glm::vec3& GetPosition() const
        {
            return m_Position;
        }

        inline const glm::ivec2& GetDimensions() const
        {
            return m_Dimensions;
        }

        inline float GetZNear() const
        {
            return m_ZNear;
        }

        inline float GetZFar() const
        {
            return m_ZFar;
        }

        inline const Frustum& GetFrustum() const
        {
            return m_Frustum;
        }

        inline const glm::vec3& GetUpVector() const
        {
            return m_UPVector;
        }

        float GetAspectRatio() const;

        glm::vec3 GetDirectionVector() const;

        glm::mat4 GetWorldTransform() const;

        //Framework Methods
        void SetNewPositionAndRotation(const glm::vec3& position, const glm::vec3& rotation);

        virtual AEResult Update(const TimerParams& timerParams);

        virtual AEResult ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight) = 0;
};

#endif
