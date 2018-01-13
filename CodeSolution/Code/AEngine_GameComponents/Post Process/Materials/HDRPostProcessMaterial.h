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

#pragma once

#ifndef _HDR_POST_PROCESS_MATERIAL_H
#define _HDR_POST_PROCESS_MATERIAL_H

/**********************
*   System Includes   *
***********************/
#include <DirectXMath.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Effects\Materials\IMaterial.h"

/********************************
*		Using					*
*********************************/
using namespace DirectX;

/********************
*   Forward Decls   *
*********************/
class RenderTarget;
class EffectParameter;

/*****************
*   Class Decl   *
******************/
class HDRPostProcessMaterial sealed : public IMaterial
{
	public:
		enum HDRTechnique : uint32_t
		{
			AE_HDR_DOWN_SCALE_4x4 = 0,
			AE_HDR_DOWN_SCALE_2x2,
			AE_HDR_SAMPLE_AVG_LUM,
			AE_HDR_RESAMPLE_AVG_LUM,
			AE_HDR_RESAMPLE_AVG_LUM_EXP,
			AE_HDR_CALCULATE_ADAPTED_LUM,
			AE_HDR_BRIGHT_PASS_FILTER,
			AE_HDR_GUASS_BLUR_5x5,
			AE_HDR_BLOOM,
			AE_HDR_STAR,
			AE_HDR_MERGE_TEXTURES_1,
			AE_HDR_MERGE_TEXTURES_2,
			AE_HDR_MERGE_TEXTURES_3,
			AE_HDR_MERGE_TEXTURES_4,
			AE_HDR_MERGE_TEXTURES_5,
			AE_HDR_MERGE_TEXTURES_6,
			AE_HDR_MERGE_TEXTURES_7,
			AE_HDR_MERGE_TEXTURES_8,
			AE_HDR_FINAL_SCENE_PASS,
		};

	private:
		//Variables
		EffectParameter*	m_HalfPixel;
		EffectParameter*	m_SampleOffsets;
		EffectParameter*	m_SampleWeights;
		EffectParameter*	m_ElapsedTime;
		EffectParameter*	m_MiddleGray;
		EffectParameter*	m_BloomScale;
		EffectParameter*	m_StarScale;
		EffectParameter*	m_EnableBlueShift;
		EffectParameter*	m_EnableToneMap;

		//Virtual Framework Methods
		AEResult			GetParameters			();

	public:
		//Constructor Destructor.
		HDRPostProcessMaterial();
		virtual ~HDRPostProcessMaterial();

		//Framewrok Methods
		AEResult			ActivateTechnique		(HDRTechnique technique);
		AEResult			ActivateMergeTechnique	(uint32_t mergeNum);
		AEResult			SetHalfPixel			(XMFLOAT2& halfpixel);
		AEResult			SetSampleWeights		(XMFLOAT4 sampleWeights[]);
		AEResult			SetSampleOffsets		(XMFLOAT2 sampleOffsets[]);
		AEResult			SetElapsedTime			(float elapsedTime);
		AEResult			SetMiddleGray			(float middleGray);
		AEResult			SetBloomScale			(float bloomScale);
		AEResult			SetStarScale			(float starScale);
		AEResult			SetEnableBlueShift		(bool enableBlueShift);
		AEResult			SetEnableToneMap		(bool enableToneMap);
};

#endif

