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
#include "HDRPostProcess.h"
#include "Utils\AETime.h"
#include "Textures\RenderTarget.h"
#include "Materials\HDRPostProcessMaterial.h"
#include "Deferred Rendering\DeferredRenderingEnd.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
HDRPostProcess::HDRPostProcess(GameApp* gameApp, bool useDREndRT, const std::string& deferredEndServiceName, const std::string& gameComponentName)
	: DrawableGameComponent(gameApp, gameComponentName)
	, m_SceneRT(nullptr)
	, m_SceneScaledRT(nullptr)
	, m_BrightPass(nullptr)
	, m_StartSource(nullptr)
	, m_BloomSource(nullptr)
	, m_AdaptedLuminanceCur(nullptr)
	, m_AdaptedLuminanceLast(nullptr)
	, m_HDREffect(nullptr)
	, m_CropWidth(0)
	, m_CropHeight(0)
	, m_AdaptationInvalid(false)
	, m_GlareProp(GlareProp::FilterCrossScreenSpectral)
	, m_KeyValue(0.18f)
	, m_DREnd(nullptr)
	, m_UseDREndRT(useDREndRT)
	, m_DeferredEndServiceName(deferredEndServiceName)
{
	memset(m_ToneMapTex, 0, sizeof(RenderTarget*) * AE_HDR_MAX_TONE_MAP_TEXS);
	memset(m_BloomTex, 0, sizeof(RenderTarget*) * AE_HDR_MAX_BLOOM_TEXS);
	memset(m_StartTexs, 0, sizeof(RenderTarget*) * AE_HDR_MAX_START_TEXS);
}

HDRPostProcess::~HDRPostProcess()
{
	uint32_t i = 0;

	for(i = 0; i < AE_HDR_MAX_TONE_MAP_TEXS; ++i)
	{
		DeleteMem(m_ToneMapTex[i]);
	}

	for(i = 0; i < AE_HDR_MAX_BLOOM_TEXS; ++i)
	{
		DeleteMem(m_BloomTex[i]);
	}

	for(i = 0; i < AE_HDR_MAX_START_TEXS; ++i)
	{
		DeleteMem(m_StartTexs[i]);
	}

	DeleteMem(m_SceneScaledRT);
	DeleteMem(m_BrightPass);
	DeleteMem(m_StartSource);
	DeleteMem(m_BloomSource);
	DeleteMem(m_AdaptedLuminanceCur);
	DeleteMem(m_AdaptedLuminanceLast);
	DeleteMem(m_SceneRT);
	DeleteMem(m_HDREffect);
}

void HDRPostProcess::Initialize()
{
	//Create Render Targets
	m_SceneRT = RenderTarget::CreateRenderTargetInst();
	m_SceneScaledRT = RenderTarget::CreateRenderTargetInst();
	m_BrightPass = RenderTarget::CreateRenderTargetInst();
	m_StartSource = RenderTarget::CreateRenderTargetInst();
	m_BloomSource = RenderTarget::CreateRenderTargetInst();
	m_AdaptedLuminanceCur = RenderTarget::CreateRenderTargetInst();
	m_AdaptedLuminanceLast = RenderTarget::CreateRenderTargetInst();

	uint32_t i = 0;

	for(i = 0; i < AE_HDR_MAX_TONE_MAP_TEXS; ++i)
	{
		m_ToneMapTex[i] = RenderTarget::CreateRenderTargetInst();
	}

	for(i = 0; i < AE_HDR_MAX_BLOOM_TEXS; ++i)
	{
		m_BloomTex[i] = RenderTarget::CreateRenderTargetInst();
	}

	for(i = 0; i < AE_HDR_MAX_START_TEXS; ++i)
	{
		m_StartTexs[i] = RenderTarget::CreateRenderTargetInst();
	}

	//Create HDR Effect
	m_HDREffect = new HDRPostProcessMaterial();

	//Get DR
	if(m_UseDREndRT)
	{
		m_DREnd = static_cast<DeferredRenderingEnd*>(m_GameApp->GetGameService(m_DeferredEndServiceName));
		assert(m_DREnd);
	}
}

