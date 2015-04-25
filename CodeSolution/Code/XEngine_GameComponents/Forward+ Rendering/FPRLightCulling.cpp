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
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Time\XETime.h"
#include "Camera\Camera.h"
#include "GraphicDevice.h"
#include "FPRLightCulling.h"
#include "GameApp\GameApp.h"
#include "Textures\Texture.h"
#include "Lights\LightManager.h"
#include "ForwardPlusRendering.h"
#include "Camera\CameraUpdater.h"
#include "Textures\DepthStencilSurface.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"
#include "Graphic Extensions\Materials\Forward+ Rendering\ForwardPlusLightCullingMaterial.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check object instances and calls to where it is init");
FPRLightCulling::FPRLightCulling(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& fprServiceName, const std::wstring& cameraServiceName, uint32_t callOrder)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
{
	m_ForwardPlusRendering = m_GameApp->GetGameService<ForwardPlusRendering>(fprServiceName);
	XEAssert(m_ForwardPlusRendering != nullptr);

	m_CameraUpdater = m_GameApp->GetGameService<CameraUpdater>(cameraServiceName);
	XEAssert(m_CameraUpdater != nullptr);
}

FPRLightCulling::~FPRLightCulling()
{
	DeleteMem(m_ForwardPlusLightCullingMaterial);
}

void FPRLightCulling::Initialize()
{
	m_ForwardPlusLightCullingMaterial = new ForwardPlusLightCullingMaterial(m_GraphicDevice, m_GameResourceManager);

	DrawableGameComponent::Initialize();
}

void FPRLightCulling::LoadContent()
{
	XEResult ret = XEResult::Ok;

	ret = m_ForwardPlusLightCullingMaterial->LoadContent();

	if(ret != XEResult::Ok)
	{
		XETODO("Add log");
	}

	DrawableGameComponent::LoadContent();
}

void FPRLightCulling::Update(const TimerParams& timerParams)
{
	DrawableGameComponent::Update(timerParams);
}

void FPRLightCulling::Render(const TimerParams& timerParams)
{
	///////////////////////////////////////////
	//Begin Event for Diagnostic View
	m_GraphicDevice->BeginEvent(L"Forward+ Rendering Light Culling");

	///////////////////////////////////////////////////
	//Get Current Main Camera
	Camera* currentCamera = m_CameraUpdater->GetMainCamera();
	if (currentCamera == nullptr)
	{
		XETODO("Add log");
		return;
	}

	///////////////////////////////////////////
	//Check that the Depth Stencil has been
	//applied and that is its the correct one
	const ShaderTextureBinding* stb = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetShaderTextureBinding(XE_TX_DEPTH_TEXTURE_CULL_NAME);
	Texture* texture = stb->GetTexture();
	DepthStencilSurface* dsFPR = m_ForwardPlusRendering->GetForwardPlusDS();

	if( !stb->HasTexture() || ( stb->HasTexture() && texture->GetUniqueID() != dsFPR->GetUniqueID() ) )
	{
		XETODO("Check Return");
		m_ForwardPlusLightCullingMaterial->GetCSProps()->SetTexture(XE_TX_DEPTH_TEXTURE_CULL_NAME, dsFPR);
	}

	///////////////////////////////////////////
	//Override Structure Buffer to use a single 
	//one
	StructuredBuffer* currentStructuredBuffer = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetStructuredBuffer(XE_BF_LIGHT_BUFFER_NAME);
	StructuredBuffer* structuredBuffer = m_ForwardPlusRendering->GetLightStructuredBuffer();

	if( currentStructuredBuffer == nullptr || (currentStructuredBuffer != nullptr && currentStructuredBuffer->GetUniqueID() != structuredBuffer->GetUniqueID() ) )
	{
		XETODO("Check Return");
		m_ForwardPlusLightCullingMaterial->GetCSProps()->OverrideStructuredBuffer(XE_BF_LIGHT_BUFFER_NAME, structuredBuffer);
	}

	///////////////////////////////////////////
	//Override Simple Buffer to use a single 
	//one
	SimpleBuffer* currentSimpleBuffer = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetSimpleBuffer(XE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME);
	SimpleBuffer* simpleBuffer = m_ForwardPlusRendering->GetPerTileLightIndexBuffer();

	if( currentSimpleBuffer == nullptr || (currentSimpleBuffer != nullptr && currentSimpleBuffer->GetUniqueID() != simpleBuffer->GetUniqueID() ) )
	{
		XETODO("Check Return");
		m_ForwardPlusLightCullingMaterial->GetCSProps()->OverrideSimpleBuffer(XE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME, simpleBuffer);
	}

	///////////////////////////////////////////
	//Set Constant Buffer Variables
	ConstantBuffer* cb = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetConstantBuffer(XE_CB_FPR_LIGHT_CULLING_NAME);
	XEAssert(cb != nullptr);
	if(cb != nullptr)
	{
		glm::mat4 invProj = glm::inverse(currentCamera->GetProjectionMatrix());
		uint32_t numLights = m_GameApp->GetLightManager()->GetNumberOfLights();

		cb->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, currentCamera->GetViewMatrix());
		cb->SetValueT<glm::mat4>(XE_CB_INV_PROJECTION_VAR_NAME, invProj);

		cb->SetValueT<uint32_t>(XE_CB_WINDOW_HEIGHT_VAR_NAME, m_GraphicDevice->GetGraphicPP().m_BackBufferHeight);
		cb->SetValueT<uint32_t>(XE_CB_WINDOW_WIDTH_VAR_NAME, m_GraphicDevice->GetGraphicPP().m_BackBufferWidth);
		cb->SetValueT<uint32_t>(XE_CB_NUM_LIGHTS_VAR_NAME, numLights);
	}

	///////////////////////////////////////////
	//Apply Shader Settings
	XETODO("Check Return");
	m_ForwardPlusLightCullingMaterial->Apply();

	///////////////////////////////////////////
	//Dispatch CS
	const glm::uvec2& numTiles = m_ForwardPlusRendering->GetNumTiles();

	XETODO("Check Return");
	m_GraphicDevice->DispatchComputeShader(numTiles.x, numTiles.y, 1);

	///////////////////////////////////////////
	//Un-apply Shader Settings
	XETODO("Check Return");
	m_ForwardPlusLightCullingMaterial->UnApply();

	///////////////////////////////////////////
	//End Event for Diagnostic View
	m_GraphicDevice->EndEvent();

	///////////////////////////////////////////
	//End
	DrawableGameComponent::Render(timerParams);
}

void FPRLightCulling::OnLostDevice()
{	
	DrawableGameComponent::OnLostDevice();
}

void FPRLightCulling::OnResetDevice()
{
	DrawableGameComponent::OnResetDevice();
}