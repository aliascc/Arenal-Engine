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
			XE_BLOOM_EXTRACT = 0,
			XE_BLOOM_GAUSSIAN_BLUR,
			XE_BLOOM_COMBINE
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
		XEResult			GetParameters			();

	public:
		//Constructor Destructor.
		BloomPostProcessMaterial();
		virtual ~BloomPostProcessMaterial();

		//Framewrok Methods
		XEResult			ActivateTechnique		(BloomTechnique technique);
		XEResult			SetHalfPixel			(XMFLOAT2& halfpixel);
		XEResult			SetSceneTexture			(RenderTarget* sceneTexture);
		XEResult			SetPostBloomTexture		(RenderTarget* postBloomTexture);
		XEResult			SetGaussianBlurTexture	(RenderTarget* gaussianBlurTexture);
		XEResult			SetBloomThreshold		(float bloomThreshold);
		XEResult			SetBloomIntensity		(float bloomIntensity);
		XEResult			SetBaseIntensity		(float baseIntensity);
		XEResult			SetBloomSaturation		(float bloomSaturation);
		XEResult			SetBaseSaturation		(float baseSaturation);
		XEResult			SetSampleWeights		(float* sampleWeights);
		XEResult			SetSampleOffsets		(XMFLOAT2* sampleOffsets);
};

#endif

