/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_PHYSICS_UPDATE_H
#define _GAME_PHYSICS_UPDATE_H

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
class AudioListener;

/*****************
*   Class Decl   *
******************/

class GamePhysicsUpdate sealed : public GameComponent
{
	private:

	public:
		//Constructor Destructor.
		GamePhysicsUpdate(GameApp* gameApp, const std::wstring& gameComponentName = AE_GAME_PHYSICS_UPDATE_DEF_COMPONENT_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GamePhysicsUpdate);

		virtual ~GamePhysicsUpdate();

		//Game Component Override methods
		void Update(const TimerParams& timerParams) override;
};

#endif