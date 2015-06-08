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
#include "cppformat\format.h"
#include "boost\filesystem.hpp"

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
			m_GameAssetMap[gameAssetPair.first] = nullptr;
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

RawGameAsset* GameAssetManager::GetRawGameAssetByGameAssetID(uint64_t gameAssetID)
{
	if (!m_IsReady)
	{
		return nullptr;
	}

	//No need for lock as this is an internal method only

	for (auto rawAssetPair : m_RawGameAssetMap)
	{
		if (rawAssetPair.second->GetUniqueAssociatedAssetID() == gameAssetID)
		{
			return rawAssetPair.second;
		}
	}

	return nullptr;
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
	std::wstring rawAssetFilepath = XE_Base::GetRelativePath(filePath);
	RawGameAsset* rawGA = new RawGameAsset(rawAssetFilepath, m_ProjectDirectory, XE_Base::GetFilenameOnly(filePath));

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

XEResult GameAssetManager::ImportModel(RawGameAsset* rawGA)
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
	ret = writerXE3D.WriteToFile(modelContent, rawGA->GetOutputFileName(), m_OutputDirAssets);

	DeleteMem(modelContent);

	if(ret != XEResult::Ok)
	{
		XETODO("Add Log");

		return XEResult::WriteToFileFailed;
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

		modelAsset->SetCustomName(modelAsset->GetCustomName());
		modelAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
	}

	ret = modelAsset->LoadAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("add log");

		if(isNew)
		{
			DeleteMem(modelAsset);
		}

		return XEResult::LoadFileFailed;
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
			meshAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());

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
			animAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());

			animAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

			m_GameAssetMap[animAsset->GetUniqueAssetID()] = animAsset;
		}
	}

	SkeletonAsset* skeletonAsset = modelAsset->GetSkeletonAsset();
	if(skeletonAsset != nullptr && skeletonAsset->GetUniqueAssetID() == 0)
	{
		skeletonAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());

		skeletonAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[skeletonAsset->GetUniqueAssetID()] = skeletonAsset;
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportShader(RawGameAsset* rawGA)
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
	ret = writerHLSL.WriteToFile(shaderContent, rawGA->GetOutputFileName(), m_OutputDirAssets);

	DeleteMem(shaderContent);

	if(ret != XEResult::Ok)
	{
		XETODO("Add Log");

		return XEResult::WriteToFileFailed;
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

		shaderAsset->SetCustomName(shaderAsset->GetCustomName());
		shaderAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
	}

	ret = shaderAsset->LoadAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("add log");

		if(isNew)
		{
			DeleteMem(shaderAsset);
		}

		return XEResult::LoadFileFailed;
	}

	if(isNew)
	{
		shaderAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[shaderAsset->GetUniqueAssetID()] = shaderAsset;

		rawGA->SetUniqueAssociatedAssetID(shaderAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportTexture(RawGameAsset* rawGA)
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
		textureAsset->SetCustomName(textureName);

		textureAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
	}

	ret = textureAsset->LoadAsset();
	if(ret != XEResult::Ok)
	{
		XETODO("add log");

		if(isNew)
		{
			DeleteMem(textureAsset);
		}

		return XEResult::LoadFileFailed;
	}

	if(isNew)
	{
		textureAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[textureAsset->GetUniqueAssetID()] = textureAsset;

		rawGA->SetUniqueAssociatedAssetID(textureAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportGameObjectScript(RawGameAsset* rawGA)
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
		gosAsset->SetCustomName(gameScriptObjectName);

		gosAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
	}

	ret = gosAsset->LoadAsset();
	if (ret != XEResult::Ok)
	{
		XETODO("add log");

		if (isNew)
		{
			DeleteMem(gosAsset);
		}

		return XEResult::LoadFileFailed;
	}

	if (isNew)
	{
		gosAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[gosAsset->GetUniqueAssetID()] = gosAsset;

		rawGA->SetUniqueAssociatedAssetID(gosAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::ImportAudio(RawGameAsset* rawGA)
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
		audioAsset->SetCustomName(audioName);

		audioAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
	}

	ret = audioAsset->LoadAsset();
	if (ret != XEResult::Ok)
	{
		XETODO("add log");

		if (isNew)
		{
			DeleteMem(audioAsset);
		}

		return XEResult::LoadFileFailed;
	}

	if (isNew)
	{
		audioAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

		m_GameAssetMap[audioAsset->GetUniqueAssetID()] = audioAsset;

		rawGA->SetUniqueAssociatedAssetID(audioAsset->GetUniqueAssetID());
	}

	return XEResult::Ok;
}

void GameAssetManager::SetAssetDetails(const GameAssetLoadingDetails& details, GameAsset* asset)
{
	if (asset == nullptr)
	{
		return;
	}

	asset->SetName(details.m_Name);
	asset->SetCustomName(details.m_CustomName);
	asset->SetUniqueAssetID(details.m_AssetID);
	asset->SetParentAssetID(details.m_ParentAssetID);
	asset->m_IsLoaded = details.m_IsLoaded;
}

XEResult GameAssetManager::LoadModelAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	ModelAsset* asset = new ModelAsset(details.m_Filepath, m_GameResourceManager, m_GraphicDevice);

	SetAssetDetails(details, asset);

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadMeshAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	GameAsset* gameAsset = GetGameAsset(details.m_ParentAssetID);
	if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Model)
	{
		return XEResult::GameAssetInvalidID;
	}
	ModelAsset* modelAsset = reinterpret_cast<ModelAsset*>(gameAsset);

	XEResult ret = XEResult::Ok;

	MeshAsset* asset = new MeshAsset(details.m_Filepath, m_GameResourceManager, m_GraphicDevice);

	SetAssetDetails(details, asset);

	if (asset->IsLoaded())
	{
		ret = asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			XETODO("add log");

			DeleteMem(asset);

			return XEResult::LoadFileFailed;
		}
	}

	MeshAssetMap& meshMap = modelAsset->GetMeshAssetMap();
	meshMap[asset->GetName()] = GameAssetLoadStatus<MeshAsset>(asset, true);

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadAnimationAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	GameAsset* gameAsset = GetGameAsset(details.m_ParentAssetID);
	if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Model)
	{
		return XEResult::GameAssetInvalidID;
	}
	ModelAsset* modelAsset = reinterpret_cast<ModelAsset*>(gameAsset);

	XEResult ret = XEResult::Ok;

	AnimationAsset* asset = new AnimationAsset(details.m_Filepath, m_GameResourceManager);

	SetAssetDetails(details, asset);

	if (asset->IsLoaded())
	{
		ret = asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			XETODO("add log");

			DeleteMem(asset);

			return XEResult::LoadFileFailed;
		}
	}

	AnimationAssetMap& animMap = modelAsset->GetAnimationAssetMap();
	animMap[asset->GetName()] = GameAssetLoadStatus<AnimationAsset>(asset, true);

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadSkeletonAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	GameAsset* gameAsset = GetGameAsset(details.m_ParentAssetID);
	if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Model)
	{
		return XEResult::GameAssetInvalidID;
	}
	ModelAsset* modelAsset = reinterpret_cast<ModelAsset*>(gameAsset);

	XEResult ret = XEResult::Ok;

	SkeletonAsset* asset = new SkeletonAsset(details.m_Filepath, m_GameResourceManager);

	SetAssetDetails(details, asset);

	if (asset->IsLoaded())
	{
		ret = asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			XETODO("add log");

			DeleteMem(asset);

			return XEResult::LoadFileFailed;
		}
	}

	modelAsset->SetSkeletonAsset(asset);

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadShaderAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	RawGameAsset* rawGameAsset = GetRawGameAssetByGameAssetID(details.m_AssetID);
	if (rawGameAsset == nullptr)
	{
		return XEResult::RawGameAssetNotFound;
	}

	ShaderType shaderType = ShaderType::VertexShader;
	switch (rawGameAsset->GetContentSubtype())
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

	ShaderAsset* asset = new ShaderAsset(details.m_Filepath, m_GameResourceManager, shaderType, m_GraphicDevice);

	SetAssetDetails(details, asset);

	if (asset->IsLoaded())
	{
		ret = asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			XETODO("add log");

			DeleteMem(asset);

			return XEResult::LoadFileFailed;
		}
	}

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadTextureAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	RawGameAsset* rawGameAsset = GetRawGameAssetByGameAssetID(details.m_AssetID);
	if (rawGameAsset == nullptr)
	{
		return XEResult::RawGameAssetNotFound;
	}

	TextureType textureType = TextureType::Texture2D;
	switch (rawGameAsset->GetContentSubtype())
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

	TextureAsset* asset = new TextureAsset(details.m_Filepath, m_GameResourceManager, textureType, m_GraphicDevice);

	SetAssetDetails(details, asset);

	if (asset->IsLoaded())
	{
		ret = asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			XETODO("add log");

			DeleteMem(asset);

			return XEResult::LoadFileFailed;
		}
	}

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadGameObjectScriptAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	GameObjectScriptAsset* asset = new GameObjectScriptAsset(details.m_Filepath, m_GameResourceManager, m_AngelScriptManager);

	SetAssetDetails(details, asset);

	if (asset->IsLoaded())
	{
		ret = asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			XETODO("add log");

			DeleteMem(asset);

			return XEResult::LoadFileFailed;
		}
	}

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadAudioAsset(const GameAssetLoadingDetails& details)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (GameAssetExists(details.m_AssetID))
	{
		return XEResult::GameAssetIDInUse;
	}

	XEResult ret = XEResult::Ok;

	AudioAsset* asset = new AudioAsset(details.m_Filepath, m_GameResourceManager, m_AudioManager);

	SetAssetDetails(details, asset);

	if (asset->IsLoaded())
	{
		ret = asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			XETODO("add log");

			DeleteMem(asset);

			return XEResult::LoadFileFailed;
		}
	}

	asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

	m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

	return XEResult::Ok;
}

