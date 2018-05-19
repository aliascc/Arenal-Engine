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
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredRenderingEnd.h"
#include "Textures\RenderTarget.h"
#include "Utils\AETimeDefs.h"
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
	m_LightRT->CreateRenderTargetTexture(m_GraphicDevice, "Light Deferred Rendering", width, height, AE_POOL_DEFAULT, m_Settings.m_LightPixFmt);

	if(!m_Settings.m_UseDefaultRT)
	{
		//Create Render Targets
		m_FinalRT = RenderTarget::CreateRenderTargetInst();

		//Initialize Render Targets
		AETODO("Set Final RT Sample Type and Quality");
		m_FinalRT->CreateRenderTargetPure(m_GraphicDevice, "Final Deferred Rendering", width, height, m_Settings.m_FinalPixFmt, AE_MS_8_SAMPLES, 0);
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

AEResult DeferredRenderingEnd::SetFrameEffectParams()
{
	if(m_DREffect->SetDepthTexture(m_DeferredRendering->GetDepthRT()) != AEResult::Ok)
	{
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

void DeferredRenderingEnd::Render(const TimerParams& timerParams)
{
	AE_Base::BeginUserEventPerf("Deferred Rendering End");

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

	AE_Base::EndUserEventPerf();

	DrawableGameComponent::Render(timerParams);
}

AEResult DeferredRenderingEnd::DrawSkyCover()
{
	AE_Base::BeginUserEventPerf("Deferred Rendering End (Sky Cover)");

	if(m_DREffect->ActivateTechnique(DeferredRenderingMaterial::AE_DEFREND_DFSKY_TECHNIQUE) != AEResult::Ok)
	{
		AE_Base::EndUserEventPerf();
		return AEResult::Fail;
	}

	m_DREffect->Begin();
	m_DREffect->BeginPass(0);
	
	m_GraphicDevice->DrawFullScreenQuad();

	m_DREffect->EndPass();
	m_DREffect->End();

	AE_Base::EndUserEventPerf();
	
	return AEResult::Ok;
}

AEResult DeferredRenderingEnd::DrawLights()
{
	AE_Base::BeginUserEventPerf("Deferred Rendering End (Lights)");

	

	AE_Base::EndUserEventPerf();

	return AEResult::Fail;
}

AEResult DeferredRenderingEnd::DrawCombine()
{
	AE_Base::BeginUserEventPerf("Deferred Rendering End (Combine)");

	if(m_DREffect->ActivateTechnique(DeferredRenderingMaterial::AE_DEFREND_DFCOMBINE) != AEResult::Ok)
	{
		return AEResult::Fail;
	}

	if(m_DREffect->SetLightTexture(m_LightRT) != AEResult::Ok)
	{
		return AEResult::Fail;
	}

	if(m_DREffect->SetDiffuseTexture(m_DeferredRendering->GetDiffuseRT()) != AEResult::Ok)
	{
		return AEResult::Fail;
	}
		
	m_DREffect->Begin();
	m_DREffect->BeginPass(0);
	
	m_GraphicDevice->DrawFullScreenQuad();
	
	m_DREffect->EndPass();
	m_DREffect->End();

	AE_Base::EndUserEventPerf();

	return AEResult::Fail;
}

AEResult DeferredRenderingEnd::InitEffectParams()
{
	if(m_DREffect->SetHalfPixel(m_GraphicDevice->GetHalfPixel()) != AEResult::Ok)
	{
		return AEResult::Fail;
	}

	return AEResult::Ok;
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