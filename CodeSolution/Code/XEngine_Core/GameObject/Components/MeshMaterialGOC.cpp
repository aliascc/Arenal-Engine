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
#include "MeshMaterialGOC.h"
#include "Textures\Texture.h"
#include "Base\BaseFunctions.h"
#include "Shaders\HullShader.h"
#include "Shaders\PixelShader.h"
#include "Shaders\DomainShader.h"
#include "Shaders\VertexShader.h"
#include "GameAssets\GameAsset.h"
#include "Shaders\ComputeShader.h"
#include "Shaders\GeometryShader.h"
#include "GameAssets\Assets\ShaderAsset.h"
#include "GameAssets\Assets\TextureAsset.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Add mutex");
MeshMaterialGOC::MeshMaterialGOC(GameObject* gameObject, const std::wstring& name)
	: GameObjectComponent(gameObject, GameObjectComponentType::MeshMaterial)
	, m_Name(name)
{
}

MeshMaterialGOC::~MeshMaterialGOC()
{
	////////////////////////////////////
	//Delete Vertex Shader and Props
	RemoveVertexShaderAsset();

	////////////////////////////////////
	//Delete Pixel Shader and Props
	RemovePixelShaderAsset();

	////////////////////////////////////
	//Delete Geometry Shader and Props
	RemoveGeometryShaderAsset();

	////////////////////////////////////
	//Delete Compute Shader and Props
	RemoveComputeShaderAsset();

	////////////////////////////////////
	//Delete Domain Shader and Props
	RemoveDomainShaderAsset();

	////////////////////////////////////
	//Delete Hull Shader and Props
	RemoveHullShaderAsset();
}

template<class T>
std::wstring MeshMaterialGOC::GetShaderResourceNameTemplate(const GameObjectAssetPair<T>& gameAssetPair) const
{
	if(gameAssetPair.m_ResourceAsset != nullptr)
	{
		return gameAssetPair.m_ResourceAsset->GetName();
	}
	else
	{
		return L"";
	}
}

template<class T>
XEResult MeshMaterialGOC::SetShaderAssetTemplate(ShaderAsset* asset, ShaderType shaderType, GameObjectAssetPair<T>& gameAssetPair)
{
	XEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(asset->GetGameContentType() != GameContentType::Shader)
	{
		return XEResult::InvalidObjType;
	}

	if(asset->GetShaderType() != shaderType)
	{
		return XEResult::ShaderTypeMismatch;
	}

	if(gameAssetPair.m_ResourceAsset != nullptr)
	{
		XEResult ret = RemoveShaderAsset(shaderType);

		if(ret != XEResult::Ok)
		{
			XETODO("Log error");

			XETODO("Add better return code");
			return XEResult::Fail;
		}

		ret = DeleteShaderProperties(shaderType);

		if(ret != XEResult::Ok)
		{
			XETODO("Log error");

			XETODO("Add better return code");
			return XEResult::Fail;
		}
	}

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = asset->RegisterEventHandlers(	callerID,
													std::bind(&MeshMaterialGOC::ShaderAssetDeletion, this, std::placeholders::_1),
													std::bind(&MeshMaterialGOC::ShaderAssetReload, this, std::placeholders::_1)
												);
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	gameAssetPair.m_AssetID						= asset->GetUniqueAssetID();
	gameAssetPair.m_CallerID					= callerID;
	gameAssetPair.m_ResourceAsset				= reinterpret_cast<T*>(asset->GetShaderReference());
	gameAssetPair.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();

	ret = CreateShaderProperties(shaderType);
	if(ret != XEResult::Ok)
	{
		RemoveShaderAsset(shaderType);

		XETODO("Log error");

		return ret;
	}

	return XEResult::Ok;
}

