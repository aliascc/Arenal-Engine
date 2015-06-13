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
#include "GraphicDevice.h"
#include "AnimationAsset.h"
#include "Base\BaseFunctions.h"
#include "Models\Animation\Animation.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check if this class needs a mutex");
AnimationAsset::AnimationAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager)
	: GameAsset(GameContentType::Animation, filePath, gameResourceManager)
{
}

AnimationAsset::~AnimationAsset()
{
	AERelease(m_Animation);
}

Animation* AnimationAsset::GetAnimationReference()
{
	if(m_Animation == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Animation*>(m_Animation->AddRef());
}

AEResult AnimationAsset::LoadAssetResource()
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

	if(m_Animation != nullptr)
	{
		AETODO("Check return value");
		ret = m_Animation->Load();
	}
	else
	{
		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Animation = (Animation*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::Animation);

		if(m_Animation != nullptr)
		{
			AETODO("Check if we always need to reload");
			AETODO("Check return value");
			m_Animation->Load();

			return AEResult::Ok;
		}
		
		/////////////////////////////////////////////
		//Create Resource
		m_Animation = new Animation(m_Name);
		
		/////////////////////////////////////////////
		//Set File Name and Load
		m_Animation->SetFileName(m_FilePath);
				
		AETODO("Check return value");
		m_Animation->Load();

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Animation, m_FilePath);
		if(ret != AEResult::Ok)
		{
			AETODO("Add log");

			AERelease(m_Animation);

			return AEResult::ResourceManagedFailed;
		}
	}

	return AEResult::Ok;
}