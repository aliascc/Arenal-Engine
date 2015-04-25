/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AUDIO_LISTENER_GOC_H
#define _AUDIO_LISTENER_GOC_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameObject\GameObjectDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class AudioManager;
class AudioListener;

/*****************
*   Class Decl   *
******************/

class AudioListenerGOC sealed : public GameObjectComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		AudioManager* m_AudioManager = nullptr;

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
		/// AudioListenerGOC Constructor
		/// </summary>
		/// <param name="gameObject>Game Object that this Component is attached too</param>
		AudioListenerGOC(GameObject* gameObject, AudioManager* audioManager);

		/// <summary>
		/// Default MeshGOC Destructor
		/// </summary>
		virtual ~AudioListenerGOC();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion
		
		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif