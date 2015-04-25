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
	XEAssert(m_GraphicDevice != nullptr);
	XEAssert(m_GameResourceManager != nullptr);
}

Material::~Material()
{
	CleanUp();
}

void Material::CleanUp()
{
	XERelease(m_VertexShader);
	DeleteMem(m_VSProps);

	XERelease(m_PixelShader);
	DeleteMem(m_PSProps);

	XERelease(m_GeometryShader);
	DeleteMem(m_GSProps);

	XERelease(m_DomainShader);
	DeleteMem(m_DSProps);

	XERelease(m_HullShader);
	DeleteMem(m_HSProps);

	XERelease(m_ComputeShader);
	DeleteMem(m_CSProps);
}

void Material::SetVertexShader(VertexShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		XERelease(m_VertexShader);
	}

	m_VertexShader = shader;
}

void Material::SetPixelShader(PixelShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		XERelease(m_PixelShader);
	}

	m_PixelShader = shader;
}

void Material::SetGeometryShader(GeometryShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		XERelease(m_GeometryShader);
	}

	m_GeometryShader = shader;
}

void Material::SetDomainShader(DomainShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		XERelease(m_DomainShader);
	}

	m_DomainShader = shader;
}

void Material::SetHullShader(HullShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		XERelease(m_HullShader);
	}

	m_HullShader = shader;
}

void Material::SetComputeShader(ComputeShader* shader, bool releaseOld)
{
	if(releaseOld)
	{
		XERelease(m_ComputeShader);
	}

	m_ComputeShader = shader;
}

XEResult Material::Apply()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;
	
	/***********************
	*Vertex Shader
	***********************/
	ret = m_GraphicDevice->SetVertexShader(m_VertexShader);

	if(ret != XEResult::Ok)
	{
		return XEResult::SetVertexShaderFailed;
	}

	if(m_VertexShader != nullptr && m_VSProps != nullptr)
	{
		ret = m_VSProps->ApplyAll();

		if(ret != XEResult::Ok)
		{
			return XEResult::SetVertexShaderPropsFailed;
		}
	}

	/***********************
	*Pixel Shader
	***********************/
	ret = m_GraphicDevice->SetPixelShader(m_PixelShader);

	if(ret != XEResult::Ok)
	{
		return XEResult::SetPixelShaderFailed;
	}

	if(m_PixelShader != nullptr && m_PSProps != nullptr)
	{
		ret = m_PSProps->ApplyAll();

		if(ret != XEResult::Ok)
		{
			return XEResult::SetPixelShaderPropsFailed;
		}
	}
	
	/***********************
	*Geometry Shader
	***********************/
	ret = m_GraphicDevice->SetGeometryShader(m_GeometryShader);

	if(ret != XEResult::Ok)
	{
		return XEResult::SetGeometryShaderFailed;
	}

	if(m_GeometryShader != nullptr && m_GSProps != nullptr)
	{
		ret = m_GSProps->ApplyAll();

		if(ret != XEResult::Ok)
		{
			return XEResult::SetGeometryShaderPropsFailed;
		}
	}
	
	/***********************
	*Domain Shader
	***********************/
	ret = m_GraphicDevice->SetDomainShader(m_DomainShader);

	if(ret != XEResult::Ok)
	{
		return XEResult::SetDomainShaderFailed;
	}

	if(m_DomainShader != nullptr && m_DSProps != nullptr)
	{
		ret = m_DSProps->ApplyAll();

		if(ret != XEResult::Ok)
		{
			return XEResult::SetDomainShaderPropsFailed;
		}
	}
	
	/***********************
	*Hull Shader
	***********************/
	ret = m_GraphicDevice->SetHullShader(m_HullShader);

	if(ret != XEResult::Ok)
	{
		return XEResult::SetHullShaderFailed;
	}

	if(m_HullShader != nullptr && m_HSProps != nullptr)
	{
		ret = m_HSProps->ApplyAll();

		if(ret != XEResult::Ok)
		{
			return XEResult::SetHullShaderPropsFailed;
		}
	}
	
	/***********************
	*Compute Shader
	***********************/
	ret = m_GraphicDevice->SetComputeShader(m_ComputeShader);

	if(ret != XEResult::Ok)
	{
		return XEResult::SetComputeShaderFailed;
	}

	if(m_ComputeShader != nullptr && m_CSProps != nullptr)
	{
		ret = m_CSProps->ApplyAll();

		if(ret != XEResult::Ok)
		{
			return XEResult::SetComputeShaderPropsFailed;
		}
	}
	
	/***********************
	*Finish
	***********************/
	return XEResult::Ok;
}

XEResult Material::UnApply()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;
	
	/***********************
	*Vertex Shader
	***********************/
	if(m_VertexShader != nullptr)
	{
		ret = m_GraphicDevice->SetVertexShader(nullptr);

		if(ret != XEResult::Ok)
		{
			return XEResult::SetVertexShaderFailed;
		}

		if(m_VSProps != nullptr)
		{
			ret = m_VSProps->UnApplyAll();

			if(ret != XEResult::Ok)
			{
				return XEResult::SetVertexShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Pixel Shader
	***********************/
	if(m_PixelShader != nullptr)
	{
		ret = m_GraphicDevice->SetPixelShader(nullptr);

		if(ret != XEResult::Ok)
		{
			return XEResult::SetPixelShaderFailed;
		}

		if(m_PSProps != nullptr)
		{
			ret = m_PSProps->UnApplyAll();

			if(ret != XEResult::Ok)
			{
				return XEResult::SetPixelShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Geometry Shader
	***********************/
	if(m_GeometryShader != nullptr)
	{
		ret = m_GraphicDevice->SetGeometryShader(nullptr);

		if(ret != XEResult::Ok)
		{
			return XEResult::SetGeometryShaderFailed;
		}

		if(m_GSProps != nullptr)
		{
			ret = m_GSProps->UnApplyAll();

			if(ret != XEResult::Ok)
			{
				return XEResult::SetGeometryShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Domain Shader
	***********************/
	if(m_DomainShader != nullptr)
	{
		ret = m_GraphicDevice->SetDomainShader(nullptr);

		if(ret != XEResult::Ok)
		{
			return XEResult::SetDomainShaderFailed;
		}

		if(m_DSProps != nullptr)
		{
			ret = m_DSProps->UnApplyAll();

			if(ret != XEResult::Ok)
			{
				return XEResult::SetDomainShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Hull Shader
	***********************/
	if(m_HullShader != nullptr)
	{
		ret = m_GraphicDevice->SetHullShader(nullptr);

		if(ret != XEResult::Ok)
		{
			return XEResult::SetHullShaderFailed;
		}

		if(m_HSProps != nullptr)
		{
			ret = m_HSProps->UnApplyAll();

			if(ret != XEResult::Ok)
			{
				return XEResult::SetHullShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Compute Shader
	***********************/
	if(m_ComputeShader != nullptr)
	{
		ret = m_GraphicDevice->SetComputeShader(nullptr);

		if(ret != XEResult::Ok)
		{
			return XEResult::SetComputeShaderFailed;
		}

		if(m_CSProps != nullptr)
		{
			ret = m_CSProps->UnApplyAll();

			if(ret != XEResult::Ok)
			{
				return XEResult::SetComputeShaderPropsFailed;
			}
		}
	}
	
	/***********************
	*Finish
	***********************/
	return XEResult::Ok;
}