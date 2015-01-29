/********************************************************
*
* Author: Carlos Chac�n N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _AUDIO_SOUND_SOURCE_GOC_H
#define _AUDIO_SOUND_SOURCE_GOC_H

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
#include "AudioPlayer.h"
#include "GameObject\GameObjectDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class Audio;
class GameAsset;
class GameObject;
class AudioAsset;

/*****************
*   Class Decl   *
******************/

class AudioSourceGOC sealed : public GameObjectComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		AudioPlayer m_AudioPlayer;

		std::wstring m_Name = L"";

		/// <summary>
		/// Game Object Pair with Audio Sound that this Game Object Component uses
		/// </summary>
		GameObjectAssetPair<Audio> m_Audio;

		/// <summary>
		/// Is Ready flag that determines if the object 
		/// is initialize and ready to run.
		/// </summary>
		bool m_IsReady = false;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void CleanUp();

		void AudioAssetDeletion(GameAsset* asset);

		void AudioAssetReload(GameAsset* asset);

		void AudioAssetPreReload(GameAsset* asset);

		XEResult ClearAudioAsset(bool informAsset = true);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// AudioSourceGOC Constructor
		/// </summary>
		/// <param name="gameObject>Game Object that this Component is attached too</param>
		/// <param name="name">Name of the Audio.</param>
		AudioSourceGOC(GameObject* gameObject, const std::wstring& name);

		/// <summary>
		/// Default AudioSourceGOC Destructor
		/// </summary>
		virtual ~AudioSourceGOC();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline const std::wstring& GetName() const
		{
			return m_Name;
		}

		inline AudioPlayer* GetAudioPlayer()
		{
			return &m_AudioPlayer;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		inline void SetName(const std::wstring& name)
		{
			m_Name = name;
		}

		std::wstring GetAudioName() const;

		XEResult SetAudioAsset(AudioAsset* asset);

#pragma endregion
		
		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		XEResult RemoveAudioAsset();

#pragma endregion

};

#endif