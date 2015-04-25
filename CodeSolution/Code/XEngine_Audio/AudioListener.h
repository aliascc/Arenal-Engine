/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AUDIO_LISTENER_H
#define _AUDIO_LISTENER_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\Singleton.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class AudioListener sealed : public Singleton<AudioListener>
{
	friend class Singleton<AudioListener>;

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

#pragma endregion

		/***********************************************
		*   Private Constructor & Destructor Methods   *
		************************************************/
#pragma region Private Constructor & Destructor Methods

		/// <summary>
		/// Default AudioListener Constructor
		/// </summary>
		AudioListener();

		/// <summary>
		/// Default AudioListener Destructor
		/// </summary>
		virtual ~AudioListener();

#pragma endregion

	public:

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		glm::vec3 GetPosition() const;

		glm::vec3 GetDirection() const;

		float GetGlobalVolume() const;

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		void SetPosition(const glm::vec3& pos);

		void SetGlobalVolume(float volume);

		void SetDirection(const glm::vec3& direction);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif