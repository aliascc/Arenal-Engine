/********************************************************
*
* Author: Carlos Chacón N.
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
#include "Base\XEObject.h"
#include "GameContentDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameAsset;
class XEXMLParser;
class XEXMLWriter;
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
class GameAssetManager sealed : public XEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		uint64_t m_UniqueAssetIDGen = XE_GAME_ASSET_UNIQUE_ID_USER_START;

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

		XEResult ImportBuiltInMeshes();

		XEResult ImportBuiltInShader();

		XEResult ImportBuiltInDiffuseTextureShader();

		XEResult ImportRawGameAssetWithoutLock(uint64_t rawAssetID);

		XEResult ImportModel(RawGameAsset* rawGA);

		XEResult ImportShader(RawGameAsset* rawGA);

		XEResult ImportTexture(RawGameAsset* rawGA);

		XEResult ImportGameObjectScript(RawGameAsset* rawGA);

		XEResult ImportAudio(RawGameAsset* rawGA);

		XEResult CheckForLatestRawGameAssets();

		XEResult SaveToXMLGameAssets(XEXMLWriter& xmlWriter) const;

		XEResult SaveToXMLRawGameAssets(XEXMLWriter& xmlWriter) const;

		XEResult LoadRawAssets(XEXMLParser& rawAssetXML);

		XEResult LoadGameAssets(XEXMLParser& gameAssetXML);

		void SetAssetDetails(const GameAssetLoadingDetails& details, GameAsset* asset);

		XEResult LoadModelAsset(const GameAssetLoadingDetails& details);

		XEResult LoadMeshAsset(const GameAssetLoadingDetails& details);

		XEResult LoadAnimationAsset(const GameAssetLoadingDetails& details);

		XEResult LoadSkeletonAsset(const GameAssetLoadingDetails& details);

		XEResult LoadShaderAsset(const GameAssetLoadingDetails& details);

		XEResult LoadTextureAsset(const GameAssetLoadingDetails& details);

		XEResult LoadGameObjectScriptAsset(const GameAssetLoadingDetails& details);

		XEResult LoadAudioAsset(const GameAssetLoadingDetails& details);

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

		XEResult Initialize();

		XEResult CreateNewRawGameAsset(const std::wstring& filePath, GameContentSubtype contentSubtype = GameContentSubtype::Unknown, uint64_t* rawAssetID = nullptr);

		XEResult ImportRawGameAsset(uint64_t rawAssetID);

		XEResult CheckForLatestRawGameAssetsAndImport();

		XEResult SaveToXML(const std::wstring& gameAssetsFilename) const;

		XEResult LoadAssetManagerFile(const std::wstring& gameAssetsFilename);

#pragma endregion

};

#endif