void HDRPostProcess::LoadContent()
{
	//Load Effect
	m_HDREffect->LoadMaterial(m_GraphicDevice);
	InitEffectParams();

	uint32_t width = m_GraphicDevice->GetGraphicPP().BackBufferWidth;
	uint32_t height = m_GraphicDevice->GetGraphicPP().BackBufferHeight;

	// Crop the scene texture so width and height are evenly divisible by 8.
	// This cropped version of the scene will be used for post processing effects,
	// and keeping everything evenly divisible allows precise control over
	// sampling points within the shaders.
	m_CropWidth = width - (width % 8);
	m_CropHeight = height - (height % 8);

	//Construct Render Tartgets

	// Create the HDR scene texture
	m_SceneRT->CreateRenderTargetTexture(m_GraphicDevice, "Scene RT", width, height, AE_POOL_DEFAULT, AE_PX_FMT_A16B16G16R16F);

	// Scaled version of the HDR scene texture
	m_SceneScaledRT->CreateRenderTargetTexture(m_GraphicDevice, "Scene Scaled", m_CropWidth / 4, m_CropHeight / 4, AE_POOL_DEFAULT, AE_PX_FMT_A16B16G16R16F);

	// Create the bright-pass filter texture. 
	// Texture has a black border of single texel thickness to fake border 
	// addressing using clamp addressing
	m_BrightPass->CreateRenderTargetTexture(m_GraphicDevice, "Bright Pass", m_CropWidth / 4 + 2, m_CropHeight / 4 + 2, AE_POOL_DEFAULT, AE_PX_FMT_A8R8G8B8);

	// Create a texture to be used as the source for the star effect
	// Texture has a black border of single texel thickness to fake border 
	// addressing using clamp addressing
	m_StartSource->CreateRenderTargetTexture(m_GraphicDevice, "Star Source", m_CropWidth / 8 + 2, m_CropHeight / 8 + 2, AE_POOL_DEFAULT, AE_PX_FMT_A8R8G8B8);

	// Create a texture to be used as the source for the bloom effect
	// Texture has a black border of single texel thickness to fake border 
	// addressing using clamp addressing
	m_BloomSource->CreateRenderTargetTexture(m_GraphicDevice, "Bloom Source", m_CropWidth / 8 + 2, m_CropHeight / 8 + 2, AE_POOL_DEFAULT, AE_PX_FMT_A8R8G8B8);

	// Create a 2 textures to hold the luminance that the user is currently adapted
	// to. This allows for a simple simulation of light adaptation.
	AETODO("R16F can be R32F if device does not support R16F");
	m_AdaptedLuminanceCur->CreateRenderTargetTexture(m_GraphicDevice, "Adaptive Luminance Current", 1, 1, AE_POOL_DEFAULT, AE_PX_FMT_R16F);
	m_AdaptedLuminanceLast->CreateRenderTargetTexture(m_GraphicDevice, "Adaptive Luminance Last", 1, 1, AE_POOL_DEFAULT, AE_PX_FMT_R16F);

	uint32_t i = 0;

	// For each scale stage, create a texture to hold the intermediate results
	// of the luminance calculation
	for(i = 0; i < AE_HDR_MAX_TONE_MAP_TEXS; ++i)
	{
		int sampleLen = 1 << ( 2 * i );

		AETODO("R16F can be R32F if device does not support R16F");
		m_ToneMapTex[i]->CreateRenderTargetTexture(m_GraphicDevice, "Tone Map", sampleLen, sampleLen, AE_POOL_DEFAULT, AE_PX_FMT_R16F);
	}

	// Create the temporary blooming effect textures
	// Texture has a black border of single texel thickness to fake border 
	// addressing using clamp addressing
	for(i = 1; i < AE_HDR_MAX_BLOOM_TEXS; ++i)
	{
		m_BloomTex[i]->CreateRenderTargetTexture(m_GraphicDevice, "Bloom Tex", m_CropWidth / 8 + 2, m_CropHeight / 8 + 2, AE_POOL_DEFAULT, AE_PX_FMT_A8R8G8B8);
	}

	//Create the final blooming effect texture
	m_BloomTex[0]->CreateRenderTargetTexture(m_GraphicDevice, "Final Bloom", m_CropWidth / 8, m_CropHeight / 8, AE_POOL_DEFAULT, AE_PX_FMT_A8R8G8B8);

	//Create the star effect textures
	for(i = 0; i < AE_HDR_MAX_START_TEXS; ++i)
	{
		m_StartTexs[i]->CreateRenderTargetTexture(m_GraphicDevice, "Start Texs", m_CropWidth / 4, m_CropHeight / 4, AE_POOL_DEFAULT, AE_PX_FMT_A16B16G16R16F);
	}

	//Textures with borders must be cleared since scissor rect testing will
	//be used to avoid rendering on top of the border
	ClearTextures();
}

void HDRPostProcess::InitEffectParams()
{
	m_HDREffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel());
	m_HDREffect->SetBloomScale(1.0f);
    m_HDREffect->SetStarScale(0.5f);
	m_HDREffect->SetEnableBlueShift(false);
	m_HDREffect->SetEnableToneMap(true);
}

void HDRPostProcess::ClearTextures()
{
	m_GraphicDevice->ColorFill(m_AdaptedLuminanceCur, XMCOLORTransparent);
	m_GraphicDevice->ColorFill(m_AdaptedLuminanceLast, XMCOLORTransparent);
	m_GraphicDevice->ColorFill(m_BloomSource, XMCOLORTransparent);
	m_GraphicDevice->ColorFill(m_BrightPass, XMCOLORTransparent);
	m_GraphicDevice->ColorFill(m_StartSource, XMCOLORTransparent);

	for(uint32_t i = 0; i < AE_HDR_MAX_BLOOM_TEXS; ++i)
	{
		m_GraphicDevice->ColorFill(m_BloomTex[i], XMCOLORTransparent);
	}
}

void HDRPostProcess::Update(const TimerParams& timerParams)
{
	// Set the flag to refresh the user's simulated adaption level.
	// Frame move is not called when the scene is paused or single-stepped. 
	// If the scene is paused, the user's adaptation level needs to remain
	// unchanged.
	m_AdaptationInvalid = true;
}

void HDRPostProcess::Render(const TimerParams& timerParams)
{
	if(m_DREnd)
	{
		//Set Deferred Rendering End Scene as Scene for HDR
		m_GraphicDevice->CaptureFrameBuffer(m_DREnd->GetFinalRT(), m_SceneRT);
	}
	else
	{
		//Capture Scene Texture
		uint32_t stage = 0;
		m_GraphicDevice->CaptureFrameBuffer(stage, m_SceneRT);
	}

	//Step 1: Scale the Scene
	ScaleScene();

	//Step 2: Measure Luminance
	MeasureLuminance();

	//Step 3: User Adaptation Level Change update
	if(m_AdaptationInvalid)
	{
		m_AdaptationInvalid = false;

		// Calculate the current luminance adaptation level
		CalculateAdaptation(timerParams);
	}

	//Step 4: Now that luminance information has been gathered, the scene can be bright-pass filtered
	//to remove everything except bright lights and reflections.
	ScaledToBrightPass();

	//Step 5: Blur the bright-pass filtered image to create the source texture for the star effect
	BrightPassStarSource();

	//Step 6: Scale-down the source texture for the star effect to create the source texture
	//for the bloom effect
	StarSourceBloomSource();

	//Step 7: Render Bloom
	RenderBloom();

	//Step 8: Render Star
	RenderStar();

	//Step 9: Render Final Pass
	RenderFinalPass();
}

