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
#include "Material.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Shaders\HullShader.h"
#include "Shaders\PixelShader.h"
#include "Shaders\DomainShader.h"
#include "Shaders\VertexShader.h"
#include "Shaders\ComputeShader.h"
#include "Shaders\GeometryShader.h"
#include "Resource\GameResourceManager.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Material::Material(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name)
	: Named(name)
	, m_GraphicDevice(graphicDevice)
	, m_GameResourceManager(gameResourceManager)
{
	AEAssert(m_GraphicDevice != nullptr);
	AEAssert(m_GameResourceManager != nullptr);
}

Material::~Material()
{
	CleanUp();
}

void Material::CleanUp()
{
	AERelease(m_VertexShader);
	DeleteMem(m_VSProps);

	AERelease(m_PixelShader);
	DeleteMem(m_PSProps);

	AERelease(m_GeometryShader);
	DeleteMem(m_GSProps);

	AERelease(m_DomainShader);
	DeleteMem(m_DSProps);

	AERelease(m_HullShader);
	DeleteMem(m_HSProps);

	AERelease(m_ComputeShader);
	DeleteMem(m_CSProps);
}

void Material::SetVertexShader(VertexShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		AERelease(m_VertexShader);
	}

	m_VertexShader = shader;
}

void Material::SetPixelShader(PixelShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		AERelease(m_PixelShader);
	}

	m_PixelShader = shader;
}

void Material::SetGeometryShader(GeometryShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		AERelease(m_GeometryShader);
	}

	m_GeometryShader = shader;
}

void Material::SetDomainShader(DomainShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		AERelease(m_DomainShader);
	}

	m_DomainShader = shader;
}

void Material::SetHullShader(HullShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		AERelease(m_HullShader);
	}

	m_HullShader = shader;
}

void Material::SetComputeShader(ComputeShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		AERelease(m_ComputeShader);
	}

	m_ComputeShader = shader;
}

AEResult Material::Apply()
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;
	
	/***********************
	*Vertex Shader
	***********************/
	ret = m_GraphicDevice->SetVertexShader(m_VertexShader);

	if(ret != AEResult::Ok)
	{
		return AEResult::SetVertexShaderFailed;
	}

	if(m_VertexShader != nullptr && m_VSProps != nullptr)
	{
		ret = m_VSProps->ApplyAll();

		if(ret != AEResult::Ok)
		{
			return AEResult::SetVertexShaderPropsFailed;
		}
	}

	/***********************
	*Pixel Shader
	***********************/
	ret = m_GraphicDevice->SetPixelShader(m_PixelShader);

	if(ret != AEResult::Ok)
	{
		return AEResult::SetPixelShaderFailed;
	}

	if(m_PixelShader != nullptr && m_PSProps != nullptr)
	{
		ret = m_PSProps->ApplyAll();

		if(ret != AEResult::Ok)
		{
			return AEResult::SetPixelShaderPropsFailed;
		}
	}
	
	/***********************
	*Geometry Shader
	***********************/
	ret = m_GraphicDevice->SetGeometryShader(m_GeometryShader);

	if(ret != AEResult::Ok)
	{
		return AEResult::SetGeometryShaderFailed;
	}

	if(m_GeometryShader != nullptr && m_GSProps != nullptr)
	{
		ret = m_GSProps->ApplyAll();

		if(ret != AEResult::Ok)
		{
			return AEResult::SetGeometryShaderPropsFailed;
		}
	}
	
	/***********************
	*Domain Shader
	***********************/
	ret = m_GraphicDevice->SetDomainShader(m_DomainShader);

	if(ret != AEResult::Ok)
	{
		return AEResult::SetDomainShaderFailed;
	}

	if(m_DomainShader != nullptr && m_DSProps != nullptr)
	{
		ret = m_DSProps->ApplyAll();

		if(ret != AEResult::Ok)
		{
			return AEResult::SetDomainShaderPropsFailed;
		}
	}
	
	/***********************
	*Hull Shader
	***********************/
	ret = m_GraphicDevice->SetHullShader(m_HullShader);

	if(ret != AEResult::Ok)
	{
		return AEResult::SetHullShaderFailed;
	}

	if(m_HullShader != nullptr && m_HSProps != nullptr)
	{
		ret = m_HSProps->ApplyAll();

		if(ret != AEResult::Ok)
		{
			return AEResult::SetHullShaderPropsFailed;
		}
	}
	
	/***********************
	*Compute Shader
	***********************/
	ret = m_GraphicDevice->SetComputeShader(m_ComputeShader);

	if(ret != AEResult::Ok)
	{
		return AEResult::SetComputeShaderFailed;
	}

	if(m_ComputeShader != nullptr && m_CSProps != nullptr)
	{
		ret = m_CSProps->ApplyAll();

		if(ret != AEResult::Ok)
		{
			return AEResult::SetComputeShaderPropsFailed;
		}
	}
	
	/***********************
	*Finish
	***********************/
	return AEResult::Ok;
}

