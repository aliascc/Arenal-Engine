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
#include "ParticleSystemMaterial.h"
#include "Textures\Texture2D.h"
#include "Effects\Effect.h"
#include "Effects\EffectParameter.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ParticleSystemMaterial::ParticleSystemMaterial()
	: IMaterial("../EffectObj/ParticleEffect" + GetXEExtension(), "ParticleEffect")
	, m_ViewParam(nullptr)
	, m_ProjectionParam(nullptr)
	, m_ViewportScaleParam(nullptr)
	, m_CurrentTimeParam(nullptr)
	, m_DurationParam(nullptr)
	, m_DurationRandomnessParam(nullptr)
	, m_GravityParam(nullptr)
	, m_EndVelocityParam(nullptr)
	, m_MinColorParam(nullptr)
	, m_MaxColorParam(nullptr)
	, m_RotateSpeedParam(nullptr)
	, m_StartSizeParam(nullptr)
	, m_EndSizeParam(nullptr)
	, m_ParticleTextureParam(nullptr)
{
}

ParticleSystemMaterial::~ParticleSystemMaterial()
{
}

AEResult ParticleSystemMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	m_ViewParam = m_Effect->GetParameter("View");
	m_ProjectionParam = m_Effect->GetParameter("Projection");
	m_ViewportScaleParam = m_Effect->GetParameter("ViewportScale");
	m_CurrentTimeParam = m_Effect->GetParameter("CurrentTime");
	m_DurationParam = m_Effect->GetParameter("Duration");
	m_DurationRandomnessParam = m_Effect->GetParameter("DurationRandomness");
	m_GravityParam = m_Effect->GetParameter("Gravity");
	m_EndVelocityParam = m_Effect->GetParameter("EndVelocity");
	m_MinColorParam = m_Effect->GetParameter("MinColor");
	m_MaxColorParam = m_Effect->GetParameter("MaxColor");
	m_RotateSpeedParam = m_Effect->GetParameter("RotateSpeed");
	m_StartSizeParam = m_Effect->GetParameter("StartSize");
	m_EndSizeParam = m_Effect->GetParameter("EndSize");
	m_ParticleTextureParam = m_Effect->GetParameter("ParticleTexture");
	
	if(m_ViewParam == nullptr || m_ProjectionParam == nullptr || m_ViewportScaleParam == nullptr || m_CurrentTimeParam == nullptr || m_DurationParam == nullptr || m_DurationRandomnessParam == nullptr,
		m_GravityParam == nullptr || m_EndVelocityParam == nullptr || m_MinColorParam == nullptr || m_MaxColorParam == nullptr || m_RotateSpeedParam == nullptr  || m_StartSizeParam == nullptr ||
		m_EndSizeParam == nullptr  || m_ParticleTextureParam == nullptr )
	{
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult ParticleSystemMaterial::SetView(const XMFLOAT4X4& view)
{
	if(m_Effect == nullptr || m_ViewParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_ViewParam->SetMatrix(view);
}

AEResult ParticleSystemMaterial::SetProjection(const XMFLOAT4X4& projection)
{
	if(m_Effect == nullptr || m_ProjectionParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_ProjectionParam->SetMatrix(projection);
}

AEResult ParticleSystemMaterial::SetViewportScale(const XMFLOAT2& viewportScale)
{
	if(m_Effect == nullptr || m_ViewportScaleParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_ViewportScaleParam->SetVect2f(viewportScale);
}

AEResult ParticleSystemMaterial::SetCurrentTime(float currentTime)
{
	if(m_Effect == nullptr || m_CurrentTimeParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_CurrentTimeParam->SetFloat(currentTime);
}

AEResult ParticleSystemMaterial::SetDuration(float duration)
{
	if(m_Effect == nullptr || m_DurationParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_DurationParam->SetFloat(duration);
}

AEResult ParticleSystemMaterial::SetDurationRandomness(float durationRandomness)
{
	if(m_Effect == nullptr || m_DurationRandomnessParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_DurationRandomnessParam->SetFloat(durationRandomness);
}

AEResult ParticleSystemMaterial::SetGravity(const XMFLOAT3& gravity)
{
	if(m_Effect == nullptr || m_GravityParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_GravityParam->SetVect3f(gravity);
}

AEResult ParticleSystemMaterial::SetEndVelocity(float endVelocity)
{
	if(m_Effect == nullptr || m_EndVelocityParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_EndVelocityParam->SetFloat(endVelocity);
}

AEResult ParticleSystemMaterial::SetMinColor(const XMCOLOR& color)
{
	if(m_Effect == nullptr || m_MinColorParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_MinColorParam->SetColor(color);
}

AEResult ParticleSystemMaterial::SetMaxColor(const XMCOLOR& color)
{
	if(m_Effect == nullptr || m_MaxColorParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_MaxColorParam->SetColor(color);
}

AEResult ParticleSystemMaterial::SetRotateSpeed(const XMFLOAT2& rotateSpeed)
{
	if(m_Effect == nullptr || m_RotateSpeedParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_RotateSpeedParam->SetVect2f(rotateSpeed);
}

AEResult ParticleSystemMaterial::SetStartSize(const XMFLOAT2& startSize)
{
	if(m_Effect == nullptr || m_StartSizeParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_StartSizeParam->SetVect2f(startSize);
}

AEResult ParticleSystemMaterial::SetEndSize(const XMFLOAT2& endSize)
{
	if(m_Effect == nullptr || m_EndSizeParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_EndSizeParam->SetVect2f(endSize);
}

AEResult ParticleSystemMaterial::SetParticleTexture(const Texture2D* texture)
{
	if(m_Effect == nullptr || m_ParticleTextureParam == nullptr)
	{
		return AEResult::Fail;
	}

	return m_ParticleTextureParam->SetTexture2D(texture);
}
