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

#ifndef _GAME_OBJECT_CAMERA_H
#define _GAME_OBJECT_CAMERA_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Camera\Camera.h"
#include "Camera\CameraDefs.h"

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

class GameObjectCamera sealed : public Camera
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// CameraManager Constructor
        /// </summary>
        GameObjectCamera(const std::string& name, const glm::vec3& position = AEMathHelpers::Vec3fZero, const glm::vec3& target = AEMathHelpers::Vec3fZero, const glm::vec3& UP = AEMathHelpers::Vec3fUp, const glm::ivec2& dimensions = AEMathHelpers::Vec2iZero, float fov = 0.0f, float znear = 1.0f, float zfar = 1000.0f);

        /// <summary>
        /// Default CameraManager Destructor
        /// </summary>
        virtual ~GameObjectCamera();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight) override;

#pragma endregion

};

#endif
