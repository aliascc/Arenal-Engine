
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/16/2013
*
* Desc:
*
*
* Copyright 2013
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
	for (size_t i = 0; i < m_MeshAssetVector.size(); i++)
	{
		DeleteMem(m_MeshAssetVector[i]);
	}
	m_MeshAssetVector.clear();

	for (size_t i = 0; i < m_AnimationAssetVector.size(); i++)
	{
		DeleteMem(m_AnimationAssetVector[i]);
	}
	m_AnimationAssetVector.clear();

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
	uint32_t numMeshes = (uint32_t)m_MeshAssetVector.size();
	for (uint32_t i = 0; i < numMeshes; i++)
	{
		ret = m_MeshAssetVector[i]->LoadAsset();
		
		if(ret != XEResult::Ok)
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
	uint32_t numAnims = (uint32_t)m_AnimationAssetVector.size();
	for (uint32_t i = 0; i < numAnims; i++)
	{
		ret = m_AnimationAssetVector[i]->LoadAsset();
		
		if(ret != XEResult::Ok)
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
	//Set Vector Size and initialize unassigned to null
	uint32_t originalMeshVectorSize = (uint32_t)m_MeshAssetVector.size();

	if(originalMeshVectorSize < numMeshes)
	{
		m_MeshAssetVector.resize(numMeshes);

		for (uint32_t i = originalMeshVectorSize; i < numMeshes; i++)
		{
			m_MeshAssetVector[i] = nullptr;
		}
	}

	/////////////////////////////////////////////
	//Get Mesh File Names
	for(uint32_t i = 0; i < numMeshes; ++i)
	{
		/////////////////////////////////////////////
		//Read Mesh Name and File Name
		std::wstring meshName = XEGameContentHelpers::ReadString(fileStream);
		std::wstring meshPath = XEGameContentHelpers::ReadString(fileStream);
		
		MeshAsset* meshAsset = m_MeshAssetVector[i];

		if(meshAsset == nullptr)
		{
			m_MeshAssetVector[i] = new MeshAsset(meshPath, m_GameResourceManager, m_GraphicDevice);

			meshAsset = m_MeshAssetVector[i];
			meshAsset->SetName(meshName);
			meshAsset->SetParentAssetID(this->GetUniqueAssetID());
		}
		else
		{
			meshAsset->SetFilePath(meshPath);
			meshAsset->SetName(meshName);
		}
	}

	/////////////////////////////////////////////
	//Remove any unused Asset
	if(numMeshes < originalMeshVectorSize)
	{
		for (uint32_t i = originalMeshVectorSize - 1; i >= numMeshes; i--)
		{
			MeshAsset* meshAsset = m_MeshAssetVector[i];
			m_MeshAssetVector.pop_back();

			DeleteMem(meshAsset);
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

	if(m_SkeletonAsset == nullptr)
	{
		m_SkeletonAsset = new SkeletonAsset(skeletonFile, m_GameResourceManager);
		m_SkeletonAsset->SetName(skeletonName);
		m_SkeletonAsset->SetParentAssetID(this->GetUniqueAssetID());
	}
	else
	{
		m_SkeletonAsset->SetFilePath(skeletonFile);
		m_SkeletonAsset->SetName(skeletonName);
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
	uint32_t originalAnimationVectorSize = (uint32_t)m_AnimationAssetVector.size();

	if(originalAnimationVectorSize < numAnim)
	{
		m_AnimationAssetVector.resize(numAnim);

		for (uint32_t i = originalAnimationVectorSize; i < numAnim; i++)
		{
			m_AnimationAssetVector[i] = nullptr;
		}
	}

	/////////////////////////////////////////////
	//Get Animations File Names
	for(uint32_t i = 0; i < numAnim; ++i)
	{		
		/////////////////////////////////////////////
		//Read Animation Name and File Name
		std::wstring animName = XEGameContentHelpers::ReadString(fileStream);
		std::wstring animFilePath = XEGameContentHelpers::ReadString(fileStream);
		
		AnimationAsset* animationAsset = m_AnimationAssetVector[i];

		if(animationAsset == nullptr)
		{
			m_AnimationAssetVector[i] = new AnimationAsset(animFilePath, m_GameResourceManager);

			animationAsset = m_AnimationAssetVector[i];
			animationAsset->SetName(animName);
			animationAsset->SetParentAssetID(this->GetUniqueAssetID());
		}
		else
		{
			animationAsset->SetFilePath(animFilePath);
			animationAsset->SetName(animName);
		}
	}

	/////////////////////////////////////////////
	//Remove any unused Asset
	if(numAnim < originalAnimationVectorSize)
	{
		for (uint32_t i = originalAnimationVectorSize - 1; i >= numAnim; i--)
		{
			AnimationAsset* animAsset = m_AnimationAssetVector[i];
			m_AnimationAssetVector.pop_back();

			DeleteMem(animAsset);
		}
	}

	return XEResult::Ok;
}