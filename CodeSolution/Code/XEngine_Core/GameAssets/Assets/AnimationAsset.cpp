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
XETODO("Check if this class needs a mutex");
AnimationAsset::AnimationAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager)
	: GameAsset(GameContentType::Animation, filePath, gameResourceManager)
{
}

AnimationAsset::~AnimationAsset()
{
	XERelease(m_Animation);
}

Animation* AnimationAsset::GetAnimationReference()
{
	if(m_Animation == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Animation*>(m_Animation->AddRef());
}

XEResult AnimationAsset::LoadAssetResource()
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

	if(m_Animation != nullptr)
	{
		XETODO("Check return value");
		ret = m_Animation->Load();
	}
	else
	{
		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Animation = (Animation*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::Animation);

		if(m_Animation != nullptr)
		{
			XETODO("Check if we always need to reload");
			XETODO("Check return value");
			m_Animation->Load();

			return XEResult::Ok;
		}
		
		/////////////////////////////////////////////
		//Create Resource
		m_Animation = new Animation(m_Name);
		
		/////////////////////////////////////////////
		//Set File Name and Load
		m_Animation->SetFileName(m_FilePath);
				
		XETODO("Check return value");
		m_Animation->Load();

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Animation, m_FilePath);
		if(ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_Animation);

			return XEResult::ResourceManagedFailed;
		}
	}

	return XEResult::Ok;
}