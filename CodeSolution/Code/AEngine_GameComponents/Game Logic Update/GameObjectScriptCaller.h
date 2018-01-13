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

#ifndef _GAME_OBJECT_SCRIPT_CALLER_H
#define _GAME_OBJECT_SCRIPT_CALLER_H

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
class GameObjectScriptCaller sealed : public GameComponent
{
	enum class CallMethod : uint32_t
	{
		Initialize,
		ConstantUpdate,
		Update,
		PostUpdate
	};

	private:

		void ScriptCaller(GameObject* gameObject, CallMethod callMethod, const TimerParams& timerParams = TimerParams());

	public:
		//Constructor Destructor.
		GameObjectScriptCaller(GameApp* gameApp, const std::wstring& gameComponentName = AE_GAME_GAME_OBJECT_SCRIPT_CALLER_COMPONENT_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GameObjectScriptCaller);

		virtual ~GameObjectScriptCaller();

		//Game Component Override methods
		void Initialize() override;

		void ConstantUpdate(const TimerParams& timerParams) override;

		void Update(const TimerParams& timerParams) override;

		void PostUpdate(const TimerParams& timerParams) override;
};

#endif