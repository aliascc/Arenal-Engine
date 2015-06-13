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
#include "SkeletonAsset.h"
#include "Base\BaseFunctions.h"
#include "Models\Skinning\Skeleton.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check if this class needs a mutex");
SkeletonAsset::SkeletonAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager)
	: GameAsset(GameContentType::Skeleton, filePath, gameResourceManager)
{
}

SkeletonAsset::~SkeletonAsset()
{
	AERelease(m_Skeleton);
}

Skeleton* SkeletonAsset::GetSkeletonReference()
{
	if(m_Skeleton == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Skeleton*>(m_Skeleton->AddRef());
}

AEResult SkeletonAsset::LoadAssetResource()
{
	AEAssert(m_GameResourceManager != nullptr);
	if (m_GameResourceManager == nullptr)
	{
		return AEResult::GameResourceManagerNull;
	}

	AEAssert(!m_FilePath.empty());
	if(m_FilePath.empty())
	{
		return AEResult::EmptyFilename;
	}

	AEResult ret = AEResult::Ok;

	AETODO("Look into what happens if file changes location");

	if(m_Skeleton != nullptr)
	{
		AETODO("Check return value");
		ret = m_Skeleton->Load();
	}
	else
	{
		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Skeleton = (Skeleton*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::Skeleton);

		if(m_Skeleton != nullptr)
		{
			AETODO("Check if we always need to reload");
			AETODO("Check return value");
			m_Skeleton->Load();

			return AEResult::Ok;
		}
		
		/////////////////////////////////////////////
		//Create Resource
		m_Skeleton = new Skeleton(m_Name);
		
		/////////////////////////////////////////////
		//Set File Name and Load
		m_Skeleton->SetFileName(m_FilePath);
				
		AETODO("Check return value");
		m_Skeleton->Load();

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Skeleton, m_FilePath);
		if(ret != AEResult::Ok)
		{
			AETODO("Add log");

			AERelease(m_Skeleton);

			return AEResult::ResourceManagedFailed;
		}
	}

	return AEResult::Ok;
}