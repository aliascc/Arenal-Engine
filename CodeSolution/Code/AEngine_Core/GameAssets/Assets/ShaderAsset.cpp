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
#include "ShaderAsset.h"
#include "GraphicDevice.h"
#include "Shaders\Shader.h"
#include "Base\BaseFunctions.h"
#include "Shaders\HullShader.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Shaders\DomainShader.h"
#include "Shaders\ComputeShader.h"
#include "Shaders\GeometryShader.h"
#include "GameObject\GameObjectManager.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check if this class needs a mutex");
ShaderAsset::ShaderAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, ShaderType shaderType, GraphicDevice* graphicDevice)
	: GameAsset(GameContentType::Shader, filePath, gameResourceManager)
	, m_ShaderType(shaderType)
	, m_GraphicDevice(graphicDevice)
{
	AEAssert(m_GraphicDevice != nullptr);
}

ShaderAsset::~ShaderAsset()
{
	AERelease(m_Shader);
}

Shader* ShaderAsset::GetShaderReference()
{
	if(m_Shader == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Shader*>(m_Shader->AddRef());
}

AEResult ShaderAsset::LoadAssetResource()
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

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

	if(m_Shader != nullptr)
	{
		AETODO("Need do see what happens if subtype changes");
		AETODO("Check return value");
		ret = m_Shader->Load();
	}
	else
	{
		GameResourceType gameResourceType = GameResourceType::Unknown;

		switch (m_ShaderType)
		{
			case ShaderType::VertexShader:
				gameResourceType = GameResourceType::VertexShader;
				break;
			case ShaderType::PixelShader:
				gameResourceType = GameResourceType::PixelShader;
				break;
			case ShaderType::GeometryShader:
				gameResourceType = GameResourceType::GeometryShader;
				break;
			case ShaderType::HullShader:
				gameResourceType = GameResourceType::HullShader;
				break;
			case ShaderType::ComputeShader:
				gameResourceType = GameResourceType::ComputeShader;
				break;
			case ShaderType::DomainShader:
				gameResourceType = GameResourceType::DomainShader;
				break;
			default:
				AEAssert(false);
				return AEResult::InvalidShaderType;
				break;
		}

		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Shader = (Shader*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, gameResourceType);

		if(m_Shader != nullptr)
		{
			AETODO("Check if we always need to reload");
			AETODO("Check return value");
			m_Shader->Load();

			return AEResult::Ok;
		}

		/////////////////////////////////////////////
		//Create Resource
		switch (m_ShaderType)
		{
			case ShaderType::VertexShader:
				m_Shader = new VertexShader(m_GraphicDevice, m_Name);
				break;
			case ShaderType::PixelShader:
				m_Shader = new PixelShader(m_GraphicDevice, m_Name);
				break;
			case ShaderType::GeometryShader:
				m_Shader = new GeometryShader(m_GraphicDevice, m_Name);
				break;
			case ShaderType::HullShader:
				m_Shader = new HullShader(m_GraphicDevice, m_Name);
				break;
			case ShaderType::ComputeShader:
				m_Shader = new ComputeShader(m_GraphicDevice, m_Name);
				break;
			case ShaderType::DomainShader:
				m_Shader = new DomainShader(m_GraphicDevice, m_Name);
				break;
			default:
				AEAssert(false);
				return AEResult::InvalidShaderType;
				break;
		}

		/////////////////////////////////////////////
		//Set File Name
		m_Shader->SetFileName(m_FilePath);

		/////////////////////////////////////////////
		//Load File
		ret = m_Shader->Load();
		if(ret != AEResult::Ok)
		{
			AETODO("Add log");

			AERelease(m_Shader);

			return AEResult::LoadFileFailed;
		}

		/////////////////////////////////////////////
		//Set Name
		this->SetName(m_Shader->GetName());

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Shader, m_FilePath);
		if(ret != AEResult::Ok)
		{
			AETODO("Add log");

			AERelease(m_Shader);

			return AEResult::ResourceManagedFailed;
		}
	}

	return AEResult::Ok;
}