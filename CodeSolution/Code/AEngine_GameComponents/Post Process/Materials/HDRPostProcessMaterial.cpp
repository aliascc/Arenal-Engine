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
#include "HDRPostProcessMaterial.h"
#include "Effects\Effect.h"
#include "Effects\EffectParameter.h"
#include "Textures\RenderTarget.h"
#include "Post Process\HDRPostProcessDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
HDRPostProcessMaterial::HDRPostProcessMaterial()
	: IMaterial("../EffectObj/HDRPostProcessEffect" + GetXEExtension(), "HDRPostProcessEffect")
	, m_HalfPixel(nullptr)
	, m_SampleOffsets(nullptr) 
	, m_SampleWeights(nullptr)
	, m_ElapsedTime(nullptr)
	, m_MiddleGray(nullptr)
	, m_BloomScale(nullptr)
	, m_StarScale(nullptr)
	, m_EnableBlueShift(nullptr) 
	, m_EnableToneMap(nullptr)
{
}

HDRPostProcessMaterial::~HDRPostProcessMaterial()
{
}

AEResult HDRPostProcessMaterial::GetParameters()
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	m_HalfPixel = m_Effect->GetParameter("HalfPixel");
	m_SampleOffsets = m_Effect->GetParameter("SampleOffsets");
	m_SampleWeights = m_Effect->GetParameter("SampleWeights");
	m_ElapsedTime = m_Effect->GetParameter("ElapsedTime");
	m_MiddleGray = m_Effect->GetParameter("MiddleGray");
	m_BloomScale = m_Effect->GetParameter("BloomScale");
	m_StarScale = m_Effect->GetParameter("StarScale");
	m_EnableBlueShift = m_Effect->GetParameter("EnableToneMap");
	m_EnableToneMap = m_Effect->GetParameter("EnableBlueShift");
	
	if(m_HalfPixel == nullptr || m_SampleOffsets == nullptr || m_SampleWeights == nullptr || m_ElapsedTime == nullptr || m_MiddleGray == nullptr || m_BloomScale == nullptr || m_StarScale == nullptr || m_EnableBlueShift == nullptr || m_EnableToneMap == nullptr)
	{
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult HDRPostProcessMaterial::ActivateTechnique(HDRTechnique technique)
{
	if(m_Effect == nullptr)
	{
		return AEResult::Fail;
	}

	EffectTechnique* tech = nullptr;

	switch(technique)
	{
		case AE_HDR_DOWN_SCALE_4x4:
			tech = m_Effect->GetTechnique("DownScale4x4");
			break;
		case AE_HDR_DOWN_SCALE_2x2:
			tech = m_Effect->GetTechnique("DownScale2x2");
			break;
		case AE_HDR_SAMPLE_AVG_LUM:
			tech = m_Effect->GetTechnique("SampleAvgLum");
			break;
		case AE_HDR_RESAMPLE_AVG_LUM:
			tech = m_Effect->GetTechnique("ResampleAvgLum");
			break;
		case AE_HDR_RESAMPLE_AVG_LUM_EXP:
			tech = m_Effect->GetTechnique("ResampleAvgLumExp");
			break;
		case AE_HDR_CALCULATE_ADAPTED_LUM:
			tech = m_Effect->GetTechnique("CalculateAdaptedLum");
			break;
		case AE_HDR_BRIGHT_PASS_FILTER:
			tech = m_Effect->GetTechnique("BrightPassFilter");
			break;
		case AE_HDR_GUASS_BLUR_5x5:
			tech = m_Effect->GetTechnique("GaussBlur5x5");
			break;
		case AE_HDR_BLOOM:
			tech = m_Effect->GetTechnique("Bloom");
			break;
		case AE_HDR_STAR:
			tech = m_Effect->GetTechnique("Star");
			break;
		case AE_HDR_MERGE_TEXTURES_1:
			tech = m_Effect->GetTechnique("MergeTextures_1");
			break;
		case AE_HDR_MERGE_TEXTURES_2:
			tech = m_Effect->GetTechnique("MergeTextures_2");
			break;
		case AE_HDR_MERGE_TEXTURES_3:
			tech = m_Effect->GetTechnique("MergeTextures_3");
			break;
		case AE_HDR_MERGE_TEXTURES_4:
			tech = m_Effect->GetTechnique("MergeTextures_4");
			break;
		case AE_HDR_MERGE_TEXTURES_5:
			tech = m_Effect->GetTechnique("MergeTextures_5");
			break;
		case AE_HDR_MERGE_TEXTURES_6:
			tech = m_Effect->GetTechnique("MergeTextures_6");
			break;
		case AE_HDR_MERGE_TEXTURES_7:
			tech = m_Effect->GetTechnique("MergeTextures_7");
			break;
		case AE_HDR_MERGE_TEXTURES_8:
			tech = m_Effect->GetTechnique("MergeTextures_8");
			break;
		case AE_HDR_FINAL_SCENE_PASS:
			tech = m_Effect->GetTechnique("FinalScenePass");
			break;
	}

	if(tech != nullptr)
	{
		return m_Effect->SetCurrentTechnique(tech);
	}

	return AEResult::Fail;
}

AEResult HDRPostProcessMaterial::ActivateMergeTechnique(uint32_t mergeNum)
{
	AEResult ret = AEResult::Fail;

	switch(mergeNum)
	{
		case 1:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_1);
			break;
		case 2:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_2);
			break;
		case 3:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_3);
			break;
		case 4:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_4);
			break;
		case 5:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_5);
			break;
		case 6:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_6);
			break;
		case 7:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_7);
			break;
		case 8:
			ret = ActivateTechnique(AE_HDR_MERGE_TEXTURES_8);
			break;
	}

	return ret;
}

