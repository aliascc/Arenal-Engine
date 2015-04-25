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
#include "SkySphereMaterial.h"
#include "Textures\TextureCube.h"
#include "Effects\Effect.h"
#include "Effects\EffectParameter.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SkySphereMaterial::SkySphereMaterial()
	: IMaterial("../EffectObj/SkySphereEffect" + GetXEExtension(), "SkySphereEffect")
	, m_WorldViewProjection(nullptr)
	, m_SkyTexture(nullptr)
{
}

SkySphereMaterial::~SkySphereMaterial()
{
}

XEResult SkySphereMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return XEResult::Fail;
	}

	m_WorldViewProjection = m_Effect->GetParameter("WorldViewProjection");
	m_SkyTexture = m_Effect->GetParameter("SkyTexture");
	
	if(m_WorldViewProjection == nullptr || m_SkyTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}


XEResult SkySphereMaterial::ActivateTechnique(SkySphereTechnique technique)
{
	if(m_Effect == nullptr)
	{
		return XEResult::Fail;
	}

	EffectTechnique* tech = nullptr;

	switch(technique)
	{
		case XE_SKY_SPHERE_EFFECT:
			tech = m_Effect->GetTechnique("SkySphereEffect");
			break;
		case XE_SKY_SPHERE_DEFERRED_EFFECT:
			tech = m_Effect->GetTechnique("SkySphereDeferredEffect");
			break;
	}

	if(tech != nullptr)
	{
		return m_Effect->SetCurrentTechnique(tech);
	}

	return XEResult::Fail;
}

XEResult SkySphereMaterial::SetWorldViewProjection(const XMFLOAT4X4& worldViewProjection)
{
	if(m_Effect == nullptr || m_WorldViewProjection == nullptr)
	{
		return XEResult::Fail;
	}

	return m_WorldViewProjection->SetMatrix(worldViewProjection);
}

XEResult SkySphereMaterial::SetSkyTexture(const TextureCube* skyTexture)
{
	if(m_Effect == nullptr || m_SkyTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return m_SkyTexture->SetTextureCube(skyTexture);
}