void HDRPostProcess::RenderFinalPass()
{
	// Draw the high dynamic range scene texture to the low dynamic range
	// back buffer. As part of this final pass, the scene will be tone-mapped
	// using the user's current adapted luminance, blue shift will occur
	// if the scene is determined to be very dark, and the post-process lighting
	// effect textures will be added to the scene.
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_FINAL_SCENE_PASS);
	m_HDREffect->SetMiddleGray(m_KeyValue);

	m_GraphicDevice->SetRenderTarget(0, nullptr);
	m_GraphicDevice->SetTexture(0, m_SceneRT);
	m_GraphicDevice->SetTexture(1, m_BloomTex[0]);
	m_GraphicDevice->SetTexture(2, m_StartTexs[0]);
	m_GraphicDevice->SetTexture(3, m_AdaptedLuminanceCur);
	
	GraphicSamplerState samplerState;
	samplerState.m_MagFilter = AE_TF_POINT;
	samplerState.m_MinFilter = AE_TF_POINT;
	m_GraphicDevice->SetSamplerState(0, samplerState);
	m_GraphicDevice->SetSamplerState(3, samplerState);

	samplerState.m_MagFilter = AE_TF_LINEAR;
	samplerState.m_MinFilter = AE_TF_LINEAR;
	m_GraphicDevice->SetSamplerState(1, samplerState);
	m_GraphicDevice->SetSamplerState(2, samplerState);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();
	
	m_HDREffect->EndPass();
	m_HDREffect->End();
	
	m_GraphicDevice->SetTexture(0, (RenderTarget*)nullptr);
	m_GraphicDevice->SetTexture(1, (RenderTarget*)nullptr);
	m_GraphicDevice->SetTexture(2, (RenderTarget*)nullptr);
	m_GraphicDevice->SetTexture(3, (RenderTarget*)nullptr);
}

void HDRPostProcess::RenderStar()
{
	//Loop variables
	uint32_t i = 0;
	uint32_t d = 0;
	uint32_t p = 0;
	uint32_t s = 0;

	//Dest Star RT
	RenderTarget* starDest = nullptr;

	// Clear the star texture
	m_GraphicDevice->ColorFill(m_StartTexs[0], XMCOLORTransparent);

	// Avoid rendering the star if it's not being used in the current glare
	if(m_GlareProp.m_GlareLuminance <= 0.0f || m_GlareProp.m_StarLuminance <= 0.0f)
	{
		return;
	}

	// Initialize the constants used during the effect
	const StarProp& starDef = m_GlareProp.m_StarProp;

	const float tanFoV = atanf( XM_PI / 8 );
	const XMFLOAT4 white(1.0f, 1.0f, 1.0f, 1.0f);

	static const int s_maxPasses = 3;
	static const int nSamples = 8;

	static XMFLOAT4 s_aaColor[s_maxPasses][8];
	static const XMCOLOR s_colorWhite(0.63f, 0.63f, 0.63f, 0.0f);

	XMFLOAT4 sampleWeights[AE_HDR_MAX_SAMPLES];
	XMFLOAT2 sampleOffsets[AE_HDR_MAX_SAMPLES];

	AETODO("LOOK INTO THIS STATE CHANGE");
	static const GraphicBlendState blendState = GraphicBlendState(true, false, AE_BLENDOP_ADD, AE_BLEND_ONE, AE_BLEND_ONE);
	m_GraphicDevice->SetBlendState(blendState);

	// Get the source texture dimensions
	float srcW = (float)m_StartSource->GetWidth();
	float srcH = (float)m_StartSource->GetHeight();

	for(p = 0; p < s_maxPasses; ++p)
	{
		float ratio = (float)(p + 1) / (float)s_maxPasses;

		for(s = 0; s < nSamples; ++s)
		{
			XMCOLOR chromaticAberrColor = XMCOLORTransparent;
			XEColorHelper::ColorLerp(StarProp::m_ChromaticAberrationColor[s], s_colorWhite, ratio, chromaticAberrColor);

			XMCOLOR res = XMCOLORTransparent;
			XEColorHelper::ColorLerp(s_colorWhite, chromaticAberrColor, m_GlareProp.m_ChromaticAberration, res);

			XEMathHelperVector::GetVector4FromColor(res, s_aaColor[p][s]);
		}
	}

	float radOffset = m_GlareProp.m_StarInclination + starDef.m_Inclination;

	RenderTarget* texSource = nullptr;

	// Direction loop
	for(d = 0; d < starDef.m_StarLineNum; ++d)
	{
		const StarLines& starLine = starDef.m_StarLines[d];

		texSource = m_StartSource;

		float rad = radOffset + starLine.m_Inclination;
		float sn = sinf(rad);
		float cs = cosf(rad);

		XMFLOAT2 stepUV;
		stepUV.x = sn / srcW * starLine.m_SampleLength;
		stepUV.y = cs / srcH * starLine.m_SampleLength;

		float attnPowScale = (tanFoV + 0.1f) * 1.0f * ( 160.0f + 120.0f ) / ( srcW + srcH ) * 1.2f;

		// 1 direction expansion loop
		int32_t workTexture = 1;

		for(p = 0; p < starLine.m_Passes; ++p)
		{
			if(p == starLine.m_Passes - 1)
			{
				// Last pass move to other work buffer
				starDest = m_StartTexs[d + 4];
			}
			else
			{
				starDest = m_StartTexs[workTexture];
			}

			// Sampling configration for each stage
			for(i = 0; i < nSamples; ++i)
			{
				float lum = powf( starLine.m_Attenuation, attnPowScale * i );
				float mul = lum * ( p + 1.0f ) * 0.5f;

				XEMathHelperVector::MultiplyVector4(s_aaColor[starLine.m_Passes - 1 - p][i], mul, sampleWeights[i]);

				// Offset of sampling coordinate
				sampleOffsets[i].x = stepUV.x * i;
				sampleOffsets[i].y = stepUV.y * i;

				if( fabs( sampleOffsets[i].x ) >= 0.9f || fabs( sampleOffsets[i].y ) >= 0.9f )
				{
					sampleOffsets[i].x = 0.0f;
					sampleOffsets[i].y = 0.0f;

					XEMathHelperVector::MultiplyVector4(sampleWeights[i], 0.0f, sampleWeights[i]);
				}
			}

			m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_STAR);

			m_HDREffect->SetSampleOffsets(sampleOffsets);
			m_HDREffect->SetSampleWeights(sampleWeights);

			m_GraphicDevice->SetRenderTarget( 0, starDest );
			m_GraphicDevice->SetTexture( 0, texSource );

			GraphicSamplerState samplerState;
			samplerState.m_MinFilter = AE_TF_LINEAR;
			samplerState.m_MagFilter = AE_TF_LINEAR;
			m_GraphicDevice->SetSamplerState(0, samplerState);

			m_HDREffect->Begin();
			m_HDREffect->BeginPass(0);

			m_GraphicDevice->DrawFullScreenQuad();

			m_HDREffect->EndPass();
			m_HDREffect->End();

			// Setup next expansion
			XEMathHelperVector::MultiplyVector2(stepUV, (float)nSamples, stepUV);
			attnPowScale *= nSamples;

			// Set the work drawn just before to next texture source.
			texSource = m_StartTexs[workTexture];

			++workTexture;
			if( workTexture > 2 )
			{
				workTexture = 1;
			}
		}
	}

	starDest = m_StartTexs[0];

	for(i = 0; i < starDef.m_StarLineNum; ++i)
	{
		m_GraphicDevice->SetTexture(i, m_StartTexs[i + 4]);

		GraphicSamplerState samplerState;
		samplerState.m_MinFilter = AE_TF_LINEAR;
		samplerState.m_MagFilter = AE_TF_LINEAR;
		m_GraphicDevice->SetSamplerState(i, samplerState);

		XMFLOAT4 res = XMFLOAT4ZERO;

		XEMathHelperVector::MultiplyVector4(white, 1.0f, res);
		XEMathHelperVector::DivideVector4(res, (float)starDef.m_StarLineNum, res);

		sampleWeights[i] = res;
	}

	m_HDREffect->ActivateMergeTechnique(starDef.m_StarLineNum);
	m_HDREffect->SetSampleWeights(sampleWeights);

	m_GraphicDevice->SetRenderTarget(0, starDest);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_HDREffect->EndPass();
	m_HDREffect->End();

	for(i = 0; i < starDef.m_StarLineNum; ++i)
	{
		m_GraphicDevice->SetTexture(i, (RenderTarget*)nullptr);
	}

	m_GraphicDevice->SetBlendState(GraphicBlendState::DefaultState);
}

