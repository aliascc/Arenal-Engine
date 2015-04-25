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
#include "BloomPostProcessMaterial.h"
#include "Effects\Effect.h"
#include "Effects\EffectParameter.h"
#include "Textures\RenderTarget.h"
#include "Post Process\BloomPostProcessDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BloomPostProcessMaterial::BloomPostProcessMaterial()
	: IMaterial("../EffectObj/BloomPostProcessEffect" + GetXEExtension(), "BloomPostProcessEffect")
	, m_HalfPixel(nullptr)
	, m_SceneTexture(nullptr)
	, m_PostBloomTexture(nullptr)
	, m_GaussianBlurTexture(nullptr)
	, m_BloomThreshold(nullptr)
	, m_BloomIntensity(nullptr)
	, m_BaseIntensity(nullptr)
	, m_BloomSaturation(nullptr)
	, m_BaseSaturation(nullptr)
	, m_SampleWeights(nullptr)
	, m_SampleOffsets(nullptr)
{
}

BloomPostProcessMaterial::~BloomPostProcessMaterial()
{
}

XEResult BloomPostProcessMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return XEResult::Fail;
	}

	m_HalfPixel = m_Effect->GetParameter("HalfPixel");
	m_SceneTexture = m_Effect->GetParameter("SceneTexture");
	m_PostBloomTexture = m_Effect->GetParameter("PostBloomTexture");
	m_GaussianBlurTexture = m_Effect->GetParameter("GaussianBlurTexture");
	m_BloomThreshold = m_Effect->GetParameter("BloomThreshold");
	m_BloomIntensity = m_Effect->GetParameter("BloomIntensity");
	m_BaseIntensity = m_Effect->GetParameter("BaseIntensity");
	m_BloomSaturation = m_Effect->GetParameter("BloomSaturation");
	m_BaseSaturation = m_Effect->GetParameter("BaseSaturation");
	m_SampleWeights = m_Effect->GetParameter("SampleWeights");
	m_SampleOffsets = m_Effect->GetParameter("SampleOffsets");
	
	if(m_HalfPixel == nullptr || m_SceneTexture == nullptr || m_PostBloomTexture == nullptr || m_GaussianBlurTexture == nullptr || m_BloomThreshold == nullptr || m_BloomIntensity == nullptr ||
		m_BaseIntensity == nullptr || m_BloomSaturation == nullptr || m_BaseSaturation == nullptr || m_SampleWeights == nullptr || m_SampleOffsets == nullptr)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult BloomPostProcessMaterial::ActivateTechnique(BloomTechnique technique)
{
	if(m_Effect == nullptr)
	{
		return XEResult::Fail;
	}

	EffectTechnique* tech = nullptr;

	switch(technique)
	{
		case XE_BLOOM_EXTRACT:
			tech = m_Effect->GetTechnique("BloomExtract");
			break;
		case XE_BLOOM_GAUSSIAN_BLUR:
			tech = m_Effect->GetTechnique("GaussianBlur");
			break;
		case XE_BLOOM_COMBINE:
			tech = m_Effect->GetTechnique("BloomCombine");
			break;
	}

	if(tech != nullptr)
	{
		return m_Effect->SetCurrentTechnique(tech);
	}

	return XEResult::Fail;
}

XEResult BloomPostProcessMaterial::SetHalfPixel(XMFLOAT2& halfpixel)
{
	if(m_Effect == nullptr || m_HalfPixel == nullptr)
	{
		return XEResult::Fail;
	}

	return m_HalfPixel->SetVect2f(halfpixel);
}

XEResult BloomPostProcessMaterial::SetSceneTexture(RenderTarget* sceneTexture)
{
	if(m_Effect == nullptr || m_SceneTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return m_SceneTexture->SetTexture2D(sceneTexture);
}

XEResult BloomPostProcessMaterial::SetGaussianBlurTexture(RenderTarget* gaussianBlurTexture)
{
	if(m_Effect == nullptr || m_GaussianBlurTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return m_GaussianBlurTexture->SetTexture2D(gaussianBlurTexture);
}

XEResult BloomPostProcessMaterial::SetPostBloomTexture(RenderTarget* postBloomTexture)
{
	if(m_Effect == nullptr || m_PostBloomTexture == nullptr)
	{
		return XEResult::Fail;
	}

	return m_PostBloomTexture->SetTexture2D(postBloomTexture);
}

XEResult BloomPostProcessMaterial::SetBloomIntensity(float bloomIntensity)
{
	if(m_Effect == nullptr || m_BloomIntensity == nullptr)
	{
		return XEResult::Fail;
	}

	return m_BloomIntensity->SetFloat(bloomIntensity);
}

XEResult BloomPostProcessMaterial::SetBaseIntensity(float baseIntensity)
{
	if(m_Effect == nullptr || m_BaseIntensity == nullptr)
	{
		return XEResult::Fail;
	}

	return m_BaseIntensity->SetFloat(baseIntensity);
}

XEResult BloomPostProcessMaterial::SetBloomSaturation(float bloomSaturation)
{
	if(m_Effect == nullptr || m_BloomSaturation == nullptr)
	{
		return XEResult::Fail;
	}

	return m_BloomSaturation->SetFloat(bloomSaturation);
}

XEResult BloomPostProcessMaterial::SetBaseSaturation(float baseSaturation)
{
	if(m_Effect == nullptr || m_BaseSaturation == nullptr)
	{
		return XEResult::Fail;
	}

	return m_BaseSaturation->SetFloat(baseSaturation);
}

XEResult BloomPostProcessMaterial::SetBloomThreshold(float bloomThreshold)
{
	if(m_Effect == nullptr || m_BloomThreshold == nullptr)
	{
		return XEResult::Fail;
	}

	return m_BloomThreshold->SetFloat(bloomThreshold);
}

XEResult BloomPostProcessMaterial::SetSampleWeights(float* sampleWeights)
{
	if(m_Effect == nullptr || m_SampleWeights == nullptr)
	{
		return XEResult::Fail;
	}

	return m_SampleWeights->SetFloatArray(sampleWeights, XE_BLOOM_BLUR_SAMPLE_COUNT);
}

XEResult BloomPostProcessMaterial::SetSampleOffsets(XMFLOAT2* sampleOffsets)
{
	if(m_Effect == nullptr || m_SampleOffsets == nullptr)
	{
		return XEResult::Fail;
	}

	return m_SampleOffsets->SetVect2fArray(sampleOffsets, XE_BLOOM_BLUR_SAMPLE_COUNT);
}