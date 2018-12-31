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

#ifndef _GAME_OBJECT_CAMERA_UPDATE_H
#define _GAME_OBJECT_CAMERA_UPDATE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AEGameComponentsDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameObject;
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class GameObjectCameraUpdate sealed : public GameComponent
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        GameObjectManager& m_GameObjectManager;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void UpdateCamera(GameObject* gameObject);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameObjectCameraUpdate Constructor
        /// </summary>
        GameObjectCameraUpdate(GameApp& gameApp, const std::string& gameComponentName = AE_GAME_GAME_OBJECT_CAMERA_UPDATE_COMPONENT_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GameObjectCameraUpdate);

        /// <summary>
        /// Default GameObjectCameraUpdate Destructor
        /// </summary>
        virtual ~GameObjectCameraUpdate();

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        void Update(const TimerParams& timerParams) override;

#pragma endregion

};

#endif
