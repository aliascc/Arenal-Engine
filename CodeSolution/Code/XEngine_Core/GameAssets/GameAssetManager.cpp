/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/
#include "boost\filesystem.hpp"
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioDefs.h"
#include "GameAsset.h"
#include "AudioManager.h"
#include "RawGameAsset.h"
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Time\XETimeDefs.h"
#include "XML\XEXMLWriter.h"
#include "XML\XEXMLParser.h"
#include "GameAssetManager.h"
#include "Assets\MeshAsset.h"
#include "Assets\ModelAsset.h"
#include "Assets\AudioAsset.h"
#include "Assets\ShaderAsset.h"
#include "Base\BaseFunctions.h"
#include "Writers\WriterXE3D.h"
#include "Writers\WriterHLSL.h"
#include "Assets\TextureAsset.h"
#include "Assets\SkeletonAsset.h"
#include "Content\ModelContent.h"
#include "Assets\AnimationAsset.h"
#include "Importers\ImporterHLSL.h"
#include "Models\Custom\CubeMesh.h"
#include "Importers\ImporterAssimp.h"
#include "Models\Custom\IcoSphereMesh.h"
#include "Assets\GameObjectScriptAsset.h"
#include "Resource\GameResourceManager.h"
#include "AngelScript\AngelScriptManager.h"
#include "Shaders\Custom\DiffuseTexturePS.h"
#include "Shaders\Custom\DiffuseTextureVS.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

XETODO("Check for Mutex");
XETODO("When importing need to handle change of directory");
XETODO("When importing need to handle change of sub content type");
GameAssetManager::GameAssetManager(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, AngelScriptManager* angelScriptManager, AudioManager* audioManager, const std::wstring& projectDir, const std::wstring& outputDirAssets)
	: m_GraphicDevice(graphicDevice)
	, m_GameResourceManager(gameResourceManager)
	, m_AngelScriptManager(angelScriptManager)
	, m_AudioManager(audioManager)
	, m_ProjectDirectory(projectDir)
	, m_OutputDirAssets(outputDirAssets)
{
	/////////////////////////////
	//Verify we have not reach the max IDs
	static_assert((uint32_t)GameAssetDefaultIDs::__Max_GameAssetDefaultIDs < XE_GAME_ASSET_UNIQUE_ID_USER_START, "Game Asset Default IDs overpass custom start ID");

	/////////////////////////////
	//Verify objects are not null
	XEAssert(m_GraphicDevice != nullptr);
	XEAssert(m_GameResourceManager != nullptr);
	XEAssert(m_AngelScriptManager != nullptr);
	XEAssert(m_AudioManager != nullptr);
}

GameAssetManager::~GameAssetManager()
{
	CleanUp();
}

void GameAssetManager::CleanUp()
{
	for(auto rawGAIT : m_RawGameAssetMap)
	{
		DeleteMem(rawGAIT.second);
	}
	m_RawGameAssetMap.clear();

	////////////////////////////////////////////////////
	// - Set to null all child assets as Parent will be in charge of deletion
	// - Set null to all callbacks, so we won't have notifications of asset deletion
	for (auto gameAssetPair : m_GameAssetMap)
	{
		GameAsset* gameAsset = gameAssetPair.second;

		gameAsset->m_OnGameAssetDeletionNotifyManagerEvent = nullptr;
		if(gameAsset->GetParentAssetID() != 0)
		{
			gameAssetPair.second = nullptr;
		}
	}
	
	////////////////////////////////////////////////////
	// - Delete remaining Assets
	for(auto gaPair : m_GameAssetMap)
	{
		DeleteMem(gaPair.second);
	}
	m_GameAssetMap.clear();
}

void GameAssetManager::GameAssetDeleted(uint64_t assetID)
{
	auto gameAssetIT = m_GameAssetMap.find(assetID);

	if(gameAssetIT == m_GameAssetMap.end())
	{
		return;
	}

	m_GameAssetMap.erase(gameAssetIT);
}

uint64_t GameAssetManager::GetNextUniqueAssetID()
{
	std::lock_guard<std::mutex> lock(m_UniqueIDGenerateMutex);

	return m_UniqueAssetIDGen++;
}

RawGameAsset* GameAssetManager::GetRawGameAsset(uint64_t id)
{
	if(!m_IsReady)
	{
		return nullptr;
	}

	std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);
	
	if(!RawGameAssetExists(id))
	{
		return nullptr;
	}

	return m_RawGameAssetMap[id];
}