void HDRPostProcess::RenderBloom()
{
	GraphicSamplerState samplerState;

	XMFLOAT4 sampleWeights[AE_HDR_MAX_SAMPLES];
	XMFLOAT2 sampleOffsets[AE_HDR_MAX_SAMPLES];
	float sampleOffsetsFloats[AE_HDR_MAX_SAMPLES];

	// Clear the bloom texture
	m_GraphicDevice->ColorFill(m_BloomTex[0], XMCOLORTransparent);

	if( m_GlareProp.m_GlareLuminance <= 0.0f || m_GlareProp.m_BloomLuminance <= 0.0f )
	{
		return;
	}

	RECT rectSrc = {0, 0, 0, 0};
	GetTextureRect(m_BloomSource, rectSrc);
	InflateRect(&rectSrc, -1, -1);

	RECT rectDest = {0, 0, 0, 0};
	GetTextureRect(m_BloomTex[2], rectDest);
	InflateRect(&rectDest, -1, -1);

	XMFLOAT4 coords;
	GetTextCoord(m_BloomSource, &rectSrc, m_BloomTex[2], &rectDest, coords);

	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_GUASS_BLUR_5x5);

	GetSampleOffsetsGaussBlur5x5(m_BloomSource->GetWidth(), m_BloomSource->GetHeight(), sampleOffsets, sampleWeights, 1.0f);

	m_HDREffect->SetSampleOffsets(sampleOffsets);
	m_HDREffect->SetSampleWeights(sampleWeights);

	m_GraphicDevice->SetRenderTarget(0, m_BloomTex[2]);
	m_GraphicDevice->SetTexture(0, m_BloomSource);

	m_GraphicDevice->SetScissorRect(rectDest);

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::ScissorEnableState);

	samplerState.m_MinFilter = AE_TF_POINT;
	samplerState.m_MagFilter = AE_TF_POINT;
	m_GraphicDevice->SetSamplerState(0, samplerState);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad(coords);

	m_HDREffect->EndPass();
	m_HDREffect->End();

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::DefaultState);

	GetSampleOffsetsBloom(m_BloomTex[2]->GetWidth(), sampleOffsetsFloats, sampleWeights, 3.0f, 2.0f);

	for(uint32_t i = 0; i < AE_HDR_MAX_SAMPLES; ++i)
	{
		sampleOffsets[i] = XMFLOAT2(sampleOffsetsFloats[i], 0.0f);
	}

	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_BLOOM);
	m_HDREffect->SetSampleOffsets(sampleOffsets);
	m_HDREffect->SetSampleWeights(sampleWeights);

	m_GraphicDevice->SetRenderTarget(0, m_BloomTex[1]);
	m_GraphicDevice->SetTexture(0, m_BloomTex[2]);

	m_GraphicDevice->SetScissorRect(rectDest);

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::ScissorEnableState);

	samplerState.m_MinFilter = AE_TF_POINT;
	samplerState.m_MagFilter = AE_TF_POINT;
	m_GraphicDevice->SetSamplerState(0, samplerState);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad(coords);

	m_HDREffect->EndPass();
	m_HDREffect->End();

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::DefaultState);

	GetSampleOffsetsBloom(m_BloomTex[1]->GetHeight(), sampleOffsetsFloats, sampleWeights, 3.0f, 2.0f);
	for(uint32_t j = 0; j < AE_HDR_MAX_SAMPLES; ++j)
	{
		sampleOffsets[j] = XMFLOAT2(0.0f, sampleOffsetsFloats[j]);
	}

	GetTextureRect(m_BloomTex[1], rectSrc);
	InflateRect(&rectSrc, -1, -1);

	GetTextCoord(m_BloomTex[1], &rectSrc, m_BloomTex[0], nullptr, coords);

	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_BLOOM);
	m_HDREffect->SetSampleOffsets(sampleOffsets);
	m_HDREffect->SetSampleWeights(sampleWeights);

	m_GraphicDevice->SetRenderTarget(0, m_BloomTex[0]);
	m_GraphicDevice->SetTexture(0, m_BloomTex[1]);

	samplerState.m_MinFilter = AE_TF_POINT;
	samplerState.m_MagFilter = AE_TF_POINT;
	m_GraphicDevice->SetSamplerState(0, samplerState);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad(coords);

	m_HDREffect->EndPass();
	m_HDREffect->End();
}

