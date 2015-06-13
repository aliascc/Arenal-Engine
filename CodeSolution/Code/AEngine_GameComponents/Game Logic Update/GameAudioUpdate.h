/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_AUDIO_UPDATE_H
#define _GAME_AUDIO_UPDATE_H

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

class GameAudioUpdate sealed : public GameComponent
{
	private:

		void UpdateGameAudio(GameObject* gameObject, const TimerParams& timerParams);

		void UpdateAudioListener(GameObject* gameObject, AudioListener* audioListener);

		void UpdateAudioSources(GameObject* gameObject);

	public:
		//Constructor Destructor.
		GameAudioUpdate(GameApp* gameApp, const std::wstring& gameComponentName = AE_GAME_AUDIO_UPDATE_DEF_COMPONENT_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GameAudioUpdate);

		virtual ~GameAudioUpdate();

		//Game Component Override methods
		void Update(const TimerParams& timerParams) override;
};

#endif