GameAsset* GameAssetManager::GetGameAsset(uint64_t id)
{
	if(!m_IsReady)
	{
		return nullptr;
	}

	std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);
	
	if(!GameAssetExists(id))
	{
		return nullptr;
	}

	return m_GameAssetMap[id];
}

XEResult GameAssetManager::Initialize()
{
	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(m_GameResourceManager != nullptr);
	if(m_GameResourceManager == nullptr)
	{
		return XEResult::GameResourceManagerNull;
	}

	XEAssert(m_AngelScriptManager != nullptr);
	if (m_AngelScriptManager == nullptr)
	{
		XETODO("Add error for AS null");
		return XEResult::NullObj;
	}

	XETODO("Check return");
	ImportBuiltInMeshes();

	XETODO("Check return");
	ImportBuiltInShader();

	XETODO("Read file to load assets and unique id gen sequence");

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportBuiltInMeshes()
{
	XEResult ret = XEResult::Ok;

	////////////////////////////////////////
	//Import Cube Mesh

	if (m_GameAssetBuiltIns.m_CubeMesh == nullptr)
	{
		m_GameAssetBuiltIns.m_CubeMesh = new CubeMesh(m_GraphicDevice, XE_CUBE_MESH_RESOURCE_NAME);

		ret = m_GameResourceManager->ManageGameResource(m_GameAssetBuiltIns.m_CubeMesh, XE_CUBE_MESH_FAKE_FILE_PATH);
		if (ret != XEResult::Ok)
		{
			XETODO("Add log");
			XERelease(m_GameAssetBuiltIns.m_CubeMesh);

			return XEResult::ResourceManagedFailed;
		}
	}

	if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::CubeMesh) == m_GameAssetMap.end())
	{
		MeshAsset* meshAsset = new MeshAsset(XE_CUBE_MESH_FAKE_FILE_PATH, m_GameResourceManager, m_GraphicDevice);

		meshAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::CubeMesh);
		meshAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::CubeMesh] = meshAsset;

		meshAsset->SetName(XE_CUBE_MESH_NAME);
		meshAsset->SetIsBuiltInAsset(true);

		XETODO("Check return");
		meshAsset->LoadAsset();
	}

	////////////////////////////////////////
	//Import Sphere Mesh

	if (m_GameAssetBuiltIns.m_IcoSphereMesh == nullptr)
	{
		m_GameAssetBuiltIns.m_IcoSphereMesh = new IcoSphereMesh(m_GraphicDevice, XE_ICO_SPHERE_MESH_RESOURCE_NAME);

		ret = m_GameResourceManager->ManageGameResource(m_GameAssetBuiltIns.m_IcoSphereMesh, XE_ICO_SPHERE_MESH_FAKE_FILE_PATH);
		if (ret != XEResult::Ok)
		{
			XETODO("Add log");
			XERelease(m_GameAssetBuiltIns.m_IcoSphereMesh);

			return XEResult::ResourceManagedFailed;
		}
	}

	if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::SphereMesh) == m_GameAssetMap.end())
	{
		MeshAsset* meshAsset = new MeshAsset(XE_ICO_SPHERE_MESH_FAKE_FILE_PATH, m_GameResourceManager, m_GraphicDevice);

		meshAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::SphereMesh);
		meshAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::SphereMesh] = meshAsset;

		meshAsset->SetName(XE_ICO_SPHERE_MESH_NAME);
		meshAsset->SetIsBuiltInAsset(true);

		XETODO("Check return");
		meshAsset->LoadAsset();
	}

	////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult GameAssetManager::ImportBuiltInShader()
{
	if (ImportBuiltInDiffuseTextureShader() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportBuiltInDiffuseTextureShader()
{
	XEResult ret = XEResult::Ok;

	///////////////////////////
	//Import Diffuse Texture VS

	if (m_GameAssetBuiltIns.m_DiffuseTextureVS == nullptr)
	{
		m_GameAssetBuiltIns.m_DiffuseTextureVS = new DiffuseTextureVS(m_GraphicDevice, XE_DIFFUSE_TEXTURE_VS_NAME);

		ret = m_GameResourceManager->ManageGameResource(m_GameAssetBuiltIns.m_DiffuseTextureVS, XE_DIFFUSE_TEXTURE_VS_FAKE_FILE_PATH);
		if (ret != XEResult::Ok)
		{
			XETODO("Add log");
			XERelease(m_GameAssetBuiltIns.m_DiffuseTextureVS);

			return XEResult::ResourceManagedFailed;
		}
	}

	if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::DiffuseTextureVS) == m_GameAssetMap.end())
	{
		ShaderAsset* shaderAsset = new ShaderAsset(XE_DIFFUSE_TEXTURE_VS_FAKE_FILE_PATH, m_GameResourceManager, ShaderType::VertexShader, m_GraphicDevice);

		shaderAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::DiffuseTextureVS);
		shaderAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::DiffuseTextureVS] = shaderAsset;

		shaderAsset->SetName(XE_DIFFUSE_TEXTURE_VS_NAME);
		shaderAsset->SetIsBuiltInAsset(true);

		XETODO("Check return");
		shaderAsset->LoadAsset();
	}

	///////////////////////////
	//Import Diffuse Texture PS

	if (m_GameAssetBuiltIns.m_DiffuseTexturePS == nullptr)
	{
		m_GameAssetBuiltIns.m_DiffuseTexturePS = new DiffuseTexturePS(m_GraphicDevice, XE_DIFFUSE_TEXTURE_PS_NAME);

		ret = m_GameResourceManager->ManageGameResource(m_GameAssetBuiltIns.m_DiffuseTexturePS, XE_DIFFUSE_TEXTURE_PS_FAKE_FILE_PATH);
		if (ret != XEResult::Ok)
		{
			XETODO("Add log");
			XERelease(m_GameAssetBuiltIns.m_DiffuseTexturePS);

			return XEResult::ResourceManagedFailed;
		}
	}

	if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::DiffuseTexturePS) == m_GameAssetMap.end())
	{
		ShaderAsset* shaderAsset = new ShaderAsset(XE_DIFFUSE_TEXTURE_PS_FAKE_FILE_PATH, m_GameResourceManager, ShaderType::PixelShader, m_GraphicDevice);

		shaderAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::DiffuseTexturePS);
		shaderAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::DiffuseTexturePS] = shaderAsset;

		shaderAsset->SetName(XE_DIFFUSE_TEXTURE_PS_NAME);
		shaderAsset->SetIsBuiltInAsset(true);

		XETODO("Check return");
		shaderAsset->LoadAsset();
	}

	///////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult GameAssetManager::CreateNewRawGameAsset(const std::wstring& filePath, GameContentSubtype contentSubtype, uint64_t* rawAssetID)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(!filePath.empty());
	if(filePath.empty())
	{
		return XEResult::EmptyFilename;
	}

	if(!boost::filesystem::exists(filePath))
	{
		return XEResult::FileNotPresent;
	}

	std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

	XETODO("Copy raw file to raw directory if it is not present currently there");
	RawGameAsset* rawGA = new RawGameAsset(XE_Base::GetRelativePath(filePath), m_ProjectDirectory, XE_Base::GetFilenameOnly(filePath));

	rawGA->SetOutputDirectory(m_OutputDirAssets);

	rawGA->SetContentSubtype(contentSubtype);

	XEResult ret = rawGA->ParseRawGameAssetFile();
	if(ret != XEResult::Ok)
	{
		DeleteMem(rawGA);

		return ret;
	}

	m_RawGameAssetMap[rawGA->GetUniqueID()] = rawGA;

	if(rawAssetID != nullptr)
	{
		*rawAssetID = rawGA->GetUniqueID();
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::CheckForLatestRawGameAssets()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	for(auto rawGAIT : m_RawGameAssetMap)
	{
		RawGameAsset* rawGA = rawGAIT.second;

		if(rawGA->CheckIfLatest() != XEResult::Ok)
		{
			XETODO("Add log here");
		}
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::CheckForLatestRawGameAssetsAndImport()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}
	
	std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

	XEResult ret = CheckForLatestRawGameAssets();

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	for(auto rawGAIT : m_RawGameAssetMap)
	{
		RawGameAsset* rawGA = rawGAIT.second;

		if(rawGA->IsReloadNeeded())
		{
			if(ImportRawGameAssetWithoutLock(rawGA->GetUniqueID()) != XEResult::Ok)
			{
				XETODO("add warning");
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportRawGameAsset(uint64_t rawAssetID)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

	return ImportRawGameAssetWithoutLock(rawAssetID);
}

XEResult GameAssetManager::ImportRawGameAssetWithoutLock(uint64_t rawAssetID)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (!RawGameAssetExists(rawAssetID))
	{
		return XEResult::NotFound;
	}

	XEResult ret = XEResult::Ok;

	XETODO("Lock game loop when loading raw asset");
	RawGameAsset* rawGA = m_RawGameAssetMap[rawAssetID];

	ret = rawGA->CheckIfLatest();
	if(ret != XEResult::Ok)
	{
		XETODO("set better return code");
		return XEResult::Fail;
	}

	if(!rawGA->IsReloadNeeded())
	{
		return XEResult::Ok;
	}

	switch (rawGA->GetContentType())
	{
		case GameContentType::Model:
			ret = ImportModel(rawGA);
			break;

		case GameContentType::Shader:
			ret = ImportShader(rawGA);
			break;

		case GameContentType::Texture:
			ret = ImportTexture(rawGA);
			break;

		case GameContentType::GameObjectScript:
			ret = ImportGameObjectScript(rawGA);
			break;

		case GameContentType::Audio:
			ret = ImportAudio(rawGA);
			break;

		default:
			XETODO("Add Log");
			ret = XEResult::InvalidObjType;
			break;
	}

	if(ret == XEResult::Ok)
	{
		rawGA->ImportDone();
	}

	return ret;
}

XEResult GameAssetManager::ImportModel(RawGameAsset* rawGA, const ModelAssetPreloadedIDs* modelAssetIDs)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(rawGA != nullptr);
	if(rawGA == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (modelAssetIDs != nullptr)
	{
		if (modelAssetIDs->m_ModelAssetID == 0 || modelAssetIDs->m_SkeletonAssetID == 0)
		{
			return XEResult::GameAssetInvalidID;
		}

		if (GameAssetExists(modelAssetIDs->m_ModelAssetID) || GameAssetExists(modelAssetIDs->m_SkeletonAssetID))
		{
			return XEResult::GameAssetIDInUse;
		}

		for (auto gaIt : modelAssetIDs->m_MeshAssetIDs)
		{
			if (gaIt.second == 0)
			{
				return XEResult::GameAssetInvalidID;
			}

			if (GameAssetExists(gaIt.second))
			{
				return XEResult::GameAssetIDInUse;
			}
		}

		for (auto gaIt : modelAssetIDs->m_AnimAssetIDs)
		{
			if (gaIt.second == 0)
			{
				return XEResult::GameAssetInvalidID;
			}

			if (GameAssetExists(gaIt.second))
			{
				return XEResult::GameAssetIDInUse;
			}
		}
	}

	/////////////////////////////////////////////////////////////
	//Import Model from Raw File
	XEResult ret = XEResult::Ok;
	ModelContent* modelContent = nullptr;

	ImporterAssimp importAssimp;

	ret = importAssimp.ImportModel(rawGA->GetFilePath(), &modelContent);
	if(ret != XEResult::Ok)
	{
		XETODO("Add Log");

		return XEResult::ImportFail;
	}
	
	/////////////////////////////////////////////////////////////
	//Write Imported Model to a File the assets can read
	WriterXE3D writerXE3D;
	ret = writerXE3D.WriteToFile(modelContent, rawGA->GetOutputFileName(), rawGA->GetFullOutputDirectory());

	DeleteMem(modelContent);

	if(ret != XEResult::Ok)
	{
		XETODO("Add Log");

		return XEResult::ImportFail;
	}
	
	/////////////////////////////////////////////////////////////
	//Create Asset if needed and load new file
	bool isNew = false;
	ModelAsset* modelAsset = nullptr;
	
	if(GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
	{
		modelAsset = reinterpret_cast<ModelAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
	}
	else
	{
		isNew = true;

		modelAsset = new ModelAsset(writerXE3D.GetOutputFilePath(), m_GameResourceManager, m_GraphicDevice);

		if (modelAssetIDs != nullptr)
		{
			modelAsset->SetUniqueAssetID(modelAssetIDs->m_ModelAssetID);
		}
		else
		{
			modelAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
		}
	}

	ret = modelAsset->LoadAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("add log");

		if(isNew)
		{
			DeleteMem(modelAsset);
		}

		return XEResult::ImportFail;
	}

	if(isNew)
	{
		modelAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[modelAsset->GetUniqueAssetID()] = modelAsset;

		rawGA->SetUniqueAssociatedAssetID(modelAsset->GetUniqueAssetID());
	}

	/////////////////////////////////////////////////////////////
	//Set IDs to Assets that have been recently imported
	//

	MeshAssetMap& meshAssetMap = modelAsset->GetMeshAssetMap();
	for (auto gaIt : meshAssetMap)
	{
		MeshAsset* meshAsset = gaIt.second.m_Asset;
		if (meshAsset->GetUniqueAssetID() == 0)
		{
			if (modelAssetIDs != nullptr)
			{
				auto idIt = modelAssetIDs->m_MeshAssetIDs.find(meshAsset->GetName());
				if (idIt != modelAssetIDs->m_MeshAssetIDs.end())
				{
					meshAsset->SetUniqueAssetID(idIt->second);
				}
				else
				{
					XETODO("Log warning of gameasset not found in preloaded map");
					meshAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
				}
			}
			else
			{
				meshAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
			}

			meshAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

			m_GameAssetMap[meshAsset->GetUniqueAssetID()] = meshAsset;
		}
	}

	AnimationAssetMap& animAssetMap = modelAsset->GetAnimationAssetMap();
	for (auto gaIt : animAssetMap)
	{
		AnimationAsset* animAsset = gaIt.second.m_Asset;
		if (animAsset->GetUniqueAssetID() == 0)
		{
			if (modelAssetIDs != nullptr)
			{
				auto idIt = modelAssetIDs->m_AnimAssetIDs.find(animAsset->GetName());
				if (idIt != modelAssetIDs->m_AnimAssetIDs.end())
				{
					animAsset->SetUniqueAssetID(idIt->second);
				}
				else
				{
					XETODO("Log warning of gameasset not found in preloaded map");
					animAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
				}
			}
			else
			{
				animAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
			}

			animAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

			m_GameAssetMap[animAsset->GetUniqueAssetID()] = animAsset;
		}
	}

	SkeletonAsset* skeletonAsset = modelAsset->GetSkeletonAsset();
	if(skeletonAsset != nullptr && skeletonAsset->GetUniqueAssetID() == 0)
	{
		if (modelAssetIDs != nullptr)
		{
			skeletonAsset->SetUniqueAssetID(modelAssetIDs->m_SkeletonAssetID);
		}
		else
		{
			skeletonAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
		}

		skeletonAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[skeletonAsset->GetUniqueAssetID()] = skeletonAsset;
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportShader(RawGameAsset* rawGA, uint64_t preloadGameAssetID)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(rawGA != nullptr);
	if(rawGA == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (preloadGameAssetID != 0 && GameAssetExists(preloadGameAssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	/////////////////////////////////////////////////////////////
	//Import Shader from Raw File
	ImporterHLSL importerHLSL;

	ShaderType shaderType = ShaderType::VertexShader;

	switch (rawGA->GetContentSubtype())
	{
		case GameContentSubtype::VertexShaderHLSL:
			shaderType = ShaderType::VertexShader;
			break;

		case GameContentSubtype::PixelShaderHLSL:
			shaderType = ShaderType::PixelShader;
			break;

		case GameContentSubtype::GeometryShaderHLSL:
			shaderType = ShaderType::GeometryShader;
			break;

		case GameContentSubtype::ComputeShaderHLSL:
			shaderType = ShaderType::ComputeShader;
			break;

		case GameContentSubtype::HullShaderHLSL:
			shaderType = ShaderType::HullShader;
			break;

		case GameContentSubtype::DomainShaderHLSL:
			shaderType = ShaderType::DomainShader;
			break;

		default:
			return XEResult::InvalidShaderType;
			break;
	}

	ShaderContent* shaderContent = nullptr;

	XETODO("make this an option");
	ShaderModel shaderModel = ShaderModel::SM_5_0;

	XEResult ret = importerHLSL.ImportShader(rawGA->GetFilePath(), shaderType, shaderModel, &shaderContent);
	if(ret != XEResult::Ok)
	{
		XETODO("Add Log");

		return XEResult::ImportFail;
	}
	
	/////////////////////////////////////////////////////////////
	//Write Imported Shader to a File the assets can read
	WriterHLSL writerHLSL;
	ret = writerHLSL.WriteToFile(shaderContent, rawGA->GetOutputFileName(), rawGA->GetFullOutputDirectory());

	DeleteMem(shaderContent);

	if(ret != XEResult::Ok)
	{
		XETODO("Add Log");

		return XEResult::ImportFail;
	}
	
	/////////////////////////////////////////////////////////////
	//Create Asset if needed and load new file
	bool isNew = false;
	ShaderAsset* shaderAsset = nullptr;
	
	if(GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
	{
		shaderAsset = reinterpret_cast<ShaderAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
	}
	else
	{
		isNew = true;

		shaderAsset = new ShaderAsset(writerHLSL.GetOutputFilePath(), m_GameResourceManager, shaderType, m_GraphicDevice);

		if (preloadGameAssetID != 0)
		{
			shaderAsset->SetUniqueAssetID(preloadGameAssetID);
		}
		else
		{
			shaderAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
		}
	}

	ret = shaderAsset->LoadAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("add log");

		if(isNew)
		{
			DeleteMem(shaderAsset);
		}

		return XEResult::ImportFail;
	}

	if(isNew)
	{
		shaderAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[shaderAsset->GetUniqueAssetID()] = shaderAsset;

		rawGA->SetUniqueAssociatedAssetID(shaderAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportTexture(RawGameAsset* rawGA, uint64_t preloadGameAssetID)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(rawGA != nullptr);
	if(rawGA == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (preloadGameAssetID != 0 && GameAssetExists(preloadGameAssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////////////
	//Import Texture from Raw File (Import is done at runtime)
	TextureType textureType = TextureType::Texture2D;

	switch (rawGA->GetContentSubtype())
	{
		case GameContentSubtype::Texture2D:
			textureType = TextureType::Texture2D;
			break;

		case GameContentSubtype::TextureCube:
			textureType = TextureType::TextureCube;
			break;

		default:
			return XEResult::InvalidTextureType;
			break;
	}

	/////////////////////////////////////////////////////////////
	//Create Asset if needed and load new file
	bool isNew = false;
	TextureAsset* textureAsset = nullptr;

	if(GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
	{
		textureAsset = reinterpret_cast<TextureAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
	}
	else
	{
		isNew = true;

		XETODO("When creating own texture reader make sure to put it in the asset paths the output");
		std::wstring textureName = XE_Base::GetFilenameOnly(rawGA->GetFilePath());
		textureAsset = new TextureAsset(rawGA->GetFilePath(), m_GameResourceManager, textureType, m_GraphicDevice);
		textureAsset->SetName(textureName);

		if (preloadGameAssetID != 0)
		{
			textureAsset->SetUniqueAssetID(preloadGameAssetID);
		}
		else
		{
			textureAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
		}
	}

	ret = textureAsset->LoadAsset();
	if(ret != XEResult::Ok)
	{
		XETODO("add log");

		if(isNew)
		{
			DeleteMem(textureAsset);
		}

		return XEResult::ImportFail;
	}

	if(isNew)
	{
		textureAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[textureAsset->GetUniqueAssetID()] = textureAsset;

		rawGA->SetUniqueAssociatedAssetID(textureAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportGameObjectScript(RawGameAsset* rawGA, uint64_t preloadGameAssetID)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(rawGA != nullptr);
	if (rawGA == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (preloadGameAssetID != 0 && GameAssetExists(preloadGameAssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////////////
	//Create Asset if needed and load new file
	bool isNew = false;
	GameObjectScriptAsset* gosAsset = nullptr;

	if (GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
	{
		gosAsset = reinterpret_cast<GameObjectScriptAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
	}
	else
	{
		isNew = true;

		XETODO("When creating own texture reader make sure to put it in the asset paths the output");
		std::wstring gameScriptObjectName = XE_Base::GetFilenameOnly(rawGA->GetFilePath());
		gosAsset = new GameObjectScriptAsset(rawGA->GetFilePath(), m_GameResourceManager, m_AngelScriptManager);
		gosAsset->SetName(gameScriptObjectName);

		if (preloadGameAssetID != 0)
		{
			gosAsset->SetUniqueAssetID(preloadGameAssetID);
		}
		else
		{
			gosAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
		}
	}

	ret = gosAsset->LoadAsset();
	if (ret != XEResult::Ok)
	{
		XETODO("add log");

		if (isNew)
		{
			DeleteMem(gosAsset);
		}

		return XEResult::ImportFail;
	}

	if (isNew)
	{
		gosAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[gosAsset->GetUniqueAssetID()] = gosAsset;

		rawGA->SetUniqueAssociatedAssetID(gosAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportAudio(RawGameAsset* rawGA, uint64_t preloadGameAssetID)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(rawGA != nullptr);
	if (rawGA == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (preloadGameAssetID != 0 && GameAssetExists(preloadGameAssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////////////
	//Import from Raw File (Import is done at runtime)

	/////////////////////////////////////////////////////////////
	//Create Asset if needed and load new file
	bool isNew = false;
	AudioAsset* audioAsset = nullptr;

	if (GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
	{
		audioAsset = reinterpret_cast<AudioAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
	}
	else
	{
		isNew = true;

		XETODO("When creating own audio reader make sure to put it in the asset paths the output");
		std::wstring audioName = XE_Base::GetFilenameOnly(rawGA->GetFilePath());
		audioAsset = new AudioAsset(rawGA->GetFilePath(), m_GameResourceManager, m_AudioManager);
		audioAsset->SetName(audioName);

		if (preloadGameAssetID != 0)
		{
			audioAsset->SetUniqueAssetID(preloadGameAssetID);
		}
		else
		{
			audioAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
		}
	}

	ret = audioAsset->LoadAsset();
	if (ret != XEResult::Ok)
	{
		XETODO("add log");

		if (isNew)
		{
			DeleteMem(audioAsset);
		}

		return XEResult::ImportFail;
	}

	if (isNew)
	{
		audioAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[audioAsset->GetUniqueAssetID()] = audioAsset;

		rawGA->SetUniqueAssociatedAssetID(audioAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::SaveToXML(const std::wstring& gameAssetsFilename)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (gameAssetsFilename.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLWriter xmlWriter;
	XEResult ret = XEResult::Ok;

	ret = xmlWriter.CreateXML(gameAssetsFilename, true);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.StartNode(XE_ASSET_MANAGER_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.StartNode(XE_ASSET_CONFIG_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	xmlWriter.WriteUInt64(XE_ASSET_PERSISTANTUNIQUEID_PROP_NAME, this->m_UniqueAssetIDGen);

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLGameAssets(xmlWriter);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLRawGameAssets(xmlWriter);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.FinalizeXML();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::SaveToXMLGameAssets(XEXMLWriter& xmlWriter)
{
	XEResult ret = XEResult::Ok;

	ret = xmlWriter.StartNode(XE_ASSETS_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	for (auto gameAssetPair : m_GameAssetMap)
	{
		GameAsset* gameAsset = gameAssetPair.second;

		if (gameAsset->IsBuiltInAsset() || gameAsset->GetParentAssetID() != 0)
		{
			continue;
		}

		ret = SaveToXMLGameAsset(xmlWriter, gameAsset);
		if (ret != XEResult::Ok)
		{
			return ret;
		}
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::SaveToXMLGameAsset(XEXMLWriter& xmlWriter, GameAsset* gameAsset)
{
	if (gameAsset == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	ret = xmlWriter.StartNode(XE_ASSET_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	xmlWriter.WriteString(XE_ASSET_NAME_PROP, gameAsset->GetName());
	xmlWriter.WriteString(XE_ASSET_CUSTOM_NAME_PROP, gameAsset->GetCustomName());
	xmlWriter.WriteUInt64(XE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP, gameAsset->GetUniqueAssetID());
	xmlWriter.WriteUInt64(XE_ASSET_PARENTASSETID_PROP, gameAsset->GetParentAssetID());
	xmlWriter.WriteUInt(XE_ASSET_GAMECONTENTTYPE_PROP, (uint32_t)gameAsset->GetGameContentType());
	xmlWriter.WriteBool(XE_ASSET_ISLOADED_PROP, gameAsset->IsLoaded());

	const GameAssetList& children = gameAsset->GetGameAssetChildren();
	if (children.size() != 0)
	{
		for (auto gameAssetChild : children)
		{
			ret = SaveToXMLGameAsset(xmlWriter, gameAssetChild);
			if (ret != XEResult::Ok)
			{
				return ret;
			}
		}
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::SaveToXMLRawGameAssets(XEXMLWriter& xmlWriter)
{
	XEResult ret = XEResult::Ok;

	ret = xmlWriter.StartNode(XE_RAW_FILES_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	for (auto rawAssetPair : m_RawGameAssetMap)
	{
		RawGameAsset* rawGameAsset = rawAssetPair.second;

		ret = xmlWriter.StartNode(XE_RAW_FILE_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		xmlWriter.WriteString(XE_RAW_FILE_NAME_PROP, rawGameAsset->GetName());
		xmlWriter.WriteUInt(XE_RAW_FILE_CONTENTTYPE_PROP, (uint32_t)rawGameAsset->GetContentType());
		xmlWriter.WriteUInt(XE_RAW_FILE_CONTENTSUBTYPE_PROP, (uint32_t)rawGameAsset->GetContentSubtype());
		xmlWriter.WriteUInt(XE_RAW_FILE_GAMECONTEXTFILEEXT_PROP, (uint32_t)rawGameAsset->GetGameContextFileExt());
		xmlWriter.WriteString(XE_RAW_FILE_LASTMODIFIEDTIMESTAMP_PROP, rawGameAsset->GetLastModifiedTimeStamp().ToString());
		xmlWriter.WriteString(XE_RAW_FILE_FILEPATH_PROP, rawGameAsset->GetFilePath());
		xmlWriter.WriteString(XE_RAW_FILE_OUTPUTDIRECTORY_PROP, rawGameAsset->GetOutputDirectory());
		xmlWriter.WriteString(XE_RAW_FILE_OUTPUTFILENAME_PROP, rawGameAsset->GetOutputFileName());
		xmlWriter.WriteBool(XE_RAW_FILE_RELOADNEEDED_PROP, rawGameAsset->IsReloadNeeded());
		xmlWriter.WriteBool(XE_RAW_FILE_OUTPUTDIRCHANGED_PROP, rawGameAsset->GetOutputDirChanged());
		xmlWriter.WriteBool(XE_RAW_FILE_CONTENTSUBTYPECHANGED_PROP, rawGameAsset->GetContentSubtypeChanged());
		xmlWriter.WriteUInt64(XE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP, rawGameAsset->GetUniqueAssociatedAssetID());

		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadAssetManagerFile(const std::wstring& gameAssetsFilename)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (gameAssetsFilename.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLParser newFile;
	if (newFile.LoadFile(gameAssetsFilename) != XEResult::Ok)
	{
		std::wstring msg_error = L"";
		fastformat::fmt(msg_error, XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, gameAssetsFilename);

		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser configXML = newFile[L"AssetManager"];
	if (!configXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	uint32_t l_Count = configXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = configXML(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(L"Config") == 0)
		{
			m_UniqueAssetIDGen = child.GetUInt64(L"PersistantUniqueID");
		}
		else if (l_Type.compare(L"RawFiles") == 0)
		{
			child.GetNumChildren();
		}
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadRawAssets(XEXMLParser& rawAssetXML)
{
	uint32_t l_Count = rawAssetXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = rawAssetXML(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(L"RawFile") == 0)
		{
			TimeStamp modTimeStamp(child.GetString(XE_RAW_FILE_LASTMODIFIEDTIMESTAMP_PROP));
			std::wstring name			= child.GetString(XE_RAW_FILE_NAME_PROP);
			std::wstring filepath		= child.GetString(XE_RAW_FILE_FILEPATH_PROP);
			GameContentSubtype subType	= (GameContentSubtype)child.GetUInt(XE_RAW_FILE_CONTENTSUBTYPE_PROP);
			std::wstring outputDir		= child.GetString(XE_RAW_FILE_OUTPUTDIRECTORY_PROP);
			uint64_t uniqueAssociatedID	= child.GetUInt64(XE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP);
			GameContentType contentType	= (GameContentType)child.GetUInt(XE_RAW_FILE_CONTENTTYPE_PROP);
			GameContextFileExt fileExt	= (GameContextFileExt)child.GetUInt(XE_RAW_FILE_GAMECONTEXTFILEEXT_PROP);
			std::wstring outputFilename	= child.GetString(XE_RAW_FILE_OUTPUTFILENAME_PROP);
			bool reloadNeeded			= child.GetBool(XE_RAW_FILE_RELOADNEEDED_PROP);
			bool outputDirChanged		= child.GetBool(XE_RAW_FILE_OUTPUTDIRCHANGED_PROP);
			bool contentSubtypeChanged	= child.GetBool(XE_RAW_FILE_CONTENTSUBTYPECHANGED_PROP);

			RawGameAsset* rawGameAsset = new RawGameAsset(filepath, m_ProjectDirectory, name);

			rawGameAsset->SetContentSubtype(subType);
			rawGameAsset->SetOutputDirectory(outputDir);
			rawGameAsset->SetUniqueAssociatedAssetID(uniqueAssociatedID);
			rawGameAsset->SetContentType(contentType);
			rawGameAsset->SetGameContextFileExt(fileExt);
			rawGameAsset->SetLastModifiedTimeStamp(modTimeStamp);
			rawGameAsset->SetOutputFileName(outputFilename);
			rawGameAsset->SetReloadNeeded(reloadNeeded);
			rawGameAsset->SetOutputDirChanged(outputDirChanged);
			rawGameAsset->SetContentSubtypeChanged(contentSubtypeChanged);

			m_RawGameAssetMap[rawGameAsset->GetUniqueID()] = rawGameAsset;
		}
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadGameAssets(XEXMLParser& gameAssetXML)
{
	//uint32_t l_Count = gameAssetXML.GetNumChildren();
	//for (uint32_t i = 0; i < l_Count; ++i)
	//{
	//	XEXMLParser child = gameAssetXML(i);

	//	std::wstring l_Type = child.GetName();

	//	if (l_Type.compare(L"Asset") == 0)
	//	{
	//	}
	//}

	return XEResult::Fail;
}
