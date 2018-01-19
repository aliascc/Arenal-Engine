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

#ifndef _GAME_ANIMATIONS_UPDATE_H
#define _GAME_ANIMATIONS_UPDATE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "AEGameComponentsDefs.h"
#include "GameUtils\GameComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameObject;
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class GameAnimationsUpdate sealed : public GameComponent
{
    private:

        void UpdateGameAnimationObjects(GameObject* gameObject, const TimerParams& timerParams);

    public:
        //Constructor Destructor.
        GameAnimationsUpdate(GameApp* gameApp, const std::wstring& gameComponentName = AE_GAME_ANIMATIONS_UPDATE_DEF_COMPONENT_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_AnimationsUpdate);

        virtual ~GameAnimationsUpdate();

        //Game Component Override methods
        void Update(const TimerParams& timerParams) override;
};

#endif
