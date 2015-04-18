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

#ifndef _HDR_POST_PROCESS_H
#define _HDR_POST_PROCESS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <Windows.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"
#include "Base\Base.h"
#include "HDRPostProcessDefs.h"

/************
*   Using   *
*************/
using namespace DirectX;
using namespace PackedVector;

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameApp;
class RenderTarget;
class GraphicDevice;
class HDRPostProcessMaterial;
class DeferredRenderingEnd;

/*****************
*   Class Decl   *
******************/
class HDRPostProcess : public DrawableGameComponent
{
	protected:
		//Variables
		RenderTarget*				m_SceneRT;
		RenderTarget*				m_SceneScaledRT;
		RenderTarget*				m_BrightPass;
		RenderTarget*				m_StartSource;
		RenderTarget*				m_BloomSource;
		RenderTarget*				m_AdaptedLuminanceCur;
		RenderTarget*				m_AdaptedLuminanceLast;
		RenderTarget*				m_ToneMapTex[XE_HDR_MAX_TONE_MAP_TEXS];
		RenderTarget*				m_BloomTex[XE_HDR_MAX_BLOOM_TEXS];
		RenderTarget*				m_StartTexs[XE_HDR_MAX_START_TEXS];

		HDRPostProcessMaterial*		m_HDREffect;

		uint32_t					m_CropWidth;
		uint32_t					m_CropHeight;

		bool						m_AdaptationInvalid;

		GlareProp					m_GlareProp;

		float						m_KeyValue;

		const std::string			m_DeferredEndServiceName;
		bool						m_UseDREndRT;
		DeferredRenderingEnd*		m_DREnd;

		//Methods
		void						InitEffectParams				();
		void						ClearTextures					();
		void						ScaleScene						();
		void						GetTextCoord					(RenderTarget* src, const RECT* rectSrc, RenderTarget* dst,  const RECT* rectDst, XMFLOAT4& texCoords);
		void						GetSampleOffsetsDS4x4			(uint32_t width, uint32_t height, XMFLOAT2 sampleOffsets[]);
		void						GetSampleOffsetsDS2x2			(uint32_t width, uint32_t height, XMFLOAT2 sampleOffsets[]);
		void						GetSampleOffsetsBloom			(uint32_t texSize, float texCoordOffset[15], XMFLOAT4 colorWeight[], float deviation, float multiplier);
		void						MeasureLuminance				();
		void						CalculateAdaptation				(const TimerParams& timerParams);
		void						ScaledToBrightPass				();
		XEResult					GetTextureRect					(RenderTarget* rt, RECT& rect);
		void						BrightPassStarSource			();
		void						StarSourceBloomSource			();
		void						GetSampleOffsetsGaussBlur5x5	(uint32_t width, uint32_t height, XMFLOAT2 texCoordOffset[], XMFLOAT4 sampleWeight[], float multiplier = 1.0f);
		void						RenderBloom						();
		void						RenderStar						();
		void						RenderFinalPass					();

	public:
		//Constructor and Destructor
		HDRPostProcess(GameApp* gameApp, bool useDREndRT = false, const std::string& deferredEndServiceName = "DeferredRenderingEnd", const std::string& gameComponentName = "HDR Post Process");
		virtual ~HDRPostProcess();

		//Drawable Game Component Override methods
		void						Initialize						();
		void						LoadContent						();
		void						Update							(const TimerParams& timerParams);
		void						Render							(const TimerParams& timerParams);
		void						OnLostDevice					();
		void						OnResetDevice					();
};

#endif