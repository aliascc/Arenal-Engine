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
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredRenderingEnd.h"
#include "Textures\RenderTarget.h"
#include "Utils\XETimeDefs.h"
#include "GameApp\GameApp.h"
#include "GraphicDevice.h"
#include "DeferredRendering.h"
#include "Materials\DeferredRenderingMaterial.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DeferredRenderingEnd::DeferredRenderingEnd(GameApp* gameApp, const DeferredRenderingEndSettings& settings, uint32_t callOrder, const std::string& serviceName, const std::string& deferredServiceName, const std::string& gameComponentName)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
	, m_DeferredServiceName(deferredServiceName)
	, m_FinalRT(nullptr)
	, m_LightRT(nullptr)
	, m_DeferredRendering(nullptr)
	, m_Settings(settings)
	, m_DREffect(nullptr)
	, m_ServiceName(serviceName)
{	
	//Register this Game Component as a service so others can use it
	m_GameApp->RegisterGameService(m_ServiceName, this);
}

DeferredRenderingEnd::~DeferredRenderingEnd()
{
	DeleteMem(m_FinalRT);
	DeleteMem(m_LightRT);
	DeleteMem(m_DREffect);
}

void DeferredRenderingEnd::Initialize()
{
	//Get Deferred Rendering Service associated with this end
	m_DeferredRendering = static_cast<DeferredRendering*>(m_GameApp->GetGameService(m_DeferredServiceName));

	uint32_t width = m_GraphicDevice->GetGraphicPP().BackBufferWidth;
	uint32_t height = m_GraphicDevice->GetGraphicPP().BackBufferHeight;
	
	//Create Render Targets
	m_LightRT = RenderTarget::CreateRenderTargetInst();
	
	//Initialize Render Targets
	m_LightRT->CreateRenderTargetTexture(m_GraphicDevice, "Light Deferred Rendering", width, height, XE_POOL_DEFAULT, m_Settings.m_LightPixFmt);

	if(!m_Settings.m_UseDefaultRT)
	{
		//Create Render Targets
		m_FinalRT = RenderTarget::CreateRenderTargetInst();

		//Initialize Render Targets
		XETODO("Set Final RT Sample Type and Quality");
		m_FinalRT->CreateRenderTargetPure(m_GraphicDevice, "Final Deferred Rendering", width, height, m_Settings.m_FinalPixFmt, XE_MS_8_SAMPLES, 0);
	}

	m_DREffect = new DeferredRenderingMaterial();
	
	DrawableGameComponent::Initialize();
}

void DeferredRenderingEnd::LoadContent()
{
	//Load Effect
	m_DREffect->LoadMaterial(m_GraphicDevice);
	InitEffectParams();

	DrawableGameComponent::LoadContent();
}

void DeferredRenderingEnd::Update(const TimerParams& timerParams)
{
	DrawableGameComponent::Update(timerParams);
}

XEResult DeferredRenderingEnd::SetFrameEffectParams()
{
	if(m_DREffect->SetDepthTexture(m_DeferredRendering->GetDepthRT()) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

void DeferredRenderingEnd::Render(const TimerParams& timerParams)
{
	XE_Base::BeginUserEventPerf(L"Deferred Rendering End");

	//Set Render Targets Back to Default
	m_GraphicDevice->SetRenderTarget(1, nullptr);
	m_GraphicDevice->SetRenderTarget(2, nullptr);

	//Set Per Frame Parameters
	SetFrameEffectParams();
	
	m_GraphicDevice->ColorFill(m_LightRT, XMCOLORTransparent);
	m_GraphicDevice->SetRenderTarget(0, m_LightRT);

	//Draw Sky Cover
	DrawSkyCover();

	//Draw Lights
	DrawLights();

	//If use Default set RT to Default Status if not use Final RT
	if(m_Settings.m_UseDefaultRT)
	{
		m_GraphicDevice->SetRenderTarget(0, nullptr);
	}
	else
	{
		m_GraphicDevice->ColorFill(m_FinalRT, XMCOLORCornflowerBlue);
		m_GraphicDevice->SetRenderTarget(0, m_FinalRT);
	}

	//Draw Combine
	DrawCombine();

	//If not using Default set it back to Default status
	if(!m_Settings.m_UseDefaultRT)
	{
		m_GraphicDevice->SetRenderTarget(0, nullptr);
	}

	XE_Base::EndUserEventPerf();

	DrawableGameComponent::Render(timerParams);
}

XEResult DeferredRenderingEnd::DrawSkyCover()
{
	XE_Base::BeginUserEventPerf(L"Deferred Rendering End (Sky Cover)");

	if(m_DREffect->ActivateTechnique(DeferredRenderingMaterial::XE_DEFREND_DFSKY_TECHNIQUE) != XEResult::Ok)
	{
		XE_Base::EndUserEventPerf();
		return XEResult::Fail;
	}

	m_DREffect->Begin();
	m_DREffect->BeginPass(0);
	
	m_GraphicDevice->DrawFullScreenQuad();

	m_DREffect->EndPass();
	m_DREffect->End();

	XE_Base::EndUserEventPerf();
	
	return XEResult::Ok;
}

XEResult DeferredRenderingEnd::DrawLights()
{
	XE_Base::BeginUserEventPerf(L"Deferred Rendering End (Lights)");

	

	XE_Base::EndUserEventPerf();

	return XEResult::Fail;
}

XEResult DeferredRenderingEnd::DrawCombine()
{
	XE_Base::BeginUserEventPerf(L"Deferred Rendering End (Combine)");

	if(m_DREffect->ActivateTechnique(DeferredRenderingMaterial::XE_DEFREND_DFCOMBINE) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(m_DREffect->SetLightTexture(m_LightRT) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(m_DREffect->SetDiffuseTexture(m_DeferredRendering->GetDiffuseRT()) != XEResult::Ok)
	{
		return XEResult::Fail;
	}
		
	m_DREffect->Begin();
	m_DREffect->BeginPass(0);
	
	m_GraphicDevice->DrawFullScreenQuad();
	
	m_DREffect->EndPass();
	m_DREffect->End();

	XE_Base::EndUserEventPerf();

	return XEResult::Fail;
}

XEResult DeferredRenderingEnd::InitEffectParams()
{
	if(m_DREffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel()) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

void DeferredRenderingEnd::OnLostDevice()
{	
	if(!m_Settings.m_UseDefaultRT)
	{
		m_FinalRT->OnLostDevice();
	}

	m_LightRT->OnLostDevice();

	m_DREffect->OnLostDevice();

	DrawableGameComponent::OnLostDevice();
}

void DeferredRenderingEnd::OnResetDevice()
{
	if(!m_Settings.m_UseDefaultRT)
	{
		m_FinalRT->OnResetDevice();
	}

	m_LightRT->OnResetDevice();

	m_DREffect->OnResetDevice();
	m_DREffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel());

	DrawableGameComponent::OnResetDevice();
}