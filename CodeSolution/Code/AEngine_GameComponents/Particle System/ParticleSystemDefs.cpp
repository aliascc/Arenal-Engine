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
#include "ParticleSystemDefs.h"
#include "Color\AEColorDefs.h"
#include "Base\Base.h"

/********************
*   Function Defs   *
*********************/
ParticleSystemSettings::ParticleSystemSettings()
	: m_TextureName("Default")
	, m_MaxParticles(100)
	, m_Duration(1.0f)
	, m_DurationRandomness(0.0f)
	, m_EmitterVelocitySensitivity(1.0f)
	, m_MinHorizontalVelocity(0.0f)
	, m_MaxHorizontalVelocity(0.0f)
	, m_MinVerticalVelocity(0.0f)
	, m_MaxVerticalVelocity(0.0f)
	, m_Gravity(XMFLOAT3ZERO)
	, m_EndVelocity(1.0f)
	, m_MinColor(XMCOLORWhite)
	, m_MaxColor(XMCOLORWhite)
	, m_MinRotateSpeed(0.0f)
	, m_MaxRotateSpeed(0.0f)
	, m_MinStartSize(100.0f)
	, m_MaxStartSize(100.0f)
	, m_MinEndSize(100.0f)
	, m_MaxEndSize(100.0f)
	, m_BlendState(GraphicBlendState::NonPremultipliedState)
{
}
		
ParticleSystemSettings::ParticleSystemSettings(const std::string textureName, uint32_t maxParticles, float duration, float durationRandomness, float emitterVelocitySensitivity,
												float minHorizontalVelocity, float maxHorizontalVelocity, float minVerticalVelocity, float maxVerticalVelocity,
												const XMFLOAT3& gravity, float endVelocity, const XMCOLOR& minColor, const XMCOLOR& maxColor, float minRotateSpeed,
												float maxRotateSpeed, float minStartSize, float maxStartSize, float minEndSize, float maxEndSize, const GraphicBlendState& blendState)
	: m_TextureName(textureName)
	, m_MaxParticles(maxParticles)
	, m_Duration(duration)
	, m_DurationRandomness(durationRandomness)
	, m_EmitterVelocitySensitivity(emitterVelocitySensitivity)
	, m_MinHorizontalVelocity(minHorizontalVelocity)
	, m_MaxHorizontalVelocity(maxHorizontalVelocity)
	, m_MinVerticalVelocity(minVerticalVelocity)
	, m_MaxVerticalVelocity(maxVerticalVelocity)
	, m_Gravity(gravity)
	, m_EndVelocity(endVelocity)
	, m_MinColor(minColor)
	, m_MaxColor(maxColor)
	, m_MinRotateSpeed(minRotateSpeed)
	, m_MaxRotateSpeed(maxRotateSpeed)
	, m_MinStartSize(minStartSize)
	, m_MaxStartSize(maxStartSize)
	, m_MinEndSize(minEndSize)
	, m_MaxEndSize(maxEndSize)
	, m_BlendState(blendState)
{
}