void HDRPostProcess::StarSourceBloomSource()
{
	XMFLOAT2 sampleOffsets[AE_HDR_MAX_SAMPLES];

	// Get the rectangle describing the sampled portion of the source texture.
	// Decrease the rectangle to adjust for the single pixel black border.
	RECT rectSrc = {0, 0, 0, 0};
	GetTextureRect(m_StartSource, rectSrc);
	InflateRect(&rectSrc, -1, -1);

	// Get the destination rectangle.
	// Decrease the rectangle to adjust for the single pixel black border.
	RECT rectDest = {0, 0, 0, 0};
	GetTextureRect(m_BloomSource, rectDest);
	InflateRect(&rectDest, -1, -1);

	// Get the correct texture coordinates to apply to the rendered quad in order 
	// to sample from the source rectangle and render into the destination rectangle
	XMFLOAT4 coords;
	GetTextCoord(m_StartSource, &rectSrc, m_BloomSource, &rectDest, coords);

	GetSampleOffsetsDS2x2(m_BrightPass->GetWidth(), m_BrightPass->GetHeight(), sampleOffsets);
	m_HDREffect->SetSampleOffsets(sampleOffsets);

	// Create an exact 1/2 x 1/2 copy of the source texture
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_DOWN_SCALE_2x2);

	m_GraphicDevice->SetRenderTarget(0, m_BloomSource);
	m_GraphicDevice->SetTexture(0, m_StartSource);

	m_GraphicDevice->SetScissorRect(rectDest);

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::ScissorEnableState);

	GraphicSamplerState samp;
	samp.m_MinFilter = AE_TF_POINT;
	samp.m_AddressU = AE_TAM_CLAMP;
	samp.m_AddressV = AE_TAM_CLAMP;
	m_GraphicDevice->SetSamplerState(0, samp);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad(coords);

	m_HDREffect->EndPass();
	m_HDREffect->End();

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::DefaultState);
}

void HDRPostProcess::BrightPassStarSource()
{
	XMFLOAT2 sampleOffsets[AE_HDR_MAX_SAMPLES];
	XMFLOAT4 sampleWeights[AE_HDR_MAX_SAMPLES];

	// Get the destination rectangle.
	// Decrease the rectangle to adjust for the single pixel black border.
	RECT rectDest = {0, 0, 0, 0};
	GetTextureRect(m_StartSource, rectDest);
	InflateRect(&rectDest, -1, -1);

	//Get the correct texture coordinates to apply to the rendered quad in order 
	//to sample from the source rectangle and render into the destination rectangle
	XMFLOAT4 coords(XMFLOAT4ZERO);
	GetTextCoord(m_BrightPass, nullptr, m_StartSource, &rectDest, coords);

	// Get the sample offsets used within the pixel shader
	GetSampleOffsetsGaussBlur5x5(m_BrightPass->GetWidth(), m_BrightPass->GetHeight(), sampleOffsets, sampleWeights);
	m_HDREffect->SetSampleOffsets(sampleOffsets);
	m_HDREffect->SetSampleWeights(sampleWeights);

	// The gaussian blur smooths out rough edges to avoid aliasing effects
	// when the star effect is run
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_GUASS_BLUR_5x5);

	m_GraphicDevice->SetRenderTarget(0, m_StartSource);
	m_GraphicDevice->SetTexture(0, m_BrightPass);

	m_GraphicDevice->SetScissorRect(rectDest);

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::ScissorEnableState);

	GraphicSamplerState samp;
	samp.m_MinFilter = AE_TF_POINT;
	samp.m_AddressU = AE_TAM_CLAMP;
	samp.m_AddressV = AE_TAM_CLAMP;
	m_GraphicDevice->SetSamplerState(0, samp);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad(coords);

	m_HDREffect->EndPass();
	m_HDREffect->End();

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::DefaultState);
}

