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

#ifndef _BLOOM_POST_PROCESS_MATERIAL_H
#define _BLOOM_POST_PROCESS_MATERIAL_H

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
class BloomPostProcessMaterial sealed : public IMaterial
{
	public:
		enum BloomTechnique
		{
			AE_BLOOM_EXTRACT = 0,
			AE_BLOOM_GAUSSIAN_BLUR,
			AE_BLOOM_COMBINE
		};

	private:
		//Variables
		EffectParameter*	m_HalfPixel;
		EffectParameter*	m_SceneTexture;
		EffectParameter*	m_PostBloomTexture;
		EffectParameter*	m_GaussianBlurTexture;
		EffectParameter*	m_BloomThreshold;
		EffectParameter*	m_BloomIntensity;
		EffectParameter*	m_BaseIntensity;
		EffectParameter*	m_BloomSaturation;
		EffectParameter*	m_BaseSaturation;
		EffectParameter*	m_SampleWeights;
		EffectParameter*	m_SampleOffsets;

		//Virtual Framework Methods
		AEResult			GetParameters			();

	public:
		//Constructor Destructor.
		BloomPostProcessMaterial();
		virtual ~BloomPostProcessMaterial();

		//Framewrok Methods
		AEResult			ActivateTechnique		(BloomTechnique technique);
		AEResult			SetHalfPixel			(XMFLOAT2& halfpixel);
		AEResult			SetSceneTexture			(RenderTarget* sceneTexture);
		AEResult			SetPostBloomTexture		(RenderTarget* postBloomTexture);
		AEResult			SetGaussianBlurTexture	(RenderTarget* gaussianBlurTexture);
		AEResult			SetBloomThreshold		(float bloomThreshold);
		AEResult			SetBloomIntensity		(float bloomIntensity);
		AEResult			SetBaseIntensity		(float baseIntensity);
		AEResult			SetBloomSaturation		(float bloomSaturation);
		AEResult			SetBaseSaturation		(float baseSaturation);
		AEResult			SetSampleWeights		(float* sampleWeights);
		AEResult			SetSampleOffsets		(XMFLOAT2* sampleOffsets);
};

#endif

