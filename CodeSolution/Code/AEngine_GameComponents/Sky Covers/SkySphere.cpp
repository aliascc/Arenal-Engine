/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include "SkySphere.h"
#include "GameApp\GameApp.h"
#include "Materials\SkySphereMaterial.h"
#include "Shapes\SphereShape.h"
#include "Models\MeshPart.h"
#include "GraphicDevice.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\IVertexBuffer.h"
#include "Camera\Camera.h"
#include "Textures\TextureCube.h"
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"
#include "Localization\LocalizationManager.h"
#include "Logger\Logger.h"
#include "Math\AEMathDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SkySphere::SkySphere(GameApp* gameApp, const std::string& skyTextureFile, bool deferred, uint32_t callOrder, const XMFLOAT3& skySphereSize, const std::string& cameraSeviceName, const std::string& gameComponentName)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
	, m_SkyEffect(nullptr)
	, m_SphereShape(nullptr)
	, m_WVPMatrix(XMFLOAT4X4IDENTITY)
	, m_Size(skySphereSize)
	, m_SkyTexture(nullptr)
	, m_SkyTextureFile(skyTextureFile)
	, m_CameraServiceName(cameraSeviceName)
	, m_Deferred(deferred)
{
}

SkySphere::~SkySphere()
{
	DeleteMem(m_SkyEffect);
	DeleteMem(m_SphereShape);
	DeleteMem(m_SkyTexture);
}

void SkySphere::Initialize()
{
	m_SphereShape = new SphereShape(m_GraphicDevice, 1);

	m_SkyEffect = new SkySphereMaterial();

	//Get Camera Service
	m_Camera = static_cast<Camera*>(m_GameApp->GetGameService(m_CameraServiceName));
	assert(m_Camera);

	//Create Texture Cube Instance
	m_SkyTexture = TextureCube::CreateTextureCubeInst();

	DrawableGameComponent::Initialize();
}

void SkySphere::LoadContent()
{	
	AEResult ret = m_SkyTexture->CreateFromFile(m_GameApp->GetGraphicsDevice(), "Sky Texture Cube", m_SkyTextureFile);

	if(ret != AEResult::Ok)
	{
		std::string errMsg = fmt::format(LOCLANG->GetLiteral("COULD_NOT_LOAD_TEXTURE"), __FUNCTION__, m_SkyTextureFile);
		LOGGER->AddNewLog(LogLevel::Error, errMsg);
	}
	
	m_SkyEffect->LoadMaterial(m_GameApp->GetGraphicsDevice());

	//Set Technique According to Deferred Option
	if(m_Deferred)
	{
		m_SkyEffect->ActivateTechnique(SkySphereMaterial::AE_SKY_SPHERE_DEFERRED_EFFECT);
	}
	else
	{
		m_SkyEffect->ActivateTechnique(SkySphereMaterial::AE_SKY_SPHERE_EFFECT);
	}

	//Initialize Constant EFfect Parameters
	InitEffectParams();

	DrawableGameComponent::LoadContent();
}

void SkySphere::InitEffectParams()
{
	m_SkyEffect->SetSkyTexture(m_SkyTexture);
}

void SkySphere::Update(const TimerParams& timerParams)
{
	XMFLOAT4X4 scale = XEMathHelperMatrix::CreateScale(m_Size);
	XMFLOAT4X4 pos = XEMathHelperMatrix::CreateTranslation(m_Camera->GetCameraPosition());
	XMFLOAT4X4 world(XMFLOAT4X4IDENTITY);

	XEMathHelperMatrix::Multiply2(scale, pos, world);

	XEMathHelperMatrix::Multiply3(world, m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_WVPMatrix);

	DrawableGameComponent::Update(timerParams);
}

void SkySphere::Render(const TimerParams& timerParams)
{
	AE_Base::BeginUserEventPerf(L"Sky Sphere");

	uint32_t passes = 0;

	m_SkyEffect->SetWorldViewProjection(m_WVPMatrix);

	m_SkyEffect->Begin(passes);
	m_SkyEffect->BeginPass(0);

	m_SphereShape->DrawSphere();

	m_SkyEffect->EndPass();
	m_SkyEffect->End();

	AE_Base::EndUserEventPerf();

	DrawableGameComponent::Render(timerParams);
}

void SkySphere::OnLostDevice()
{
	m_SkyEffect->OnLostDevice();
	m_SphereShape->OnLostDevice();
	
	DrawableGameComponent::OnLostDevice();
}

void SkySphere::OnResetDevice()
{
	m_SkyEffect->OnResetDevice();
	m_SphereShape->OnResetDevice();

	DrawableGameComponent::OnResetDevice();
}