AEResult HDRPostProcessMaterial::SetHalfPixel(XMFLOAT2& halfpixel)
{
	if(m_Effect == nullptr || m_HalfPixel == nullptr)
	{
		return AEResult::Fail;
	}

	return m_HalfPixel->SetVect2f(halfpixel);
}

AEResult HDRPostProcessMaterial::SetSampleWeights(XMFLOAT4 sampleWeights[])
{
	if(m_Effect == nullptr || m_SampleWeights == nullptr)
	{
		return AEResult::Fail;
	}

	return m_SampleWeights->SetVect4fArray(sampleWeights, AE_HDR_MAX_SAMPLES);
}

AEResult HDRPostProcessMaterial::SetSampleOffsets(XMFLOAT2 sampleOffsets[])
{
	if(m_Effect == nullptr || m_SampleOffsets == nullptr)
	{
		return AEResult::Fail;
	}

	return m_SampleOffsets->SetVect2fArray(sampleOffsets, AE_HDR_MAX_SAMPLES);
}

AEResult HDRPostProcessMaterial::SetElapsedTime(float elapsedTime)
{
	if(m_Effect == nullptr || m_ElapsedTime == nullptr)
	{
		return AEResult::Fail;
	}

	return m_ElapsedTime->SetFloat(elapsedTime);
}

AEResult HDRPostProcessMaterial::SetMiddleGray(float middleGray)
{
	if(m_Effect == nullptr || m_MiddleGray == nullptr)
	{
		return AEResult::Fail;
	}

	return m_MiddleGray->SetFloat(middleGray);
}

AEResult HDRPostProcessMaterial::SetBloomScale(float bloomScale)
{
	if(m_Effect == nullptr || m_BloomScale == nullptr)
	{
		return AEResult::Fail;
	}

	return m_BloomScale->SetFloat(bloomScale);
}

AEResult HDRPostProcessMaterial::SetStarScale(float starScale)
{
	if(m_Effect == nullptr || m_StarScale == nullptr)
	{
		return AEResult::Fail;
	}

	return m_StarScale->SetFloat(starScale);
}

AEResult HDRPostProcessMaterial::SetEnableBlueShift(bool enableBlueShift)
{
	if(m_Effect == nullptr || m_EnableBlueShift == nullptr)
	{
		return AEResult::Fail;
	}

	return m_EnableBlueShift->SetBool(enableBlueShift);
}

AEResult HDRPostProcessMaterial::SetEnableToneMap(bool enableToneMap)
{
	if(m_Effect == nullptr || m_EnableToneMap == nullptr)
	{
		return AEResult::Fail;
	}

	return m_EnableToneMap->SetBool(enableToneMap);
}
