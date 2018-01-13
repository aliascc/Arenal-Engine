/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

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
AETODO("Add mutex");
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
AEResult MeshMaterialGOC::SetShaderAssetTemplate(ShaderAsset* asset, ShaderType shaderType, GameObjectAssetPair<T>& gameAssetPair)
{
	AEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return AEResult::NullParameter;
	}

	if(asset->GetGameContentType() != GameContentType::Shader)
	{
		return AEResult::InvalidObjType;
	}

	if(asset->GetShaderType() != shaderType)
	{
		return AEResult::ShaderTypeMismatch;
	}

	if(gameAssetPair.m_ResourceAsset != nullptr)
	{
		AEResult ret = RemoveShaderAsset(shaderType);

		if(ret != AEResult::Ok)
		{
			AETODO("Log error");

			AETODO("Add better return code");
			return AEResult::Fail;
		}

		ret = DeleteShaderProperties(shaderType);

		if(ret != AEResult::Ok)
		{
			AETODO("Log error");

			AETODO("Add better return code");
			return AEResult::Fail;
		}
	}

	uint64_t callerID = AE_Base::GetNextUniqueID();

	AEResult ret = asset->RegisterEventHandlers(	callerID,
													std::bind(&MeshMaterialGOC::ShaderAssetDeletion, this, std::placeholders::_1),
													std::bind(&MeshMaterialGOC::ShaderAssetReload, this, std::placeholders::_1)
												);
	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	gameAssetPair.m_AssetID						= asset->GetUniqueAssetID();
	gameAssetPair.m_CallerID					= callerID;
	gameAssetPair.m_ResourceAsset				= reinterpret_cast<T*>(asset->GetShaderReference());
	gameAssetPair.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();

	ret = CreateShaderProperties(shaderType);
	if(ret != AEResult::Ok)
	{
		RemoveShaderAsset(shaderType);

		AETODO("Log error");

		return ret;
	}

	return AEResult::Ok;
}

template<class T>
AEResult MeshMaterialGOC::ClearShaderAssetTemplate(GameObjectAssetPair<T>& gameAssetPair, bool informGameAsset)
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

		AERelease(gameAssetPair.m_ResourceAsset);
	}

	return AEResult::Ok;
}

AEResult MeshMaterialGOC::RemoveShaderAsset(ShaderType shaderType, bool informGameAsset, uint64_t assetID)
{
	switch (shaderType)
	{
		case ShaderType::VertexShader:
			if(assetID != 0 && m_VertexShader.m_AssetID != assetID)
			{
				AETODO("Add better return code");
				return AEResult::NotFound;
			}
			return ClearShaderAssetTemplate<VertexShader>(m_VertexShader, informGameAsset);

		case ShaderType::PixelShader:
			if(assetID != 0 && m_PixelShader.m_AssetID != assetID)
			{
				AETODO("Add better return code");
				return AEResult::NotFound;
			}
			return ClearShaderAssetTemplate<PixelShader>(m_PixelShader, informGameAsset);

		case ShaderType::GeometryShader:
			if(assetID != 0 && m_GeometryShader.m_AssetID != assetID)
			{
				AETODO("Add better return code");
				return AEResult::NotFound;
			}
			return ClearShaderAssetTemplate<GeometryShader>(m_GeometryShader, informGameAsset);

		case ShaderType::HullShader:
			if(assetID != 0 && m_HullShader.m_AssetID != assetID)
			{
				AETODO("Add better return code");
				return AEResult::NotFound;
			}
			return ClearShaderAssetTemplate<HullShader>(m_HullShader, informGameAsset);

		case ShaderType::ComputeShader:
			if(assetID != 0 && m_ComputeShader.m_AssetID != assetID)
			{
				AETODO("Add better return code");
				return AEResult::NotFound;
			}
			return ClearShaderAssetTemplate<ComputeShader>(m_ComputeShader, informGameAsset);

		case ShaderType::DomainShader:
			if(assetID != 0 && m_DomainShader.m_AssetID != assetID)
			{
				AETODO("Add better return code");
				return AEResult::NotFound;
			}
			return ClearShaderAssetTemplate<DomainShader>(m_DomainShader, informGameAsset);

		default:
			AEAssert(false);
			return AEResult::InvalidShaderType;
	}
}

template<class T>
AEResult MeshMaterialGOC::CreateShaderPropertiesTemplate(GameObjectAssetPair<T>& gameAssetPair, ShaderProperties** shaderProperties)
{
	AEAssert(gameAssetPair.m_ResourceAsset != nullptr);
	if(gameAssetPair.m_ResourceAsset == nullptr)
	{
		return AEResult::NullObj;
	}

	AEAssert(shaderProperties != nullptr);
	if(shaderProperties == nullptr)
	{
		return AEResult::NullParameter;
	}

	ShaderProperties* tempShaderProps = new ShaderProperties(gameAssetPair.m_ResourceAsset->GetShaderType(), gameAssetPair.m_ResourceAsset->GetGraphicDevice());

	AEResult ret = tempShaderProps->CreateFromShaderSignatures(gameAssetPair.m_ResourceAsset);

	if(ret != AEResult::Ok)
	{
		DeleteMem(tempShaderProps);

		AETODO("Add Log");

		return ret;
	}

	*shaderProperties = tempShaderProps;

	return AEResult::Ok;
}

