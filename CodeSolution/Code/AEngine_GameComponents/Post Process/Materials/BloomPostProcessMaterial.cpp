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

AEResult BloomPostProcessMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
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
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult BloomPostProcessMaterial::ActivateTechnique(BloomTechnique technique)
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	EffectTechnique* tech = nullptr;

	switch(technique)
	{
		case AE_BLOOM_EXTRACT:
			tech = m_Effect->GetTechnique("BloomExtract");
			break;
		case AE_BLOOM_GAUSSIAN_BLUR:
			tech = m_Effect->GetTechnique("GaussianBlur");
			break;
		case AE_BLOOM_COMBINE:
			tech = m_Effect->GetTechnique("BloomCombine");
			break;
	}

	if(tech != nullptr)
	{
		return m_Effect->SetCurrentTechnique(tech);
	}

	return AEResult::Fail;
}

AEResult BloomPostProcessMaterial::SetHalfPixel(XMFLOAT2& halfpixel)
{
	if(m_Effect == nullptr || m_HalfPixel == nullptr)
	{
		return AEResult::Fail;
	}

	return m_HalfPixel->SetVect2f(halfpixel);
}

AEResult BloomPostProcessMaterial::SetSceneTexture(RenderTarget* sceneTexture)
{
	if(m_Effect == nullptr || m_SceneTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return m_SceneTexture->SetTexture2D(sceneTexture);
}

AEResult BloomPostProcessMaterial::SetGaussianBlurTexture(RenderTarget* gaussianBlurTexture)
{
	if(m_Effect == nullptr || m_GaussianBlurTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return m_GaussianBlurTexture->SetTexture2D(gaussianBlurTexture);
}

AEResult BloomPostProcessMaterial::SetPostBloomTexture(RenderTarget* postBloomTexture)
{
	if(m_Effect == nullptr || m_PostBloomTexture == nullptr)
	{
		return AEResult::Fail;
	}

	return m_PostBloomTexture->SetTexture2D(postBloomTexture);
}

AEResult BloomPostProcessMaterial::SetBloomIntensity(float bloomIntensity)
{
	if(m_Effect == nullptr || m_BloomIntensity == nullptr)
	{
		return AEResult::Fail;
	}

	return m_BloomIntensity->SetFloat(bloomIntensity);
}

AEResult BloomPostProcessMaterial::SetBaseIntensity(float baseIntensity)
{
	if(m_Effect == nullptr || m_BaseIntensity == nullptr)
	{
		return AEResult::Fail;
	}

	return m_BaseIntensity->SetFloat(baseIntensity);
}

AEResult BloomPostProcessMaterial::SetBloomSaturation(float bloomSaturation)
{
	if(m_Effect == nullptr || m_BloomSaturation == nullptr)
	{
		return AEResult::Fail;
	}

	return m_BloomSaturation->SetFloat(bloomSaturation);
}

AEResult BloomPostProcessMaterial::SetBaseSaturation(float baseSaturation)
{
	if(m_Effect == nullptr || m_BaseSaturation == nullptr)
	{
		return AEResult::Fail;
	}

	return m_BaseSaturation->SetFloat(baseSaturation);
}

AEResult BloomPostProcessMaterial::SetBloomThreshold(float bloomThreshold)
{
	if(m_Effect == nullptr || m_BloomThreshold == nullptr)
	{
		return AEResult::Fail;
	}

	return m_BloomThreshold->SetFloat(bloomThreshold);
}

AEResult BloomPostProcessMaterial::SetSampleWeights(float* sampleWeights)
{
	if(m_Effect == nullptr || m_SampleWeights == nullptr)
	{
		return AEResult::Fail;
	}

	return m_SampleWeights->SetFloatArray(sampleWeights, AE_BLOOM_BLUR_SAMPLE_COUNT);
}

AEResult BloomPostProcessMaterial::SetSampleOffsets(XMFLOAT2* sampleOffsets)
{
	if(m_Effect == nullptr || m_SampleOffsets == nullptr)
	{
		return AEResult::Fail;
	}

	return m_SampleOffsets->SetVect2fArray(sampleOffsets, AE_BLOOM_BLUR_SAMPLE_COUNT);
}