void HDRPostProcess::ScaledToBrightPass()
{	
	// Get the rectangle describing the sampled portion of the source texture.
	// Decrease the rectangle to adjust for the single pixel black border.
	RECT rectSrc = {0, 0, 0, 0};
	GetTextureRect(m_SceneScaledRT, rectSrc);
	InflateRect(&rectSrc, -1, -1);

	// Get the destination rectangle.
	// Decrease the rectangle to adjust for the single pixel black border.
	RECT rectDest = {0, 0, 0, 0};
	GetTextureRect(m_BrightPass, rectDest );
	InflateRect(&rectDest, -1, -1);

	// Get the correct texture coordinates to apply to the rendered quad in order 
	// to sample from the source rectangle and render into the destination rectangle
	XMFLOAT4 coords;
	GetTextCoord(m_SceneScaledRT, &rectSrc, m_BrightPass, &rectDest, coords);

	// The bright-pass filter removes everything from the scene except lights and
	// bright reflections
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_BRIGHT_PASS_FILTER);

	m_GraphicDevice->SetRenderTarget(0, m_BrightPass);

	m_GraphicDevice->SetTexture(0, m_SceneScaledRT);
	m_GraphicDevice->SetTexture(1, m_AdaptedLuminanceCur);

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::ScissorEnableState);

	m_GraphicDevice->SetScissorRect(rectDest);

	GraphicSamplerState samp;
	samp.m_MagFilter = AE_TF_POINT;
	samp.m_MinFilter = AE_TF_POINT;
	m_GraphicDevice->SetSamplerState(0, samp);
	m_GraphicDevice->SetSamplerState(1, samp);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad(coords);

	m_HDREffect->EndPass();
	m_HDREffect->End();

	m_GraphicDevice->SetRasterizerState(GraphicRasterizerState::DefaultState);
}

void HDRPostProcess::CalculateAdaptation(const TimerParams& timerParams)
{
	// Swap current & last luminance
	RenderTarget* swap = m_AdaptedLuminanceLast;
	m_AdaptedLuminanceLast = m_AdaptedLuminanceCur;
	m_AdaptedLuminanceCur = swap;

	// This simulates the light adaptation that occurs when moving from a 
	// dark area to a bright area, or vice versa. The g_pTexAdaptedLuminance
	// texture stores a single texel cooresponding to the user's adapted 
	// level.
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_CALCULATE_ADAPTED_LUM);
	m_HDREffect->SetElapsedTime((float)timerParams.m_ElapsedTime);

	m_GraphicDevice->SetRenderTarget(0, m_AdaptedLuminanceCur);
	m_GraphicDevice->SetTexture(0, m_AdaptedLuminanceLast);
	m_GraphicDevice->SetTexture(1, m_ToneMapTex[0]);

	GraphicSamplerState samp;
	samp.m_MagFilter = AE_TF_POINT;
	samp.m_MinFilter = AE_TF_POINT;

	m_GraphicDevice->SetSamplerState(0, samp);
	m_GraphicDevice->SetSamplerState(1, samp);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_HDREffect->EndPass();
	m_HDREffect->End();
}

void HDRPostProcess::MeasureLuminance()
{
	GraphicSamplerState samp;
	XMFLOAT2 sampleOffsets[AE_HDR_MAX_SAMPLES];

	uint32_t curTexture = AE_HDR_MAX_TONE_MAP_TEXS - 1;

	// Sample log average luminance

	// Initialize the sample offsets for the initial luminance pass.
	float tU, tV;
	tU = 1.0f / ( 3.0f * m_ToneMapTex[curTexture]->GetWidth() );
	tV = 1.0f / ( 3.0f * m_ToneMapTex[curTexture]->GetHeight() );

	uint32_t index = 0;
	for(uint32_t x = -1; x <= 1; ++x)
	{
		for(uint32_t y = -1; y <= 1; ++y)
		{
			sampleOffsets[index].x = x * tU;
			sampleOffsets[index].y = y * tV;

			index++;
		}
	}

	// After this pass, the g_apTexToneMap[NUM_TONEMAP_TEXTURES-1] texture will contain
	// a scaled, grayscale copy of the HDR scene. Individual texels contain the log 
	// of average luminance values for points sampled on the HDR texture.
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_SAMPLE_AVG_LUM);
	m_HDREffect->SetSampleOffsets(sampleOffsets);

	m_GraphicDevice->SetRenderTarget(0, m_ToneMapTex[curTexture]);

	m_GraphicDevice->SetTexture(0, m_SceneScaledRT);

	samp.m_MagFilter = AE_TF_LINEAR;
	samp.m_MinFilter = AE_TF_LINEAR;
	m_GraphicDevice->SetSamplerState(0, samp);
	m_GraphicDevice->SetSamplerState(1, samp);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_HDREffect->EndPass();
	m_HDREffect->End();

	--curTexture;

	// Initialize the sample offsets for the iterative luminance passes
	while( curTexture > 0 )
	{
		GetSampleOffsetsDS4x4(m_ToneMapTex[curTexture + 1]->GetWidth(), m_ToneMapTex[curTexture + 1]->GetHeight(), sampleOffsets);

		// Each of these passes continue to scale down the log of average
		// luminance texture created above, storing intermediate results in 
		// g_apTexToneMap[1] through g_apTexToneMap[NUM_TONEMAP_TEXTURES-1].
		m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_RESAMPLE_AVG_LUM);
		m_HDREffect->SetSampleOffsets(sampleOffsets);

		m_GraphicDevice->SetRenderTarget(0, m_ToneMapTex[curTexture]);

		m_GraphicDevice->SetTexture(0, m_ToneMapTex[curTexture + 1]);

		samp.m_MagFilter = AE_TF_POINT;
		samp.m_MinFilter = AE_TF_POINT;
		m_GraphicDevice->SetSamplerState(0, samp);

		m_HDREffect->Begin();
		m_HDREffect->BeginPass(0);

		m_GraphicDevice->DrawFullScreenQuad();

		m_HDREffect->EndPass();
		m_HDREffect->End();

		--curTexture;
	}

	// Downsample to 1x1
	GetSampleOffsetsDS4x4( m_ToneMapTex[1]->GetWidth(), m_ToneMapTex[1]->GetHeight(), sampleOffsets );

	// Perform the final pass of the average luminance calculation. This pass
	// scales the 4x4 log of average luminance texture from above and performs
	// an exp() operation to return a single texel cooresponding to the average
	// luminance of the scene in g_apTexToneMap[0].
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_RESAMPLE_AVG_LUM_EXP);
	m_HDREffect->SetSampleOffsets(sampleOffsets);

	m_GraphicDevice->SetRenderTarget(0, m_ToneMapTex[0]);

	m_GraphicDevice->SetTexture(0, m_ToneMapTex[1]);

	samp.m_MagFilter = AE_TF_POINT;
	samp.m_MinFilter = AE_TF_POINT;
	m_GraphicDevice->SetSamplerState(0, samp);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_HDREffect->EndPass();
	m_HDREffect->End();
}

