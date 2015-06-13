/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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

