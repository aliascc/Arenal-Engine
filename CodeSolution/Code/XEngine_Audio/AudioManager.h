/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AUDIO_MANAGER_H
#define _AUDIO_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class AudioManager sealed : public XEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Defines if Listener is set or not 
		/// to a Game Object
		/// </summary>
		bool m_IsListenerSet = false;

		std::mutex m_AudioManagerMutex;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default AudioManager Constructor
		/// </summary>
		AudioManager();

		/// <summary>
		/// Default AudioManager Destructor
		/// </summary>
		virtual ~AudioManager();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets if Listener is set to a Game Object
		/// </summary>
		inline bool IsListenerSet() const
		{
			return m_IsListenerSet;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		void SetIsListenerSet(bool isListenerSet)
		{
			m_IsListenerSet = isListenerSet;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif