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

#ifndef _AUDIO_PLAYER_H
#define _AUDIO_PLAYER_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "SFML\Audio.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioDefs.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class AudioPlayer sealed : public AEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		AudioPlayerState m_AudioPlayerState;

		bool m_IsSound = true;

		sf::SoundBuffer m_SoundBuffer;

		sf::SoundBuffer m_EmptySoundBuffer;

		sf::Sound m_Sound;

		sf::Music m_Music;

		const uint8_t* m_AudioData = nullptr;

		uint32_t m_AudioDataSize = 0;

		bool m_IsReady = false;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Private Constructor & Destructor Methods

		/// <summary>
		/// Default AudioPlayer Constructor
		/// </summary>
		AudioPlayer();

		/// <summary>
		/// Default AudioPlayer Destructor
		/// </summary>
		virtual ~AudioPlayer();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline const glm::vec3& GetPosition() const
		{
			return m_AudioPlayerState.m_Position;
		}

		inline float GetVolume() const
		{
			return m_AudioPlayerState.m_Volume;
		}

		inline float GetAttenuation() const
		{
			return m_AudioPlayerState.m_Attenuation;
		}

		inline float GetMinDistance() const
		{
			return m_AudioPlayerState.m_MinDistance;
		}

		inline float GetPitch() const
		{
			return m_AudioPlayerState.m_Pitch;
		}

		inline bool IsLooped() const
		{
			return m_AudioPlayerState.m_Loop;
		}

		inline bool Is3D() const
		{
			return m_AudioPlayerState.m_Is3D;
		}

		uint32_t GetChannelCount() const;

		uint32_t GetSampleRate() const;

		float GetDuration() const;

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		void SetPosition(const glm::vec3& pos);

		void SetVolume(float volume);

		void SetAttenuation(float attenuation);

		void SetMinDistance(float minDistance);

		void SetPitch(float pitch);

		void SetLoop(bool looped);

		void Set3D(bool is3d);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		AEResult Initialize(const uint8_t* data, uint32_t size);

		void StopAndUninitialize();

		AEResult SetAsSoundOrMusicProcessing(bool asSound);

		AEResult Play();

		AEResult Stop();

		AEResult Pause();

#pragma endregion

};

#endif