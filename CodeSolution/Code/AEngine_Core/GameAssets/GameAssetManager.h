/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_ASSET_MANAGER_H
#define _GAME_ASSET_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <string>
#include <stdint.h>
#include <unordered_map>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameAssetDefs.h"
#include "Base\AEObject.h"
#include "GameContentDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameAsset;
class AEXMLParser;
class AEXMLWriter;
class AudioManager;
class RawGameAsset;
class GraphicDevice;
class AngelScriptManager;
class GameResourceManager;

/**************
*   Typedef   *
***************/
typedef std::unordered_map<uint64_t, RawGameAsset*> RawGameAssetMap;
typedef std::unordered_map<uint64_t, GameAsset*> GameAssetMap;

/*****************
*   Class Decl   *
******************/
class GameAssetManager sealed : public AEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		uint64_t m_UniqueAssetIDGen = AE_GAME_ASSET_UNIQUE_ID_USER_START;

		RawGameAssetMap m_RawGameAssetMap;

		GameAssetMap m_GameAssetMap;

		GraphicDevice* m_GraphicDevice = nullptr;

		GameResourceManager* m_GameResourceManager = nullptr;

		/// <summary>
		/// Angel Script Manager to handle the Scripts
		/// </summary>
		AngelScriptManager* m_AngelScriptManager = nullptr;

		AudioManager* m_AudioManager = nullptr;

		bool m_IsReady = false;

		std::wstring m_OutputDirAssets = L"";

		std::wstring m_ProjectDirectory = L"";

		GameAssetBuiltIns m_GameAssetBuiltIns;

		std::mutex m_UniqueIDGenerateMutex;

		std::mutex m_GameAssetManagerMutex;
		
#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void CleanUp();

		void GameAssetDeleted(uint64_t assetID);

		uint64_t GetNextUniqueAssetID();

		AEResult ImportBuiltInMeshes();

		AEResult ImportBuiltInShader();

		AEResult ImportBuiltInDiffuseTextureShader();

		AEResult ImportRawGameAssetWithoutLock(uint64_t rawAssetID);

		AEResult ImportModel(RawGameAsset* rawGA);

		AEResult ImportShader(RawGameAsset* rawGA);

		AEResult ImportTexture(RawGameAsset* rawGA);

		AEResult ImportGameObjectScript(RawGameAsset* rawGA);

		AEResult ImportAudio(RawGameAsset* rawGA);

		AEResult CheckForLatestRawGameAssets();

		AEResult SaveToXMLGameAssets(AEXMLWriter& xmlWriter) const;

		AEResult SaveToXMLRawGameAssets(AEXMLWriter& xmlWriter) const;

		AEResult LoadRawAssets(AEXMLParser& rawAssetXML);

		AEResult LoadGameAssets(AEXMLParser& gameAssetXML);

		void SetAssetDetails(const GameAssetLoadingDetails& details, GameAsset* asset);

		AEResult LoadModelAsset(const GameAssetLoadingDetails& details);

		AEResult LoadMeshAsset(const GameAssetLoadingDetails& details);

		AEResult LoadAnimationAsset(const GameAssetLoadingDetails& details);

		AEResult LoadSkeletonAsset(const GameAssetLoadingDetails& details);

		AEResult LoadShaderAsset(const GameAssetLoadingDetails& details);

		AEResult LoadTextureAsset(const GameAssetLoadingDetails& details);

		AEResult LoadGameObjectScriptAsset(const GameAssetLoadingDetails& details);

		AEResult LoadAudioAsset(const GameAssetLoadingDetails& details);

		RawGameAsset* GetRawGameAssetByGameAssetID(uint64_t gameAssetID);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameAssetManager Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to create assets with</param>
		/// <param name="gameResourceManager">Resource Manager to handle assets</param>
		/// <param name="angelScriptManager">Angel Script Manager to handle the Scripts</param>
		/// <param name="audioManager">Audio Manager to handle the Audio</param>
		/// <param name="outputDirAssets">Output Directory for Assets</param>
		/// <param name="projectDir">Project Directory</param>
		GameAssetManager(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, AngelScriptManager* angelScriptManager, AudioManager* audioManager, const std::wstring& projectDir, const std::wstring& outputDirAssets);

		/// <summary>
		/// Default GameAssetManager Destructor
		/// </summary>
		virtual ~GameAssetManager();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		GameAsset* GetGameAsset(uint64_t id);

		RawGameAsset* GetRawGameAsset(uint64_t id);

		inline const RawGameAssetMap& GetRawGameAssetMap() const
		{
			return m_RawGameAssetMap;
		}

		inline const GameAssetMap& GetGameAssetMap() const
		{
			return m_GameAssetMap;
		}

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

		inline bool GameAssetManager::RawGameAssetExists(uint64_t id) const
		{
			return (m_RawGameAssetMap.find(id) != m_RawGameAssetMap.end());
		}

		inline bool GameAssetManager::GameAssetExists(uint64_t id) const
		{
			return (m_GameAssetMap.find(id) != m_GameAssetMap.end());
		}

		AEResult Initialize();

		AEResult CreateNewRawGameAsset(const std::wstring& filePath, GameContentSubtype contentSubtype = GameContentSubtype::Unknown, uint64_t* rawAssetID = nullptr);

		AEResult ImportRawGameAsset(uint64_t rawAssetID);

		AEResult CheckForLatestRawGameAssetsAndImport();

		AEResult SaveToXML(const std::wstring& gameAssetsFilename) const;

		AEResult LoadAssetManagerFile(const std::wstring& gameAssetsFilename);

#pragma endregion

};

#endif