XEResult GameAssetManager::SaveToXML(const std::wstring& gameAssetsFilename) const
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

	ret = SaveToXMLRawGameAssets(xmlWriter);
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

XEResult GameAssetManager::SaveToXMLGameAssets(XEXMLWriter& xmlWriter) const
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

		if (gameAsset->IsBuiltInAsset())
		{
			continue;
		}

		ret = xmlWriter.StartNode(XE_ASSET_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		xmlWriter.WriteString(XE_ASSET_NAME_PROP, gameAsset->GetName());
		xmlWriter.WriteString(XE_ASSET_CUSTOM_NAME_PROP, gameAsset->GetCustomName());
		xmlWriter.WriteString(XE_ASSET_FILEPATH_PROP, gameAsset->GetFilePath());
		xmlWriter.WriteUInt64(XE_ASSET_UNIQUEASSETID_PROP, gameAsset->GetUniqueAssetID());
		xmlWriter.WriteUInt64(XE_ASSET_PARENTASSETID_PROP, gameAsset->GetParentAssetID());
		xmlWriter.WriteUInt(XE_ASSET_GAMECONTENTTYPE_PROP, (uint32_t)gameAsset->GetGameContentType());
		xmlWriter.WriteBool(XE_ASSET_ISLOADED_PROP, gameAsset->IsLoaded());

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

XEResult GameAssetManager::SaveToXMLRawGameAssets(XEXMLWriter& xmlWriter) const
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
		xmlWriter.WriteString(XE_RAW_FILE_CUSTOM_NAME_PROP, rawGameAsset->GetCustomName());
		xmlWriter.WriteUInt(XE_RAW_FILE_CONTENTTYPE_PROP, (uint32_t)rawGameAsset->GetContentType());
		xmlWriter.WriteUInt(XE_RAW_FILE_CONTENTSUBTYPE_PROP, (uint32_t)rawGameAsset->GetContentSubtype());
		xmlWriter.WriteUInt(XE_RAW_FILE_GAMECONTEXTFILEEXT_PROP, (uint32_t)rawGameAsset->GetGameContextFileExt());
		xmlWriter.WriteString(XE_RAW_FILE_LASTMODIFIEDTIMESTAMP_PROP, rawGameAsset->GetLastModifiedTimeStamp().ToString());
		xmlWriter.WriteString(XE_RAW_FILE_FILEPATH_PROP, rawGameAsset->GetFilePath());
		xmlWriter.WriteString(XE_RAW_FILE_OUTPUTFILENAME_PROP, rawGameAsset->GetOutputFileName());
		xmlWriter.WriteBool(XE_RAW_FILE_RELOADNEEDED_PROP, rawGameAsset->IsReloadNeeded());
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
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, gameAssetsFilename);

		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser configXML = newFile[XE_ASSET_MANAGER_NODE_NAME];
	if (!configXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	uint32_t l_Count = configXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = configXML(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(XE_ASSET_CONFIG_NODE_NAME) == 0)
		{
			m_UniqueAssetIDGen = child.GetUInt64(XE_ASSET_PERSISTANTUNIQUEID_PROP_NAME);
		}
		else if (l_Type.compare(XE_RAW_FILES_NODE_NAME) == 0)
		{
			if (LoadRawAssets(child) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (l_Type.compare(XE_ASSETS_NODE_NAME) == 0)
		{
			if (LoadGameAssets(child) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
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
			std::wstring name				= child.GetString(XE_RAW_FILE_NAME_PROP);
			std::wstring customName			= child.GetString(XE_RAW_FILE_CUSTOM_NAME_PROP);
			std::wstring filepath			= child.GetString(XE_RAW_FILE_FILEPATH_PROP);
			GameContentSubtype subType		= (GameContentSubtype)child.GetUInt(XE_RAW_FILE_CONTENTSUBTYPE_PROP);
			uint64_t uniqueAssociatedID		= child.GetUInt64(XE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP);
			GameContentType contentType		= (GameContentType)child.GetUInt(XE_RAW_FILE_CONTENTTYPE_PROP);
			GameContextFileExt fileExt		= (GameContextFileExt)child.GetUInt(XE_RAW_FILE_GAMECONTEXTFILEEXT_PROP);
			std::wstring outputFilename		= child.GetString(XE_RAW_FILE_OUTPUTFILENAME_PROP);
			bool reloadNeeded				= child.GetBool(XE_RAW_FILE_RELOADNEEDED_PROP);
			bool contentSubtypeChanged		= child.GetBool(XE_RAW_FILE_CONTENTSUBTYPECHANGED_PROP);

			RawGameAsset* rawGameAsset = new RawGameAsset(filepath, m_ProjectDirectory, name);

			rawGameAsset->SetContentSubtype(subType);
			rawGameAsset->SetUniqueAssociatedAssetID(uniqueAssociatedID);
			rawGameAsset->SetContentType(contentType);
			rawGameAsset->SetGameContextFileExt(fileExt);
			rawGameAsset->SetLastModifiedTimeStamp(modTimeStamp);
			rawGameAsset->SetOutputFileName(outputFilename);
			rawGameAsset->SetReloadNeeded(reloadNeeded);
			rawGameAsset->SetContentSubtypeChanged(contentSubtypeChanged);

			m_RawGameAssetMap[rawGameAsset->GetUniqueID()] = rawGameAsset;
		}
	}

	return XEResult::Ok;
}

XEResult GameAssetManager::LoadGameAssets(XEXMLParser& gameAssetXML)
{
	uint32_t l_Count = gameAssetXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = gameAssetXML(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(L"Asset") == 0)
		{
			GameContentType type = (GameContentType)child.GetUInt(XE_ASSET_GAMECONTENTTYPE_PROP);

			GameAssetLoadingDetails details;

			details.m_Name				= child.GetString(XE_ASSET_NAME_PROP);
			details.m_CustomName		= child.GetString(XE_ASSET_CUSTOM_NAME_PROP);
			details.m_Filepath			= child.GetString(XE_ASSET_FILEPATH_PROP);
			details.m_AssetID			= child.GetUInt64(XE_ASSET_UNIQUEASSETID_PROP);
			details.m_ParentAssetID		= child.GetUInt64(XE_ASSET_PARENTASSETID_PROP);
			details.m_IsLoaded			= child.GetBool(XE_ASSET_ISLOADED_PROP);

			XEResult ret = XEResult::Ok;
			switch (type)
			{
				case GameContentType::Model:
					ret = LoadModelAsset(details);
					break;

				case GameContentType::Mesh:
					ret = LoadMeshAsset(details);
					break;

				case GameContentType::Animation:
					ret = LoadAnimationAsset(details);
					break;

				case GameContentType::Skeleton:
					ret = LoadSkeletonAsset(details);
					break;

				case GameContentType::Texture:
					ret = LoadTextureAsset(details);
					break;

				case GameContentType::Shader:
					ret = LoadShaderAsset(details);
					break;

				case GameContentType::GameObjectScript:
					ret = LoadGameObjectScriptAsset(details);
					break;

				case GameContentType::Audio:
					ret = LoadAudioAsset(details);
					break;

				default:
					return XEResult::InvalidObjType;
			}

			if (ret != XEResult::Ok)
			{
				return ret;
			}
		}
	}

	return XEResult::Ok;
}
