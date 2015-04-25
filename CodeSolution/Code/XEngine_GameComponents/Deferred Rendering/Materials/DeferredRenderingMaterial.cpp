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

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredRenderingMaterial.h"
#include "Effects\Effect.h"
#include "Effects\EffectParameter.h"
#include "Textures\RenderTarget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DeferredRenderingMaterial::DeferredRenderingMaterial()
	: IMaterial("../EffectObj/DeferredRenderingEffect" + GetXEExtension(), "Deferred Rendering Effect")
	, m_HalfPixel(nullptr)
	, m_DepthTexture(nullptr)
	, m_LightTexture(nullptr)
	, m_DiffuseTexture(nullptr)
{
}

DeferredRenderingMaterial::~DeferredRenderingMaterial()
{
}

XEResult DeferredRenderingMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return XEResult::Fail;
	}

	m_HalfPixel = m_Effect->GetParameter("HalfPixel");
	m_DepthTexture = m_Effect->GetParameter("DepthTexture");
	m_LightTexture = m_Effect->GetParameter("LightTexture");
	m_DiffuseTexture = m_Effect->GetParameter("DiffuseTexture");
	
	if(m_HalfPixel == nullptr || m_DepthTexture == nullptr || m_LightTexture == nullptr || m_DiffuseTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult DeferredRenderingMaterial::ActivateTechnique(DeferredRenderingTechnique technique)
{
	if(m_Effect == nullptr)
	{
		return XEResult::Fail;
	}

	EffectTechnique* tech = nullptr;

	switch(technique)
	{
		case XE_DEFREND_DFCOMBINE:
			tech = m_Effect->GetTechnique("DFCombine");
			break;
		case XE_DEFREND_DFSKY_TECHNIQUE:
			tech = m_Effect->GetTechnique("DFSkyTechnique");
			break;
	}

	if(tech != nullptr)
	{
		return m_Effect->SetCurrentTechnique(tech);
	}

	return XEResult::Fail;
}

XEResult DeferredRenderingMaterial::SetHalfPixel(XMFLOAT2& halfpixel)
{
	if(m_Effect == nullptr || m_HalfPixel == nullptr)
	{
		return XEResult::Fail;
	}

	return m_HalfPixel->SetVect2f(halfpixel);
}

XEResult DeferredRenderingMaterial::SetDepthTexture(RenderTarget* depthTexture)
{
	if(m_Effect == nullptr || m_DepthTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return m_DepthTexture->SetTexture2D(depthTexture);
}

XEResult DeferredRenderingMaterial::SetLightTexture(RenderTarget* lightTexture)
{
	if(m_Effect == nullptr || m_LightTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return m_LightTexture->SetTexture2D(lightTexture);
}

XEResult DeferredRenderingMaterial::SetDiffuseTexture(RenderTarget* diffuseTexture)
{
	if(m_Effect == nullptr || m_DiffuseTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return m_DiffuseTexture->SetTexture2D(diffuseTexture);
}
