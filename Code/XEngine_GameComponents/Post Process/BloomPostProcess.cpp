/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/8/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "BloomPostProcess.h"
#include "GameApp\GameApp.h"
#include "Utils\XETime.h"
#include "Materials\BloomPostProcessMaterial.h"
#include "Textures\RenderTarget.h"
#include "GraphicDevice.h"
#include "Math\XEMathDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BloomPostProcess::BloomPostProcess(GameApp* gameApp, const std::string& gameComponentName)
	: DrawableGameComponent(gameApp, gameComponentName)
	, m_SceneRT(nullptr)
	, m_RT1(nullptr)
	, m_RT2(nullptr)
	, m_BloomEffect(nullptr)
{
}

BloomPostProcess::BloomPostProcess(GameApp* gameApp, const BloomPostProcessSettings& settings, const std::string& gameComponentName)
	: DrawableGameComponent(gameApp, gameComponentName)
	, m_Settings(settings)
{
}

BloomPostProcess::~BloomPostProcess()
{
	DeleteMem(m_SceneRT);
	DeleteMem(m_RT1);
	DeleteMem(m_RT2);
	DeleteMem(m_BloomEffect);
}

void BloomPostProcess::Initialize()
{
	m_BloomEffect = new BloomPostProcessMaterial();
	m_SceneRT = RenderTarget::CreateRenderTargetInst();
	m_RT1 = RenderTarget::CreateRenderTargetInst();
	m_RT2 = RenderTarget::CreateRenderTargetInst();

	DrawableGameComponent::Initialize();
}

void BloomPostProcess::LoadContent()
{
	m_BloomEffect->LoadMaterial(m_GraphicDevice);

    uint32_t width = m_GraphicDevice->GetGraphicPP().BackBufferWidth;
    uint32_t height = m_GraphicDevice->GetGraphicPP().BackBufferHeight;

	m_SceneRT->CreateRenderTargetTexture(m_GraphicDevice, "Scene RT", width, height, XE_POOL_DEFAULT, XE_PX_FMT_A8R8G8B8);

	// Create two rendertargets for the bloom processing. These are half the
    // size of the backbuffer, in order to minimize fillrate costs. Reducing
    // the resolution in this way doesn't hurt quality, because we are going
    // to be blurring the bloom images in any case.	
	m_RT1->CreateRenderTargetTexture(m_GraphicDevice, "Temp Bloom RT 1", width / 2, height / 2, XE_POOL_DEFAULT, XE_PX_FMT_A8R8G8B8);
	m_RT2->CreateRenderTargetTexture(m_GraphicDevice, "Temp Bloom RT 2", width / 2, height / 2,	XE_POOL_DEFAULT, XE_PX_FMT_A8R8G8B8);

	DrawableGameComponent::LoadContent();
}

void BloomPostProcess::Update(const TimerParams& timerParams)
{
	DrawableGameComponent::Update(timerParams);
}

void BloomPostProcess::Render(const TimerParams& timerParams)
{
	uint32_t stage = 0;
	m_GraphicDevice->CaptureFrameBuffer(stage, m_SceneRT);

	m_BloomEffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel());

	// Pass 1: Draw the scene into PreBloom, using a
	// shader that extracts only the brightest parts of the image.
	RenderBloomExtract();

	// Pass 2: Apply a horizontal & Vertical gaussian blur filter.
	RenderBloomGaussianBlur();

	// Pass 3: Combine Bloom textures into 1 to produce the final bloomed result.
	RenderBloomCombine();

	DrawableGameComponent::Render(timerParams);
}

XEResult BloomPostProcess::RenderBloomExtract()
{	
	uint32_t passes = 0;

	m_GraphicDevice->SetRenderTarget(0, m_RT1);
	
	m_BloomEffect->SetSceneTexture(m_SceneRT);
	m_BloomEffect->SetBloomThreshold(m_Settings.m_BloomThreshold);
	
	m_BloomEffect->ActivateTechnique(BloomPostProcessMaterial::XE_BLOOM_EXTRACT);

	m_BloomEffect->Begin(passes);
	m_BloomEffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_BloomEffect->EndPass();
	m_BloomEffect->End();

	m_GraphicDevice->SetRenderTarget(0, nullptr);

	return XEResult::Ok;
}

XEResult BloomPostProcess::RenderBloomGaussianBlur()
{
	uint32_t passes = 0;

	//Do Gaussian Blur Horizontal
	m_GraphicDevice->SetRenderTarget(0, m_RT2);
	
	m_BloomEffect->ActivateTechnique(BloomPostProcessMaterial::XE_BLOOM_GAUSSIAN_BLUR);

	SetBlurParameters(1.0f / (float)m_RT1->GetWidth(), 0.0f);
	m_BloomEffect->SetGaussianBlurTexture(m_RT1);

	m_BloomEffect->Begin(passes);
	m_BloomEffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_BloomEffect->EndPass();
	m_BloomEffect->End();

	//Do Gaussian Blur Vertical
	m_GraphicDevice->SetRenderTarget(0, m_RT1);
	
	SetBlurParameters(0.0f, 1.0f / (float)m_RT1->GetHeight());
	m_BloomEffect->SetGaussianBlurTexture(m_RT2);

	m_BloomEffect->Begin(passes);
	m_BloomEffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_BloomEffect->EndPass();
	m_BloomEffect->End();

	m_GraphicDevice->SetRenderTarget(0, nullptr);

	return XEResult::Ok;
}