void HDRPostProcess::ScaleScene()
{
	XMFLOAT2 sampleOffsets[AE_HDR_MAX_SAMPLES];

	uint32_t width = m_GraphicDevice->GetGraphicPP().BackBufferWidth;
	uint32_t height = m_GraphicDevice->GetGraphicPP().BackBufferHeight;

	// Create a 1/4 x 1/4 scale copy of the HDR texture. Since bloom textures
	// are 1/8 x 1/8 scale, border texels of the HDR texture will be discarded 
	// to keep the dimensions evenly divisible by 8; this allows for precise 
	// control over sampling inside pixel shaders.
	m_HDREffect->ActivateTechnique(HDRPostProcessMaterial::AE_HDR_DOWN_SCALE_4x4);

	// Place the rectangle in the center of the back buffer surface
	RECT rectSrc = {0, 0, 0, 0};
	rectSrc.left = (width - m_CropWidth) / 2;
	rectSrc.top = (height - m_CropHeight) / 2;
	rectSrc.right = rectSrc.left + m_CropWidth;
	rectSrc.bottom = rectSrc.top + m_CropHeight;

	// Get the texture coordinates for the render target
	XMFLOAT4 coords;
	GetTextCoord(m_SceneRT, &rectSrc, m_SceneScaledRT, nullptr, coords);

	// Get the sample offsets used within the pixel shader
	GetSampleOffsetsDS4x4(width, height, sampleOffsets);
	m_HDREffect->SetSampleOffsets(sampleOffsets);

	m_GraphicDevice->SetRenderTarget(0, m_SceneScaledRT);

	m_GraphicDevice->SetTexture(0, m_SceneRT);

	GraphicSamplerState samp;
	samp.m_MinFilter = AE_TF_POINT;
	samp.m_AddressU = AE_TAM_CLAMP;
	samp.m_AddressV = AE_TAM_CLAMP;
	m_GraphicDevice->SetSamplerState(0, samp);

	m_HDREffect->Begin();
	m_HDREffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad(coords);

	m_HDREffect->EndPass();
	m_HDREffect->End();
}

AEResult HDRPostProcess::GetTextureRect(RenderTarget* rt, RECT& rect)
{
	assert(rt);

	if(rt == nullptr)
	{
		return AEResult::Fail;
	}

	rect.left = 0;
	rect.top = 0;
	rect.right = rt->GetWidth();
	rect.bottom = rt->GetHeight();

	return AEResult::Ok;
}

void HDRPostProcess::GetTextCoord(RenderTarget* src, const RECT* rectSrc, RenderTarget* dst,  const RECT* rectDst, XMFLOAT4& texCoords)
{
	assert(src);
	assert(dst);

	float tU = 0.0f;
	float tV = 0.0f;

	// Start with a default mapping of the complete source surface to complete 
	// destination surface
	texCoords = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// If not using the complete source surface, adjust the coordinates
	if(rectSrc != nullptr)
	{
		// These delta values are the distance between source texel centers in 
		// texture address space
		tU = 1.0f / src->GetWidth();
		tV = 1.0f / src->GetHeight();

		texCoords.x += rectSrc->left * tU;
		texCoords.y += rectSrc->top * tV;
		texCoords.z -= ( src->GetWidth() - rectSrc->right ) * tU;
		texCoords.w -= ( src->GetHeight() - rectSrc->bottom ) * tV;
	}

	// If not drawing to the complete destination surface, adjust the coordinates
	if(rectDst != nullptr)
	{
		// These delta values are the distance between source texel centers in 
		// texture address space
		tU = 1.0f / dst->GetWidth();
		tV = 1.0f / dst->GetHeight();

		texCoords.x -= rectDst->left * tU;
		texCoords.y -= rectDst->top * tV;
		texCoords.w += ( dst->GetWidth() - rectDst->right ) * tU;
		texCoords.z += ( dst->GetHeight() - rectDst->bottom ) * tV;
	}
}

void HDRPostProcess::GetSampleOffsetsDS4x4(uint32_t width, uint32_t height, XMFLOAT2 sampleOffsets[])
{
	assert(sampleOffsets);

	float tU = 1.0f / width;
	float tV = 1.0f / height;

	// Sample from the 16 surrounding points. Since the center point will be in
	// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
	// center.
	uint32_t index = 0;
	for(uint32_t y = 0; y < 4; ++y)
	{
		for(uint32_t x = 0; x < 4; ++x)
		{
			sampleOffsets[index].x = (x - 1.5f) * tU;
			sampleOffsets[index].y = (y - 1.5f) * tV;

			++index;
		}
	}
}

void HDRPostProcess::GetSampleOffsetsDS2x2(uint32_t width, uint32_t height, XMFLOAT2 sampleOffsets[])
{
	assert(sampleOffsets);

	float tU = 1.0f / width;
	float tV = 1.0f / height;

	// Sample from the 4 surrounding points. Since the center point will be in
	// the exact center of 4 texels, a 0.5f offset is needed to specify a texel
	// center.
	uint32_t index = 0;
	for(uint32_t y = 0; y < 2; y++ )
	{
		for(uint32_t x = 0; x < 2; x++ )
		{
			sampleOffsets[ index ].x = (x - 0.5f) * tU;
			sampleOffsets[ index ].y = (y - 0.5f) * tV;

			index++;
		}
	}
}

