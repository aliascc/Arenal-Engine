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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "MeshAsset.h"
#include "ModelAsset.h"
#include "GraphicDevice.h"
#include "SkeletonAsset.h"
#include "AnimationAsset.h"
#include "Base\BaseFunctions.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check if this class needs a mutex");
ModelAsset::ModelAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, GraphicDevice* graphicDevice)
	: GameAsset(GameContentType::Model, filePath, gameResourceManager)
	, m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);
}

ModelAsset::~ModelAsset()
{
	CleanUp();
}

void ModelAsset::CleanUp()
{
	for (auto gameAssetLoadedIt : m_MeshAssetMap)
	{
		GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

		DeleteMem(gameAssetLoaded.m_Asset);
	}
	m_MeshAssetMap.clear();

	for (auto gameAssetLoadedIt : m_AnimationAssetMap)
	{
		GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

		DeleteMem(gameAssetLoaded.m_Asset);
	}
	m_AnimationAssetMap.clear();

	DeleteMem(m_SkeletonAsset);
}

XEResult ModelAsset::LoadAssetResource()
{
	XEAssert(m_GameResourceManager != nullptr);
	if(m_GameResourceManager == nullptr)
	{
		return XEResult::GameResourceManagerNull;
	}

	XEAssert(!m_FilePath.empty());
	if(m_FilePath.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEResult ret = XEResult::Ok;
	
	////////////////////////////////////////
	//Load Model
	ret = LoadFile();

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	////////////////////////////////////////
	//Load Meshes
	for (auto gameAssetLoadedIt : m_MeshAssetMap)
	{
		GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

		ret = gameAssetLoaded.m_Asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			return ret;
		}
	}

	////////////////////////////////////////
	//Load Skeleton
	if(m_SkeletonAsset != nullptr)
	{
		m_SkeletonAsset->LoadAsset();
		
		if(ret != XEResult::Ok)
		{
			return ret;
		}
	}

	////////////////////////////////////////
	//Load Animations
	for (auto gameAssetLoadedIt : m_AnimationAssetMap)
	{
		GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

		ret = gameAssetLoaded.m_Asset->LoadAsset();
		if (ret != XEResult::Ok)
		{
			return ret;
		}
	}

	////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult ModelAsset::LoadFile()
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(m_GameResourceManager != nullptr);
	if (m_GameResourceManager == nullptr)
	{
		return XEResult::GameResourceManagerNull;
	}

	XEAssert(!m_FilePath.empty());
	if(m_FilePath.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEResult ret = XEResult::Ok;
	
	std::ifstream modelFile;

	modelFile.open(m_FilePath, std::ios::binary | std::ios::in);

	if(!modelFile.is_open())
	{
		XETODO("add log");
		return XEResult::OpenFileFail;
	}
	
	/////////////////////////////////////////////
	//Verify Header
	bool verifyHeader = XEGameContentHelpers::ReadFileHeaderAndVerify(modelFile, XE_CT_XE3D_FILE_HEADER, XE_CT_XE3D_FILE_VERSION_MAYOR, XE_CT_XE3D_FILE_VERSION_MINOR, XE_CT_XE3D_FILE_VERSION_REVISON);

	if(!verifyHeader)
	{
		XETODO("Add log");
		return XEResult::InvalidFileHeader;
	}

	/////////////////////////////////////////////
	//Initial check pass, Read Information for
	//Model Asset

	/////////////////////////////////////////////
	//Read Name of Model
	this->SetName(XEGameContentHelpers::ReadString(modelFile));
	if (m_CustomName.empty())
	{
		m_CustomName = this->GetName();
	}

	/////////////////////////////////////////////
	//Read Meshes Filenames
	ret = ReadModelMeshFiles(modelFile);
	if(ret != XEResult::Ok)
	{
		XETODO("Add log");

		CleanUp();

		return ret;
	}
	
	/////////////////////////////////////////////
	//Read Skeleton Filename
	ret = ReadModelSkeletonFile(modelFile);
	if(ret != XEResult::Ok)
	{
		XETODO("Add log");

		CleanUp();

		return ret;
	}
	
	/////////////////////////////////////////////
	//Read Animations Filenames 
	ret = ReadModelAnimationFiles(modelFile);
	if(ret != XEResult::Ok)
	{
		XETODO("Add log");
		
		CleanUp();

		return ret;
	}
	
	/////////////////////////////////////////////
	//Read Footer 
	bool verifyFooter = XEGameContentHelpers::ReadFileFooterAndVerify(modelFile, XE_CT_XE3D_FILE_FOOTER);
	if(!verifyFooter)
	{
		XETODO("Add Warning log");
	}

	////////////////////////////////////////
	//Finish
	modelFile.close();

	return XEResult::Ok;
}

XEResult ModelAsset::ReadModelMeshFiles(std::ifstream& fileStream)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;

	/////////////////////////////////////////////
	//Read Number of Meshes
	uint32_t numMeshes = 0;
	tempPtr = reinterpret_cast<char*>(&numMeshes);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);
	
	/////////////////////////////////////////////
	//Set all Meshes to unloaded
	for (auto gameAssetLoadedIt : m_MeshAssetMap)
	{
		gameAssetLoadedIt.second.m_IsLoaded = false;
	}

	/////////////////////////////////////////////
	//Get Mesh File Names
	for(uint32_t i = 0; i < numMeshes; ++i)
	{
		/////////////////////////////////////////////
		//Read Mesh Name and File Name
		std::wstring meshName = XEGameContentHelpers::ReadString(fileStream);
		std::wstring meshPath = XEGameContentHelpers::ReadString(fileStream);

		auto mapIt = m_MeshAssetMap.find(meshName);
		if (mapIt != m_MeshAssetMap.end())
		{
			GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = mapIt->second;
			MeshAsset* meshAsset = gameAssetLoaded.m_Asset;

			XEAssert(meshAsset != nullptr);
			if (meshAsset == nullptr)
			{
				return XEResult::NullObj;
			}

			meshAsset->SetFilePath(meshPath);

			gameAssetLoaded.m_IsLoaded = true;
		}
		else
		{
			MeshAsset* meshAsset = new MeshAsset(meshPath, m_GameResourceManager, m_GraphicDevice);

			meshAsset->SetFilePath(meshPath);
			meshAsset->SetName(meshName);
			meshAsset->SetCustomName(meshName);
			meshAsset->SetParentAssetID(this->GetUniqueAssetID());

			m_MeshAssetMap[meshName] = GameAssetLoadStatus<MeshAsset>(meshAsset, true);

			AddChildGameAsset(meshAsset);
		}
	}

	/////////////////////////////////////////////
	//Remove any unused Asset
	for (auto gameAssetLoadedIt : m_MeshAssetMap)
	{
		GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

		if (!gameAssetLoaded.m_IsLoaded)
		{
			DeleteMem(gameAssetLoaded.m_Asset);

			m_MeshAssetMap.erase(gameAssetLoadedIt.first);
		}
	}

	return XEResult::Ok;
}

