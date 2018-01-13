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

AEResult SkySphereMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	m_WorldViewProjection = m_Effect->GetParameter("WorldViewProjection");
	m_SkyTexture = m_Effect->GetParameter("SkyTexture");
	
	if(m_WorldViewProjection == nullptr || m_SkyTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return AEResult::Ok;
}


AEResult SkySphereMaterial::ActivateTechnique(SkySphereTechnique technique)
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	EffectTechnique* tech = nullptr;

	switch(technique)
	{
		case AE_SKY_SPHERE_EFFECT:
			tech = m_Effect->GetTechnique("SkySphereEffect");
			break;
		case AE_SKY_SPHERE_DEFERRED_EFFECT:
			tech = m_Effect->GetTechnique("SkySphereDeferredEffect");
			break;
	}

	if(tech != nullptr)
	{
		return m_Effect->SetCurrentTechnique(tech);
	}

	return AEResult::Fail;
}

AEResult SkySphereMaterial::SetWorldViewProjection(const XMFLOAT4X4& worldViewProjection)
{
	if(m_Effect == nullptr || m_WorldViewProjection == nullptr)
	{
		return AEResult::Fail;
	}

	return m_WorldViewProjection->SetMatrix(worldViewProjection);
}

AEResult SkySphereMaterial::SetSkyTexture(const TextureCube* skyTexture)
{
	if(m_Effect == nullptr || m_SkyTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return m_SkyTexture->SetTextureCube(skyTexture);
}