AEResult Material::UnApply()
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;
	
	/***********************
	*Vertex Shader
	***********************/
	if(m_VertexShader != nullptr)
	{
		ret = m_GraphicDevice->SetVertexShader(nullptr);

		if(ret != AEResult::Ok)
		{
			return AEResult::SetVertexShaderFailed;
		}

		if(m_VSProps != nullptr)
		{
			ret = m_VSProps->UnApplyAll();

			if(ret != AEResult::Ok)
			{
				return AEResult::SetVertexShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Pixel Shader
	***********************/
	if(m_PixelShader != nullptr)
	{
		ret = m_GraphicDevice->SetPixelShader(nullptr);

		if(ret != AEResult::Ok)
		{
			return AEResult::SetPixelShaderFailed;
		}

		if(m_PSProps != nullptr)
		{
			ret = m_PSProps->UnApplyAll();

			if(ret != AEResult::Ok)
			{
				return AEResult::SetPixelShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Geometry Shader
	***********************/
	if(m_GeometryShader != nullptr)
	{
		ret = m_GraphicDevice->SetGeometryShader(nullptr);

		if(ret != AEResult::Ok)
		{
			return AEResult::SetGeometryShaderFailed;
		}

		if(m_GSProps != nullptr)
		{
			ret = m_GSProps->UnApplyAll();

			if(ret != AEResult::Ok)
			{
				return AEResult::SetGeometryShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Domain Shader
	***********************/
	if(m_DomainShader != nullptr)
	{
		ret = m_GraphicDevice->SetDomainShader(nullptr);

		if(ret != AEResult::Ok)
		{
			return AEResult::SetDomainShaderFailed;
		}

		if(m_DSProps != nullptr)
		{
			ret = m_DSProps->UnApplyAll();

			if(ret != AEResult::Ok)
			{
				return AEResult::SetDomainShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Hull Shader
	***********************/
	if(m_HullShader != nullptr)
	{
		ret = m_GraphicDevice->SetHullShader(nullptr);

		if(ret != AEResult::Ok)
		{
			return AEResult::SetHullShaderFailed;
		}

		if(m_HSProps != nullptr)
		{
			ret = m_HSProps->UnApplyAll();

			if(ret != AEResult::Ok)
			{
				return AEResult::SetHullShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Compute Shader
	***********************/
	if(m_ComputeShader != nullptr)
	{
		ret = m_GraphicDevice->SetComputeShader(nullptr);

		if(ret != AEResult::Ok)
		{
			return AEResult::SetComputeShaderFailed;
		}

		if(m_CSProps != nullptr)
		{
			ret = m_CSProps->UnApplyAll();

			if(ret != AEResult::Ok)
			{
				return AEResult::SetComputeShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Finish
	***********************/
	return AEResult::Ok;
}