template<class T>
AEResult MeshMaterialGOC::ShaderPropertiesReloadTemplate(GameObjectAssetPair<T>& gameAssetPair, ShaderProperties** shaderProperties)
{
	AEAssert(gameAssetPair.m_ResourceAsset != nullptr);
	if(gameAssetPair.m_ResourceAsset == nullptr)
	{
		return AEResult::NullObj;
	}

	AEAssert(shaderProperties != nullptr);
	AEAssert(*shaderProperties != nullptr);
	if(shaderProperties == nullptr || *shaderProperties == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEResult ret = AEResult::Ok;

	ShaderProperties* newShaderProps = new ShaderProperties(gameAssetPair.m_ResourceAsset->GetShaderType(), gameAssetPair.m_ResourceAsset->GetGraphicDevice());

	ret = newShaderProps->CreateFromShaderSignatures(gameAssetPair.m_ResourceAsset);
	if(ret != AEResult::Ok)
	{
		DeleteMem(newShaderProps);

		AETODO("Log error");

		AETODO("add better return code");
		return AEResult::Fail;
	}
		
	ret = newShaderProps->CopyVariableData(*shaderProperties);
	if(ret != AEResult::Ok)
	{
		DeleteMem(newShaderProps);

		AETODO("Log error");

		AETODO("add better return code");
		return AEResult::Fail;
	}
	
	DeleteMem(*shaderProperties);

	*shaderProperties = newShaderProps;

	return AEResult::Ok;
}

AEResult MeshMaterialGOC::CreateShaderProperties(ShaderType shaderType)
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
			AEAssert(false);
			return AEResult::InvalidShaderType;
	}

	return AEResult::Ok;
}

AEResult MeshMaterialGOC::DeleteShaderProperties(ShaderType shaderType)
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
			AEAssert(false);
			return AEResult::InvalidShaderType;
	}

	return AEResult::Ok;
}

void MeshMaterialGOC::ShaderAssetDeletion(GameAsset* asset)
{
	AEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return;
	}

	AEAssert(asset->GetGameContentType() == GameContentType::Shader);
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
	AEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return;
	}
	
	AEAssert(asset->GetGameContentType() == GameContentType::Shader);
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
			AEAssert(false);
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

AEResult MeshMaterialGOC::SetVertexShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<VertexShader>(asset, ShaderType::VertexShader, m_VertexShader);
}

AEResult MeshMaterialGOC::RemoveVertexShaderAsset()
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

AEResult MeshMaterialGOC::SetPixelShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<PixelShader>(asset, ShaderType::PixelShader, m_PixelShader);
}

AEResult MeshMaterialGOC::RemovePixelShaderAsset()
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

AEResult MeshMaterialGOC::SetGeometryShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<GeometryShader>(asset, ShaderType::GeometryShader, m_GeometryShader);
}

AEResult MeshMaterialGOC::RemoveGeometryShaderAsset()
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

AEResult MeshMaterialGOC::SetComputeShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<ComputeShader>(asset, ShaderType::ComputeShader, m_ComputeShader);
}

AEResult MeshMaterialGOC::RemoveComputeShaderAsset()
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

AEResult MeshMaterialGOC::SetHullShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<HullShader>(asset, ShaderType::HullShader, m_HullShader);
}

AEResult MeshMaterialGOC::RemoveHullShaderAsset()
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

AEResult MeshMaterialGOC::SetDomainShaderAsset(ShaderAsset* asset)
{
	return SetShaderAssetTemplate<DomainShader>(asset, ShaderType::DomainShader, m_DomainShader);
}

AEResult MeshMaterialGOC::RemoveDomainShaderAsset()
{
	DeleteMem(m_DomainShaderProps);
	return ClearShaderAssetTemplate<DomainShader>(m_DomainShader);
}

/*******************
 *   All Shaders   *
 *******************/

AEResult MeshMaterialGOC::ApplyShaders(GraphicDevice* graphicDevice)
{
	AEAssert(graphicDevice != nullptr);
	if(graphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AETODO("Check returns for all");
	AETODO("mutex in adding creating MeshGOC");

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

	return AEResult::Ok;
}

AEResult MeshMaterialGOC::UnApplyShaders(GraphicDevice* graphicDevice)
{
	AEAssert(graphicDevice != nullptr);
	if(graphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AETODO("Check returns for all");
	AETODO("mutex in adding creating MeshGOC");

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

	return AEResult::Ok;
}