template<class T>
XEResult MeshMaterialGOC::ClearShaderAssetTemplate(GameObjectAssetPair<T>& gameAssetPair, bool informGameAsset)
{
	if(gameAssetPair.m_ResourceAsset != nullptr)
	{
		if(gameAssetPair.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
		{
			gameAssetPair.m_OnListenerObjDeletionEvent(gameAssetPair.m_CallerID);
		}

		gameAssetPair.m_AssetID						= 0;
		gameAssetPair.m_CallerID					= 0;
		gameAssetPair.m_OnListenerObjDeletionEvent	= nullptr;

		XERelease(gameAssetPair.m_ResourceAsset);
	}

	return XEResult::Ok;
}

XEResult MeshMaterialGOC::RemoveShaderAsset(ShaderType shaderType, bool informGameAsset, uint64_t assetID)
{
	switch (shaderType)
	{
		case ShaderType::VertexShader:
			if(assetID != 0 && m_VertexShader.m_AssetID != assetID)
			{
				XETODO("Add better return code");
				return XEResult::NotFound;
			}
			return ClearShaderAssetTemplate<VertexShader>(m_VertexShader, informGameAsset);

		case ShaderType::PixelShader:
			if(assetID != 0 && m_PixelShader.m_AssetID != assetID)
			{
				XETODO("Add better return code");
				return XEResult::NotFound;
			}
			return ClearShaderAssetTemplate<PixelShader>(m_PixelShader, informGameAsset);

		case ShaderType::GeometryShader:
			if(assetID != 0 && m_GeometryShader.m_AssetID != assetID)
			{
				XETODO("Add better return code");
				return XEResult::NotFound;
			}
			return ClearShaderAssetTemplate<GeometryShader>(m_GeometryShader, informGameAsset);

		case ShaderType::HullShader:
			if(assetID != 0 && m_HullShader.m_AssetID != assetID)
			{
				XETODO("Add better return code");
				return XEResult::NotFound;
			}
			return ClearShaderAssetTemplate<HullShader>(m_HullShader, informGameAsset);

		case ShaderType::ComputeShader:
			if(assetID != 0 && m_ComputeShader.m_AssetID != assetID)
			{
				XETODO("Add better return code");
				return XEResult::NotFound;
			}
			return ClearShaderAssetTemplate<ComputeShader>(m_ComputeShader, informGameAsset);

		case ShaderType::DomainShader:
			if(assetID != 0 && m_DomainShader.m_AssetID != assetID)
			{
				XETODO("Add better return code");
				return XEResult::NotFound;
			}
			return ClearShaderAssetTemplate<DomainShader>(m_DomainShader, informGameAsset);

		default:
			XEAssert(false);
			return XEResult::InvalidShaderType;
	}
}

template<class T>
XEResult MeshMaterialGOC::CreateShaderPropertiesTemplate(GameObjectAssetPair<T>& gameAssetPair, ShaderProperties** shaderProperties)
{
	XEAssert(gameAssetPair.m_ResourceAsset != nullptr);
	if(gameAssetPair.m_ResourceAsset == nullptr)
	{
		return XEResult::NullObj;
	}

	XEAssert(shaderProperties != nullptr);
	if(shaderProperties == nullptr)
	{
		return XEResult::NullParameter;
	}

	ShaderProperties* tempShaderProps = new ShaderProperties(gameAssetPair.m_ResourceAsset->GetShaderType(), gameAssetPair.m_ResourceAsset->GetGraphicDevice());

	XEResult ret = tempShaderProps->CreateFromShaderSignatures(gameAssetPair.m_ResourceAsset);

	if(ret != XEResult::Ok)
	{
		DeleteMem(tempShaderProps);

		XETODO("Add Log");

		return ret;
	}

	*shaderProperties = tempShaderProps;

	return XEResult::Ok;
}

template<class T>
XEResult MeshMaterialGOC::ShaderPropertiesReloadTemplate(GameObjectAssetPair<T>& gameAssetPair, ShaderProperties** shaderProperties)
{
	XEAssert(gameAssetPair.m_ResourceAsset != nullptr);
	if(gameAssetPair.m_ResourceAsset == nullptr)
	{
		return XEResult::NullObj;
	}

	XEAssert(shaderProperties != nullptr);
	XEAssert(*shaderProperties != nullptr);
	if(shaderProperties == nullptr || *shaderProperties == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	ShaderProperties* newShaderProps = new ShaderProperties(gameAssetPair.m_ResourceAsset->GetShaderType(), gameAssetPair.m_ResourceAsset->GetGraphicDevice());

	ret = newShaderProps->CreateFromShaderSignatures(gameAssetPair.m_ResourceAsset);
	if(ret != XEResult::Ok)
	{
		DeleteMem(newShaderProps);

		XETODO("Log error");

		XETODO("add better return code");
		return XEResult::Fail;
	}
		
	ret = newShaderProps->CopyVariableData(*shaderProperties);
	if(ret != XEResult::Ok)
	{
		DeleteMem(newShaderProps);

		XETODO("Log error");

		XETODO("add better return code");
		return XEResult::Fail;
	}
	
	DeleteMem(*shaderProperties);

	*shaderProperties = newShaderProps;

	return XEResult::Ok;
}

XEResult MeshMaterialGOC::CreateShaderProperties(ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::VertexShader:
			DeleteMem(m_VertexShaderProps);
			return CreateShaderPropertiesTemplate<VertexShader>(m_VertexShader, &m_VertexShaderProps);

		case ShaderType::PixelShader:
			DeleteMem(m_PixelShaderProps);
			return CreateShaderPropertiesTemplate<PixelShader>(m_PixelShader, &m_PixelShaderProps);

		case ShaderType::GeometryShader:
			DeleteMem(m_GeometryShaderProps);
			return CreateShaderPropertiesTemplate<GeometryShader>(m_GeometryShader, &m_GeometryShaderProps);

		case ShaderType::HullShader:
			DeleteMem(m_HullShaderProps);
			return CreateShaderPropertiesTemplate<HullShader>(m_HullShader, &m_HullShaderProps);

		case ShaderType::ComputeShader:
			DeleteMem(m_ComputeShaderProps);
			return CreateShaderPropertiesTemplate<ComputeShader>(m_ComputeShader, &m_ComputeShaderProps);

		case ShaderType::DomainShader:
			DeleteMem(m_DomainShaderProps);
			return CreateShaderPropertiesTemplate<DomainShader>(m_DomainShader, &m_DomainShaderProps);

		default:
			XEAssert(false);
			return XEResult::InvalidShaderType;
	}

	return XEResult::Ok;
}

XEResult MeshMaterialGOC::DeleteShaderProperties(ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::VertexShader:
			DeleteMem(m_VertexShaderProps);
			break;

		case ShaderType::PixelShader:
			DeleteMem(m_PixelShaderProps);
			break;

		case ShaderType::GeometryShader:
			DeleteMem(m_GeometryShaderProps);
			break;

		case ShaderType::HullShader:
			DeleteMem(m_HullShaderProps);
			break;

		case ShaderType::ComputeShader:
			DeleteMem(m_ComputeShaderProps);
			break;

		case ShaderType::DomainShader:
			DeleteMem(m_DomainShaderProps);
			break;

		default:
			XEAssert(false);
			return XEResult::InvalidShaderType;
	}

	return XEResult::Ok;
}

