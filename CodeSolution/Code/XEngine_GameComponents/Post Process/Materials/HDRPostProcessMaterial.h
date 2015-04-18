
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/24/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

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
			XE_HDR_DOWN_SCALE_4x4 = 0,
			XE_HDR_DOWN_SCALE_2x2,
			XE_HDR_SAMPLE_AVG_LUM,
			XE_HDR_RESAMPLE_AVG_LUM,
			XE_HDR_RESAMPLE_AVG_LUM_EXP,
			XE_HDR_CALCULATE_ADAPTED_LUM,
			XE_HDR_BRIGHT_PASS_FILTER,
			XE_HDR_GUASS_BLUR_5x5,
			XE_HDR_BLOOM,
			XE_HDR_STAR,
			XE_HDR_MERGE_TEXTURES_1,
			XE_HDR_MERGE_TEXTURES_2,
			XE_HDR_MERGE_TEXTURES_3,
			XE_HDR_MERGE_TEXTURES_4,
			XE_HDR_MERGE_TEXTURES_5,
			XE_HDR_MERGE_TEXTURES_6,
			XE_HDR_MERGE_TEXTURES_7,
			XE_HDR_MERGE_TEXTURES_8,
			XE_HDR_FINAL_SCENE_PASS,
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
		XEResult			GetParameters			();

	public:
		//Constructor Destructor.
		HDRPostProcessMaterial();
		virtual ~HDRPostProcessMaterial();

		//Framewrok Methods
		XEResult			ActivateTechnique		(HDRTechnique technique);
		XEResult			ActivateMergeTechnique	(uint32_t mergeNum);
		XEResult			SetHalfPixel			(XMFLOAT2& halfpixel);
		XEResult			SetSampleWeights		(XMFLOAT4 sampleWeights[]);
		XEResult			SetSampleOffsets		(XMFLOAT2 sampleOffsets[]);
		XEResult			SetElapsedTime			(float elapsedTime);
		XEResult			SetMiddleGray			(float middleGray);
		XEResult			SetBloomScale			(float bloomScale);
		XEResult			SetStarScale			(float starScale);
		XEResult			SetEnableBlueShift		(bool enableBlueShift);
		XEResult			SetEnableToneMap		(bool enableToneMap);
};

#endif

