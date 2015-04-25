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
XETODO("Check if this class needs a mutex");
ShaderAsset::ShaderAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, ShaderType shaderType, GraphicDevice* graphicDevice)
	: GameAsset(GameContentType::Shader, filePath, gameResourceManager)
	, m_ShaderType(shaderType)
	, m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);
}

ShaderAsset::~ShaderAsset()
{
	XERelease(m_Shader);
}

Shader* ShaderAsset::GetShaderReference()
{
	if(m_Shader == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Shader*>(m_Shader->AddRef());
}

XEResult ShaderAsset::LoadAssetResource()
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

	if(m_Shader != nullptr)
	{
		XETODO("Need do see what happens if subtype changes");
		XETODO("Check return value");
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
				XEAssert(false);
				return XEResult::InvalidShaderType;
				break;
		}

		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Shader = (Shader*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, gameResourceType);

		if(m_Shader != nullptr)
		{
			XETODO("Check if we always need to reload");
			XETODO("Check return value");
			m_Shader->Load();

			return XEResult::Ok;
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
				XEAssert(false);
				return XEResult::InvalidShaderType;
				break;
		}

		/////////////////////////////////////////////
		//Set File Name
		m_Shader->SetFileName(m_FilePath);

		/////////////////////////////////////////////
		//Load File
		ret = m_Shader->Load();
		if(ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_Shader);

			return XEResult::LoadFileFailed;
		}

		/////////////////////////////////////////////
		//Set Name
		this->SetName(m_Shader->GetName());

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Shader, m_FilePath);
		if(ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_Shader);

			return XEResult::ResourceManagedFailed;
		}
	}

	return XEResult::Ok;
}