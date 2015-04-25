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
#include "Models\Mesh.h"
#include "Base\BaseFunctions.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check if this class needs a mutex");
MeshAsset::MeshAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, GraphicDevice* graphicDevice)
	: GameAsset(GameContentType::Mesh, filePath, gameResourceManager)
	, m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);
}

MeshAsset::~MeshAsset()
{
	XERelease(m_Mesh);
}

Mesh* MeshAsset::GetMeshReference()
{
	if(m_Mesh == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Mesh*>(m_Mesh->AddRef());
}

XEResult MeshAsset::LoadAssetResource()
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

	XETODO("Look into what happens if file changes location");

	if(m_Mesh != nullptr)
	{
		XETODO("Check return value");
		ret = m_Mesh->Load();
	}
	else
	{
		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Mesh = (Mesh*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::Mesh);

		if(m_Mesh != nullptr)
		{
			XETODO("Check if we always need to reload");
			XETODO("Check return value");
			m_Mesh->Load();

			return XEResult::Ok;
		}
		
		/////////////////////////////////////////////
		//Create Resource
		m_Mesh = new Mesh(m_GraphicDevice, m_Name);
		
		/////////////////////////////////////////////
		//Set File Name and Load
		m_Mesh->SetFileName(m_FilePath);
				
		XETODO("Check return value");
		m_Mesh->Load();

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Mesh, m_FilePath);
		if(ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_Mesh);

			return XEResult::ResourceManagedFailed;
		}
	}

	return XEResult::Ok;
}