void MeshMaterialGOC::ShaderAssetDeletion(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Shader);
	if(asset->GetGameContentType() != GameContentType::Shader)
	{
		return;
	}

	ShaderAsset* shaderAsset = reinterpret_cast<ShaderAsset*>(asset);

	ShaderType shaderType = shaderAsset->GetShaderType();

	RemoveShaderAsset(shaderType, false, shaderAsset->GetUniqueAssetID());
}

void MeshMaterialGOC::ShaderAssetReload(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return;
	}
	
	XEAssert(asset->GetGameContentType() == GameContentType::Shader);
	if(asset->GetGameContentType() != GameContentType::Shader)
	{
		return;
	}

	ShaderAsset* shaderAsset = reinterpret_cast<ShaderAsset*>(asset);

	ShaderType shaderType = shaderAsset->GetShaderType();

	switch (shaderType)
	{
		case ShaderType::VertexShader:
			ShaderPropertiesReloadTemplate<VertexShader>(m_VertexShader, &m_VertexShaderProps);
			break;
		case ShaderType::PixelShader:
			ShaderPropertiesReloadTemplate<PixelShader>(m_PixelShader, &m_PixelShaderProps);
			break;
		case ShaderType::GeometryShader:
			ShaderPropertiesReloadTemplate<GeometryShader>(m_GeometryShader, &m_GeometryShaderProps);
			break;
		case ShaderType::HullShader:
			ShaderPropertiesReloadTemplate<HullShader>(m_HullShader, &m_PixelShaderProps);
			break;
		case ShaderType::ComputeShader:
			ShaderPropertiesReloadTemplate<ComputeShader>(m_ComputeShader, &m_ComputeShaderProps);
			break;
		case ShaderType::DomainShader:
			ShaderPropertiesReloadTemplate<DomainShader>(m_DomainShader, &m_DomainShaderProps);
			break;
		default:
			XEAssert(false);
			break;
	}
}

/*********************
 *   Vertex Shader   *
 *********************/

std::wstring MeshMaterialGOC::GetVertexShaderName() const
{
	return GetShaderResourceNameTemplate<VertexShader>(m_VertexShader);
}

XEResult MeshMaterialGOC::SetVertexShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<VertexShader>(asset, ShaderType::VertexShader, m_VertexShader);
}

XEResult MeshMaterialGOC::RemoveVertexShaderAsset()
{
	DeleteMem(m_VertexShaderProps);
	return ClearShaderAssetTemplate<VertexShader>(m_VertexShader);
}

/********************
 *   Pixel Shader   *
 ********************/

std::wstring MeshMaterialGOC::GetPixelShaderName() const
{
	return GetShaderResourceNameTemplate<PixelShader>(m_PixelShader);
}

XEResult MeshMaterialGOC::SetPixelShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<PixelShader>(asset, ShaderType::PixelShader, m_PixelShader);
}

XEResult MeshMaterialGOC::RemovePixelShaderAsset()
{
	DeleteMem(m_PixelShaderProps);
	return ClearShaderAssetTemplate<PixelShader>(m_PixelShader);
}