void HDRPostProcess::GetSampleOffsetsGaussBlur5x5(uint32_t width, uint32_t height, XMFLOAT2 texCoordOffset[], XMFLOAT4 sampleWeight[], float multiplier)
{
	float tu = 1.0f / ( float )width;
	float tv = 1.0f / ( float )height;

	XMFLOAT4 white(1.0f, 1.0f, 1.0f, 1.0f);

	float totalWeight = 0.0f;
	uint32_t index = 0;
	for(int32_t x = -2; x <= 2; x++ )
	{
		for(int32_t y = -2; y <= 2; y++ )
		{
			AETODO("Look into this")
				// Exclude pixels with a block distance greater than 2. This will
				// create a kernel which approximates a 5x5 kernel using only 13
				// sample points instead of 25; this is necessary since 2.0 shaders
				// only support 16 texture grabs.
				if(fabsl(x) + fabsl(y) > 2)
				{
					continue;
				}

				// Get the unscaled Gaussian intensity for this offset
				texCoordOffset[index] = XMFLOAT2(x * tu, y * tv);

				XEMathHelperVector::MultiplyVector4(white, AE_Base::GaussianDistribution((float)x, (float)y, 1.0f), sampleWeight[index]);

				totalWeight += sampleWeight[index].x;

				index++;
		}
	}

	// Divide the current weight by the total weight of all the samples; Gaussian
	// blur kernels add to 1.0f to ensure that the intensity of the image isn't
	// changed when the blur occurs. An optional multiplier variable is used to
	// add or remove image intensity during the blur.
	for(uint32_t i = 0; i < index; i++ )
	{
		XEMathHelperVector::DivideVector4(sampleWeight[i], totalWeight, sampleWeight[i]);
		XEMathHelperVector::MultiplyVector4(sampleWeight[i], multiplier, sampleWeight[i]);
	}
}

void HDRPostProcess::GetSampleOffsetsBloom(uint32_t texSize, float texCoordOffset[15], XMFLOAT4 colorWeight[], float deviation, float multiplier)
{
	float tu = 1.0f / ( float )texSize;

	// Fill the center texel
	float weight = multiplier * AE_Base::GaussianDistribution(0.0f, 0.0f, deviation);
	colorWeight[0] = XMFLOAT4(weight, weight, weight, 1.0f);

	texCoordOffset[0] = 0.0f;

	uint32_t i = 0;

	// Fill the first half
	for( i = 1; i < 8; i++ )
	{
		// Get the Gaussian intensity for this offset
		weight = multiplier * AE_Base::GaussianDistribution((float)i, 0.0f, deviation );
		texCoordOffset[i] = i * tu;

		colorWeight[i] = XMFLOAT4(weight, weight, weight, 1.0f);
	}

	// Mirror to the second half
	for( i = 8; i < 15; i++ )
	{
		colorWeight[i] = colorWeight[i - 7];
		texCoordOffset[i] = -texCoordOffset[i - 7];
	}
}

void HDRPostProcess::OnLostDevice()
{
	m_HDREffect->OnLostDevice();
	m_SceneRT->OnLostDevice();
	m_SceneScaledRT->OnLostDevice();
	m_BrightPass->OnLostDevice();
	m_StartSource->OnLostDevice();
	m_BloomSource->OnLostDevice();
	m_AdaptedLuminanceCur->OnLostDevice();
	m_AdaptedLuminanceLast->OnLostDevice();

	uint32_t i = 0;

	for(i = 0; i < AE_HDR_MAX_TONE_MAP_TEXS; ++i)
	{
		m_ToneMapTex[i]->OnLostDevice();
	}

	for(i = 0; i < AE_HDR_MAX_BLOOM_TEXS; ++i)
	{
		m_BloomTex[i]->OnLostDevice();
	}

	for(i = 0; i < AE_HDR_MAX_START_TEXS; ++i)
	{
		m_StartTexs[i]->OnLostDevice();
	}
}

void HDRPostProcess::OnResetDevice()
{
	m_HDREffect->OnResetDevice();
	m_HDREffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel());

	m_SceneRT->OnResetDevice();

	uint32_t width = m_GraphicDevice->GetGraphicPP().BackBufferWidth;
	uint32_t height = m_GraphicDevice->GetGraphicPP().BackBufferHeight;
	m_CropWidth = width - (width % 8);
	m_CropHeight = height - (height % 8);

	m_SceneScaledRT->OnResetDevice(m_CropWidth / 4, m_CropHeight / 4);
	m_BrightPass->OnResetDevice(m_CropWidth / 4 + 2, m_CropHeight / 4 + 2);
	m_StartSource->OnResetDevice(m_CropWidth / 8 + 2, m_CropHeight / 8 + 2);
	m_BloomSource->OnResetDevice(m_CropWidth / 8 + 2, m_CropHeight / 8 + 2);
	m_AdaptedLuminanceCur->OnResetDevice(false);
	m_AdaptedLuminanceLast->OnResetDevice(false);

	uint32_t i = 0;

	for(i = 0; i < AE_HDR_MAX_TONE_MAP_TEXS; ++i)
	{
		m_ToneMapTex[i]->OnResetDevice(false);
	}

	for(i = 1; i < AE_HDR_MAX_BLOOM_TEXS; ++i)
	{
		m_BloomTex[i]->OnResetDevice(m_CropWidth / 8 + 2, m_CropHeight / 8 + 2);
	}

	m_BloomTex[0]->OnResetDevice(m_CropWidth / 8, m_CropHeight / 8);

	for(i = 0; i < AE_HDR_MAX_START_TEXS; ++i)
	{
		m_StartTexs[i]->OnResetDevice(m_CropWidth / 4, m_CropHeight / 4);
	}

	ClearTextures();
}
