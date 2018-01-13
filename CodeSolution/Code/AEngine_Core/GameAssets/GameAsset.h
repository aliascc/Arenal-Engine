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

#ifndef _GAME_ASSET_H
#define _GAME_ASSET_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <list>
#include <mutex>
#include <string>
#include <stdint.h>
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameAssetDefs.h"
#include "GameContentDefs.h"
#include "Base\UniqueAEObjectNamed.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class GameResourceManager;

/*****************
*   Class Decl   *
******************/
class GameAsset abstract : public UniqueAEObjectNamed
{
	friend class GameAssetManager;

	typedef std::map<uint64_t, GameAssetEventHandlers> GameAssetEventHandlersMap;

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Unique ID of this Asset
		/// </summary>
		/// <remarks>
		/// This ID will only be assigned by Game Asset Manager
		/// it is use so Raw Asset knows its imported asset
		/// </remarks>
		uint64_t m_UniqueAssetID = 0;

		/// <summary>
		/// Unique ID of the Parent Asset
		/// </summary>
		uint64_t m_ParentAssetID = 0;

		/// <summary>
		/// Game Asset Content Type
		/// </summary>
		GameContentType m_GameContentType = GameContentType::Unknown;

		/// <summary>
		/// Callback to notify Manager of Game Asset Deletion
		/// </summary>
		OnGameAssetDeletionNotifyManagerEvent m_OnGameAssetDeletionNotifyManagerEvent = nullptr;

		/// <summary>
		/// Event Handler to be called when Register Object is deleted
		/// </summary>
		OnListenerObjDeletionEvent m_OnListenerObjDeletionEventHandler = nullptr;

		/// <summary>
		/// Stores all Object's Event Handlers for when this Game Asset gets destroyed or reloaded
		/// </summary>
		GameAssetEventHandlersMap m_GameAssetEventHandlersMap;

		/// <summary>
		/// Flag that Lets know if the Game Asset has been loaded successfully or not
		/// </summary>
		bool m_IsLoaded = false;

		/// <summary>
		/// Defines if it is a built in asset or not
		/// </summary>
		bool m_IsBuiltInAsset = false;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void SetUniqueAssetID(uint64_t uniqueAssetID)
		{
			m_UniqueAssetID = uniqueAssetID;
		}

#pragma endregion

	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		/// <summary>
		/// File Path of the Game Asset
		/// </summary>
		std::wstring m_FilePath = L"";
		
		/// <summary>
		/// Mutex to be able to handle multiple request from different threads
		/// </summary>
		std::mutex m_GameAssetMutex;

		/// <summary>
		/// Resource Manager to handle the Asset Resource
		/// </summary>
		GameResourceManager* m_GameResourceManager = nullptr;

		/// <summary>
		/// Custom name of the Game Asset
		/// </summary>
		std::wstring m_CustomName = L"";

		/// <summary>
		/// Parent Game Asset
		/// </summary>
		GameAsset* m_ParentGameAsset = nullptr;

#pragma endregion

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Protected Methods
		
		/// <summary>
		/// Loads an Asset Resource to Memory
		/// </summary>
		/// <returns>AEResult::OK if successful</returns>
		virtual AEResult LoadAssetResource() = 0;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameAsset Constructor
		/// </summary>
		/// <param name="gameContentType">Asset Content Type</param>
		/// <param name="filePath">File Path of Game Asset to Load</param>
		/// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
		GameAsset(GameContentType gameContentType, const std::wstring& filePath, GameResourceManager* gameResourceManager);

		/// <summary>
		/// Default GameAsset Destructor
		/// </summary>
		virtual ~GameAsset();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// File Path of the Game Asset
		/// </summary>
		/// <returns>File path of Game Asset</returns>
		inline const std::wstring& GetFilePath() const
		{
			return m_FilePath;
		}

		/// <summary>
		/// Unique ID of this Asset
		/// </summary>
		/// <remarks>
		/// If 0 Game Asset Manager is not controlling in a direct way this Asset
		/// </remarks>
		inline uint64_t GetUniqueAssetID() const
		{
			return m_UniqueAssetID;
		}

		/// <summary>
		/// Gets the Game Content Type
		/// </summary>
		/// <returns>Game Content Type</returns>
		inline GameContentType GetGameContentType() const
		{
			return m_GameContentType;
		}

		/// <summary>
		/// Parent Asset ID
		/// </summary>
		/// <returns>ID of the Parent Asset, 0 if it has no parent</returns>
		inline uint64_t GetParentAssetID() const
		{
			return m_ParentAssetID;
		}

		/// <summary>
		/// Event Handler to be called when a Register Object is deleted
		/// </summary>
		/// <returns>Callback Function to Call when GOC gets deleted</returns>
		inline OnListenerObjDeletionEvent GetOnListenerObjDeletionEventHandler()
		{
			return m_OnListenerObjDeletionEventHandler;
		}

		/// <summary>
		/// Flag that Lets know if the Game Asset has been loaded successfully or not
		/// </summary>
		/// <returns>True if it was loaded, false otherwise</returns>
		inline bool IsLoaded() const
		{
			return m_IsLoaded;
		}

		/// <summary>
		/// Defines if this is a Game Asset or not
		/// </summary>
		inline bool IsBuiltInAsset() const
		{
			return m_IsBuiltInAsset;
		}

		/// <summary>
		/// Custom Name of the Game Asset
		/// </summary>
		/// <returns>Custom Name of Game Asset</returns>
		inline const std::wstring& GetCustomName() const
		{
			return m_CustomName;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods
		
		/// <summary>
		/// Sets the Parent Asset ID
		/// </summary>
		inline void SetParentAssetID(uint64_t parentAssetID)
		{
			m_ParentAssetID = parentAssetID;
		}

		/// <summary>
		/// Sets the File Path of the Game Asset
		/// </summary>
		inline void SetFilePath(const std::wstring& filePath)
		{
			m_FilePath = filePath;
		}

		/// <summary>
		/// Sets if this is a Game Asset or not
		/// </summary>
		inline void SetIsBuiltInAsset(bool isBuiltInAsset)
		{
			m_IsBuiltInAsset = isBuiltInAsset;
		}

		/// <summary>
		/// Sets the Custom Name of the Game Asset
		/// </summary>
		inline void SetCustomName(const std::wstring& customName)
		{
			m_CustomName = customName;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		AEResult RegisterEventHandlers(uint64_t id, OnGameAssetDeletionEvent onGameAssetDeletionEvent, OnGameAssetReloadEvent onGameAssetReloadEvent = nullptr, OnGameAssetPreReloadEvent onGameAssetPreReloadEvent = nullptr);

		AEResult UnregisterEventHandlers(uint64_t id);

		/// <summary>
		/// Loads an Asset Resource to Memory and notifies any Game Object that is register with this 
		/// asset about the loading
		/// </summary>
		/// <returns>AEResult::OK if successful</returns>
		AEResult LoadAsset();

#pragma endregion

};

#endif