XEResult ModelAsset::ReadModelSkeletonFile(std::ifstream& fileStream)
{	
	uint32_t tempUInt32 = 0;
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;
	
	/////////////////////////////////////////////
	//Write if Skeleton is present
	bool skeletonExists = false;
	tempPtr = reinterpret_cast<char*>(&skeletonExists);
	sizeToRead = sizeof(bool);
	fileStream.read(tempPtr, sizeToRead);
	
	if(!skeletonExists)
	{
		return XEResult::Ok;
	}

	/////////////////////////////////////////////
	//Read Skeleton Name and File Name
	std::wstring skeletonName = XEGameContentHelpers::ReadString(fileStream);
	std::wstring skeletonFile = XEGameContentHelpers::ReadString(fileStream);

	if(m_SkeletonAsset == nullptr || (m_SkeletonAsset->GetName().compare(skeletonName) != 0))
	{
		DeleteMem(m_SkeletonAsset);

		m_SkeletonAsset = new SkeletonAsset(skeletonFile, m_GameResourceManager);
		m_SkeletonAsset->SetName(skeletonName);
		m_SkeletonAsset->SetParentAssetID(this->GetUniqueAssetID());

		AddChildGameAsset(m_SkeletonAsset);
	}
	else
	{
		m_SkeletonAsset->SetFilePath(skeletonFile);
	}

	return XEResult::Ok;
}

XEResult ModelAsset::ReadModelAnimationFiles(std::ifstream& fileStream)
{
	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;
	
	/////////////////////////////////////////////
	//Read Number of Animations
	uint32_t numAnim = 0;
	tempPtr = reinterpret_cast<char*>(&numAnim);
	sizeToRead = sizeof(uint32_t);
	fileStream.read(tempPtr, sizeToRead);
	
	/////////////////////////////////////////////
	//Set Vector Size and initialize unassigned to null
	for (auto gameAssetLoadedIt : m_AnimationAssetMap)
	{
		gameAssetLoadedIt.second.m_IsLoaded = false;
	}

	/////////////////////////////////////////////
	//Get Animations File Names
	for(uint32_t i = 0; i < numAnim; ++i)
	{
		/////////////////////////////////////////////
		//Read Animation Name and File Name
		std::wstring animName = XEGameContentHelpers::ReadString(fileStream);
		std::wstring animFilePath = XEGameContentHelpers::ReadString(fileStream);

		auto mapIt = m_AnimationAssetMap.find(animName);
		if (mapIt != m_AnimationAssetMap.end())
		{
			GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = mapIt->second;
			AnimationAsset* animAsset = gameAssetLoaded.m_Asset;

			XEAssert(animAsset != nullptr);
			if (animAsset == nullptr)
			{
				return XEResult::NullObj;
			}

			animAsset->SetFilePath(animFilePath);

			gameAssetLoaded.m_IsLoaded = true;
		}
		else
		{
			AnimationAsset* animAsset = new AnimationAsset(animFilePath, m_GameResourceManager);

			animAsset->SetFilePath(animFilePath);
			animAsset->SetName(animName);
			animAsset->SetCustomName(animName);
			animAsset->SetParentAssetID(this->GetUniqueAssetID());

			m_AnimationAssetMap[animName] = GameAssetLoadStatus<AnimationAsset>(animAsset, true);

			AddChildGameAsset(animAsset);
		}
	}

	/////////////////////////////////////////////
	//Remove any unused Asset
	for (auto gameAssetLoadedIt : m_AnimationAssetMap)
	{
		GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

		if (!gameAssetLoaded.m_IsLoaded)
		{
			DeleteMem(gameAssetLoaded.m_Asset);

			m_MeshAssetMap.erase(gameAssetLoadedIt.first);
		}
	}

	return XEResult::Ok;
}