void BloomPostProcess::SetBlurParameters(float dx, float dy)
{
	// Create temporary arrays for computing our filter settings.
	float sampleWeights[XE_BLOOM_BLUR_SAMPLE_COUNT];
	XMFLOAT2 sampleOffsets[XE_BLOOM_BLUR_SAMPLE_COUNT];

	// The first sample always has a zero offset.
	sampleWeights[0] = XE_Base::GaussianDistribution(0, 0, m_Settings.m_BlurAmount);// ComputeGaussianBlur(0);
	sampleOffsets[0] = XMFLOAT2ZERO;

	// Maintain a sum of all the weighting values.
	float totalWeights = sampleWeights[0];

	// Add pairs of additional sample taps, positioned
	// along a line in both directions from the center.
	for (int i = 0; i < XE_BLOOM_BLUR_SAMPLE_COUNT / 2; i++)
	{
		// Store weights for the positive and negative taps.
		float weight = XE_Base::GaussianDistribution(i + 1.0f, 0, m_Settings.m_BlurAmount);// ComputeGaussianBlur(i + 1.0f);

		sampleWeights[i * 2 + 1] = weight;
		sampleWeights[i * 2 + 2] = weight;

		totalWeights += weight * 2;

		// To get the maximum amount of blurring from a limited number of
		// pixel shader samples, we take advantage of the bilinear filtering
		// hardware inside the texture fetch unit. If we position our texture
		// coordinates exactly halfway between two texels, the filtering unit
		// will average them for us, giving two samples for the price of one.
		// This allows us to step in units of two texels per sample, rather
		// than just one at a time. The 1.5 offset kicks things off by
		// positioning us nicely in between two texels.
		float sampleOffset = i * 2 + 1.5f;

		XMFLOAT2 delta = XEMathHelperVector::MultiplyVector2(XMFLOAT2(dx, dy), sampleOffset);

		// Store texture coordinate offsets for the positive and negative taps.
		sampleOffsets[i * 2 + 1] = delta;
		sampleOffsets[i * 2 + 2].x = -(delta.x);
		sampleOffsets[i * 2 + 2].y = -(delta.y);
	}

	// Normalize the list of sample weightings, so they will always sum to one.
	for (int i = 0; i < XE_BLOOM_BLUR_SAMPLE_COUNT; i++)
	{
		sampleWeights[i] /= totalWeights;
	}

	// Tell the effect about our new filter settings.
	m_BloomEffect->SetSampleWeights(sampleWeights);
	m_BloomEffect->SetSampleOffsets(sampleOffsets);
}

XEResult BloomPostProcess::RenderBloomCombine()
{
	uint32_t passes = 0;
	
	m_BloomEffect->ActivateTechnique(BloomPostProcessMaterial::XE_BLOOM_COMBINE);

	m_BloomEffect->SetSceneTexture(m_SceneRT);
	m_BloomEffect->SetPostBloomTexture(m_RT1);

	m_BloomEffect->SetBloomIntensity(m_Settings.m_BloomIntensity);
	m_BloomEffect->SetBaseIntensity(m_Settings.m_BaseIntensity);
	m_BloomEffect->SetBloomSaturation(m_Settings.m_BloomSaturation);
	m_BloomEffect->SetBaseSaturation(m_Settings.m_BaseSaturation);

	m_BloomEffect->Begin(passes);
	m_BloomEffect->BeginPass(0);

	m_GraphicDevice->DrawFullScreenQuad();

	m_BloomEffect->EndPass();
	m_BloomEffect->End();

	return XEResult::Ok;
}

void BloomPostProcess::OnLostDevice()
{
	m_BloomEffect->OnLostDevice();
	m_RT1->OnLostDevice();
	m_RT2->OnLostDevice();
	m_SceneRT->OnLostDevice();

	DrawableGameComponent::OnLostDevice();
}

void BloomPostProcess::OnResetDevice()
{
	m_BloomEffect->OnResetDevice();
	
	m_SceneRT->OnResetDevice();

	// Create two rendertargets for the bloom processing. These are half the
    // size of the backbuffer, in order to minimize fillrate costs. Reducing
    // the resolution in this way doesn't hurt quality, because we are going
    // to be blurring the bloom images in any case.
    uint32_t width = m_GraphicDevice->GetGraphicPP().BackBufferWidth / 2;
    uint32_t height = m_GraphicDevice->GetGraphicPP().BackBufferHeight / 2;

	m_RT1->OnResetDevice(width, height);
	m_RT2->OnResetDevice(width, height);

	DrawableGameComponent::OnResetDevice();
}
