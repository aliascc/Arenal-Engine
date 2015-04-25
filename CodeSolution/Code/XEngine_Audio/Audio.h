/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AUDIO_H
#define _AUDIO_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Resource\GameResource.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class Audio sealed : public GameResource
{
	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Private Variables

		/// <summary>
		/// Audio Data Loaded into memory
		/// </summary>
		uint8_t* m_AudioData = nullptr;

		/// <summary>
		/// Size of Audio Data
		/// </summary>
		uint32_t m_AudioDataSize = 0;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default Audio Constructor
		/// </summary>
		/// <param name="audioName">Audio Name</param>
		Audio(const std::wstring& audioName);

		/// <summary>
		/// Default Audio Destructor
		/// </summary>
		virtual ~Audio();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline const uint8_t* GetAudioData() const
		{
			return m_AudioData;
		}

		inline const uint32_t GetAudioDataSize() const
		{
			return m_AudioDataSize;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion

		/********************************
		*   Virtual Framework Methods   *
		*********************************/
#pragma region Virtual Framework Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		XEResult LoadFile(const std::wstring& file);

		XEResult Load() override;

#pragma endregion

};

#endif