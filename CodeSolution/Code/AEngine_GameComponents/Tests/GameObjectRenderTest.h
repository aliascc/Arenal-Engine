/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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