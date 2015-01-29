
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
#include "SkeletonAsset.h"
#include "Base\BaseFunctions.h"
#include "Models\Skinning\Skeleton.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check if this class needs a mutex");
SkeletonAsset::SkeletonAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager)
	: GameAsset(GameContentType::Skeleton, filePath, gameResourceManager)
{
}

SkeletonAsset::~SkeletonAsset()
{
	XERelease(m_Skeleton);
}

Skeleton* SkeletonAsset::GetSkeletonReference()
{
	if(m_Skeleton == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Skeleton*>(m_Skeleton->AddRef());
}

XEResult SkeletonAsset::LoadAssetResource()
{
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

	XETODO("Look into what happens if file changes location");

	if(m_Skeleton != nullptr)
	{
		XETODO("Check return value");
		ret = m_Skeleton->Load();
	}
	else
	{
		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Skeleton = (Skeleton*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::Skeleton);

		if(m_Skeleton != nullptr)
		{
			XETODO("Check if we always need to reload");
			XETODO("Check return value");
			m_Skeleton->Load();

			return XEResult::Ok;
		}
		
		/////////////////////////////////////////////
		//Create Resource
		m_Skeleton = new Skeleton(m_Name);
		
		/////////////////////////////////////////////
		//Set File Name and Load
		m_Skeleton->SetFileName(m_FilePath);
				
		XETODO("Check return value");
		m_Skeleton->Load();

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Skeleton, m_FilePath);
		if(ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_Skeleton);

			return XEResult::ResourceManagedFailed;
		}
	}

	return XEResult::Ok;
}