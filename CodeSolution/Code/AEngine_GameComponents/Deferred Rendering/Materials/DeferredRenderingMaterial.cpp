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

AEResult DeferredRenderingMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	m_HalfPixel = m_Effect->GetParameter("HalfPixel");
	m_DepthTexture = m_Effect->GetParameter("DepthTexture");
	m_LightTexture = m_Effect->GetParameter("LightTexture");
	m_DiffuseTexture = m_Effect->GetParameter("DiffuseTexture");
	
	if(m_HalfPixel == nullptr || m_DepthTexture == nullptr || m_LightTexture == nullptr || m_DiffuseTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult DeferredRenderingMaterial::ActivateTechnique(DeferredRenderingTechnique technique)
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	EffectTechnique* tech = nullptr;

	switch(technique)
	{
		case AE_DEFREND_DFCOMBINE:
			tech = m_Effect->GetTechnique("DFCombine");
			break;
		case AE_DEFREND_DFSKY_TECHNIQUE:
			tech = m_Effect->GetTechnique("DFSkyTechnique");
			break;
	}

	if(tech != nullptr)
	{
		return m_Effect->SetCurrentTechnique(tech);
	}

	return AEResult::Fail;
}

AEResult DeferredRenderingMaterial::SetHalfPixel(XMFLOAT2& halfpixel)
{
	if(m_Effect == nullptr || m_HalfPixel == nullptr)
	{
		return AEResult::Fail;
	}

	return m_HalfPixel->SetVect2f(halfpixel);
}

AEResult DeferredRenderingMaterial::SetDepthTexture(RenderTarget* depthTexture)
{
	if(m_Effect == nullptr || m_DepthTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return m_DepthTexture->SetTexture2D(depthTexture);
}

AEResult DeferredRenderingMaterial::SetLightTexture(RenderTarget* lightTexture)
{
	if(m_Effect == nullptr || m_LightTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return m_LightTexture->SetTexture2D(lightTexture);
}

AEResult DeferredRenderingMaterial::SetDiffuseTexture(RenderTarget* diffuseTexture)
{
	if(m_Effect == nullptr || m_DiffuseTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return m_DiffuseTexture->SetTexture2D(diffuseTexture);
}