/***********************
 *   Geometry Shader   *
 ***********************/

std::wstring MeshMaterialGOC::GetGeometryShaderName() const
{
	return GetShaderResourceNameTemplate<GeometryShader>(m_GeometryShader);
}

XEResult MeshMaterialGOC::SetGeometryShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<GeometryShader>(asset, ShaderType::GeometryShader, m_GeometryShader);
}

XEResult MeshMaterialGOC::RemoveGeometryShaderAsset()
{
	DeleteMem(m_GeometryShaderProps);
	return ClearShaderAssetTemplate<GeometryShader>(m_GeometryShader);
}

/**********************
 *   Compute Shader   *
 **********************/

std::wstring MeshMaterialGOC::GetComputeShaderName() const
{
	return GetShaderResourceNameTemplate<ComputeShader>(m_ComputeShader);
}

XEResult MeshMaterialGOC::SetComputeShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<ComputeShader>(asset, ShaderType::ComputeShader, m_ComputeShader);
}

XEResult MeshMaterialGOC::RemoveComputeShaderAsset()
{
	DeleteMem(m_ComputeShaderProps);
	return ClearShaderAssetTemplate<ComputeShader>(m_ComputeShader);
}

/*******************
 *   Hull Shader   *
 *******************/

std::wstring MeshMaterialGOC::GetHullShaderName() const
{
	return GetShaderResourceNameTemplate<HullShader>(m_HullShader);
}

XEResult MeshMaterialGOC::SetHullShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<HullShader>(asset, ShaderType::HullShader, m_HullShader);
}

XEResult MeshMaterialGOC::RemoveHullShaderAsset()
{
	DeleteMem(m_HullShaderProps);
	return ClearShaderAssetTemplate<HullShader>(m_HullShader);
}

/*********************
 *   Domain Shader   *
 *********************/

std::wstring MeshMaterialGOC::GetDomainShaderName() const
{
	return GetShaderResourceNameTemplate<DomainShader>(m_DomainShader);
}

XEResult MeshMaterialGOC::SetDomainShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<DomainShader>(asset, ShaderType::DomainShader, m_DomainShader);
}

XEResult MeshMaterialGOC::RemoveDomainShaderAsset()
{
	DeleteMem(m_DomainShaderProps);
	return ClearShaderAssetTemplate<DomainShader>(m_DomainShader);
}

/*******************
 *   All Shaders   *
 *******************/

XEResult MeshMaterialGOC::ApplyShaders(GraphicDevice* graphicDevice)
{
	XEAssert(graphicDevice != nullptr);
	if(graphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XETODO("Check returns for all");
	XETODO("mutex in adding creating MeshGOC");

	if(m_VertexShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetVertexShader(m_VertexShader.m_ResourceAsset);
		m_VertexShaderProps->ApplyAll();
	}

	if(m_PixelShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetPixelShader(m_PixelShader.m_ResourceAsset);
		m_PixelShaderProps->ApplyAll();
	}

	if(m_GeometryShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetGeometryShader(m_GeometryShader.m_ResourceAsset);
		m_GeometryShaderProps->ApplyAll();
	}

	if(m_ComputeShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetComputeShader(m_ComputeShader.m_ResourceAsset);
		m_ComputeShaderProps->ApplyAll();
	}

	if(m_DomainShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetDomainShader(m_DomainShader.m_ResourceAsset);
		m_DomainShaderProps->ApplyAll();
	}

	if(m_HullShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetHullShader(m_HullShader.m_ResourceAsset);
		m_HullShaderProps->ApplyAll();
	}

	return XEResult::Ok;
}

XEResult MeshMaterialGOC::UnApplyShaders(GraphicDevice* graphicDevice)
{
	XEAssert(graphicDevice != nullptr);
	if(graphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XETODO("Check returns for all");
	XETODO("mutex in adding creating MeshGOC");

	if(m_VertexShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetVertexShader(nullptr);
		m_VertexShaderProps->UnApplyAll();
	}

	if(m_PixelShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetPixelShader(nullptr);
		m_PixelShaderProps->UnApplyAll();
	}

	if(m_GeometryShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetGeometryShader(nullptr);
		m_GeometryShaderProps->UnApplyAll();
	}

	if(m_ComputeShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetComputeShader(nullptr);
		m_ComputeShaderProps->UnApplyAll();
	}

	if(m_DomainShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetDomainShader(nullptr);
		m_DomainShaderProps->UnApplyAll();
	}

	if(m_HullShader.m_ResourceAsset != nullptr)
	{
		graphicDevice->SetHullShader(nullptr);
		m_HullShaderProps->UnApplyAll();
	}

	return XEResult::Ok;
}