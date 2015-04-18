/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
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
#include "MeshGOC.h"
#include "Models\Mesh.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\Assets\MeshAsset.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Add mutex");
MeshGOC::MeshGOC(GameObject* gameObject)
	: GameObjectComponent(gameObject, GameObjectComponentType::Mesh)
{
}

MeshGOC::~MeshGOC()
{
	RemoveMeshAsset();
}

void MeshGOC::MeshAssetDeletion(GameAsset* asset)
{
	XEAssert(asset != nullptr);

	if(asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Mesh);
	if(asset->GetGameContentType() != GameContentType::Mesh)
	{
		return;
	}

	if(m_Mesh.m_AssetID != asset->GetUniqueAssetID())
	{
		return;
	}

	ClearMeshAsset(false);
}

XEResult MeshGOC::SetMeshAsset(MeshAsset* asset)
{
	XEAssert(asset != nullptr);

	if(asset == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(asset->GetGameContentType() != GameContentType::Mesh)
	{
		return XEResult::InvalidObjType;
	}

	if(m_Mesh.m_ResourceAsset != nullptr)
	{
		XEResult ret = RemoveMeshAsset();

		if(ret != XEResult::Ok)
		{
			XETODO("Log error");

			XETODO("Add better return code");
			return XEResult::Fail;
		}
	}

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&MeshGOC::MeshAssetDeletion, this, std::placeholders::_1));
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	m_Mesh.m_AssetID					= asset->GetUniqueAssetID();
	m_Mesh.m_CallerID					= callerID;
	m_Mesh.m_ResourceAsset				= asset->GetMeshReference();
	m_Mesh.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();

	return XEResult::Ok;
}

XEResult MeshGOC::RemoveMeshAsset()
{
	return ClearMeshAsset(true);
}

XEResult MeshGOC::ClearMeshAsset(bool informGameAsset)
{
	if(m_Mesh.m_ResourceAsset != nullptr)
	{
		if(m_Mesh.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
		{
			m_Mesh.m_OnListenerObjDeletionEvent(m_Mesh.m_CallerID);
		}

		m_Mesh.m_AssetID					= 0;
		m_Mesh.m_CallerID					= 0;
		m_Mesh.m_OnListenerObjDeletionEvent	= nullptr;

		XERelease(m_Mesh.m_ResourceAsset);
	}

	return XEResult::Ok;
}

std::wstring MeshGOC::GetMeshName() const
{
	if(m_Mesh.m_ResourceAsset != nullptr)
	{
		return m_Mesh.m_ResourceAsset->GetName();
	}
	else
	{
		return L"";
	}
}