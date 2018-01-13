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

#ifndef _GAME_OBJECT_RENDER_TEST_H
#define _GAME_OBJECT_RENDER_TEST_H

/**********************
*   System Includes   *
***********************/
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Camera;
class GameApp;
class GameObject;
struct TimerParams;
class GameObjectManager;

/*****************
*   Class Decl   *
******************/
class GameObjectRenderTest sealed : public DrawableGameComponent
{
	private:
		//Variables
		GameObjectManager* m_GameObjectManager = nullptr;

		Camera* m_Camera = nullptr;

		void DrawGameObject(GameObject* gameObject);

	public:
		//Constructor Destructor.
		GameObjectRenderTest(GameApp* gameApp, const std::wstring& gameComponentName = L"Game Object Render Test");
		virtual ~GameObjectRenderTest();

		//Gets
		
		//Framework Methods

		//Drawable Game Component Override methods
		void					Initialize					() override;
		void					LoadContent					() override;
		void					Update						(const TimerParams& timerParams) override;
		void					Render						(const TimerParams& timerParams) override;
		void					OnLostDevice				() override;
		void					OnResetDevice				() override;
};

#endif