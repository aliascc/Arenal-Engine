/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

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
#include "XEGameComponentsDefs.h"
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
		GameAnimationsUpdate(GameApp* gameApp, const std::wstring& gameComponentName = XE_GAME_ANIMATIONS_UPDATE_DEF_COMPONENT_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_AnimationsUpdate);

		virtual ~GameAnimationsUpdate();

		//Game Component Override methods
		void Update(const TimerParams& timerParams) override;
};

#endif