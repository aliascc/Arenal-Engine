/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "FPRPreZ.h"
#include "FPRDefs.h"
#include "Time\XETime.h"
#include "FPRObjectDraw.h"
#include "GraphicDevice.h"
#include "FPRLightCulling.h"
#include "GameApp\GameApp.h"
#include "Lights\LightDefs.h"
#include "Lights\LightManager.h"
#include "ForwardPlusRendering.h"
#include "Shaders\Variables\Sampler.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Textures\DepthStencilSurface.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Variables\Texture2DArray.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ForwardPlusRendering::ForwardPlusRendering(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& serviceName, uint32_t callOrder)
	: GameComponent(gameApp, gameComponentName, callOrder)
	, m_ServiceName(serviceName)
{
	//Register Game Component as Service
	XETODO("Check ret better");
	XEResult ret = m_GameApp->RegisterGameService(m_ServiceName, this);
	XEAssert(ret == XEResult::Ok);

	//Create Forward+ Rendering Game Components
	m_FPRPreZ = new FPRPreZ(gameApp);
	m_FPRLightCulling = new FPRLightCulling(gameApp);
	m_FPRObjectDraw = new FPRObjectDraw(gameApp);

	XETODO("add check");
	m_GameApp->AddComponent(m_FPRPreZ);
	m_GameApp->AddComponent(m_FPRLightCulling);
	m_GameApp->AddComponent(m_FPRObjectDraw);
}

ForwardPlusRendering::~ForwardPlusRendering()
{
	m_GameApp->RemoveComponent(m_FPRPreZ);
	m_GameApp->RemoveComponent(m_FPRLightCulling);
	m_GameApp->RemoveComponent(m_FPRObjectDraw);

	DeleteMem(m_FPRPreZ);
	DeleteMem(m_FPRLightCulling);
	DeleteMem(m_FPRObjectDraw);

	XERelease(m_ForwardPlusDS);
	DeleteMem(m_LightStructuredBuffer);
	DeleteMem(m_ShadowSpotLightInfoStructuredBuffer);
	DeleteMem(m_ShadowDirLightInfoStructuredBuffer);
	DeleteMem(m_PerTileLightIndexBuffer);
	DeleteMem(m_ShadowTextureSampler);
}

void ForwardPlusRendering::Initialize()
{
	XEResult ret = XEResult::Ok;
	GraphicDevice* graphicDevice = m_GameApp->GetGraphicsDevice();

	/////////////////////////////////////////////////////
	//Create Depth Stencil
	m_ForwardPlusDS = new DepthStencilSurface(graphicDevice, XE_FORWARD_PLUS_DEPTH_STENCIL_NAME);

	/////////////////////////////////////////////////////
	//Create Shadow Texture Sampler
	m_ShadowTextureSampler = new Sampler(graphicDevice, XE_SAM_SHADOW_TEXTURE_SAMPLER_NAME, XE_BI_SHADOW_TEXTURE_SAMPLER_BIND_IDX);

	/////////////////////////////////////////////////////
	//Create Structured Buffer for Lights
	ret = XEBuiltInMaterialsHelpers::BuildBufferLightBuffer(graphicDevice, &m_LightStructuredBuffer);
	if(ret != XEResult::Ok)
	{
		XETODO("log error");
	}

	/////////////////////////////////////////////////////
	//Create Simple Buffer for Tile Light Index Buffer
	ret = XEBuiltInMaterialsHelpers::BuildBufferPerTileLightIndexBuffer(graphicDevice, &m_PerTileLightIndexBuffer);
	if(ret != XEResult::Ok)
	{
		XETODO("log error");
	}

	/////////////////////////////////////////////////////
	//Create Structured Buffer for Shadow Lights Info
	m_ShadowSpotLightInfoStructuredBuffer = new StructuredBuffer(XE_BF_SHADOW_SPOT_LIGHT_INFO_BUFFER_NAME, XE_BI_SHADOW_SPOT_LIGHT_INFO_BUFFER_BIND_IDX, false, graphicDevice);

	m_ShadowDirLightInfoStructuredBuffer = new StructuredBuffer(XE_BF_SHADOW_DIR_LIGHT_INFO_BUFFER_NAME, XE_BI_SHADOW_DIR_LIGHT_INFO_BUFFER_BIND_IDX, false, graphicDevice);

	GameComponent::Initialize();
}

void ForwardPlusRendering::LoadContent()
{
	XEResult ret = XEResult::Ok;
	GraphicDevice* graphicDevice = m_GameApp->GetGraphicsDevice();

	m_NumTiles = ForwardPlusHelper::GetNumTiles(graphicDevice->GetGraphicPP().m_BackBufferWidth, m_GameApp->GetGraphicsDevice()->GetGraphicPP().m_BackBufferHeight);

	XETODO("Set is ready when ds and buffers are created");

	/////////////////////////////////////////////////////
	//Init Depth Stencil
	XETODO("Check return");
	InitForwardPlusDS();

	/////////////////////////////////////////////////////
	//Init Per Light Light Index Buffer
	XETODO("Check return");
	InitPerTileLightIndexBuffer();

	/////////////////////////////////////////////////////
	//Init Light Structure Buffer
	ret = m_LightStructuredBuffer->InitializeBuffer(sizeof(LightFX), XE_MAX_LIGHTS, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
	if (ret != XEResult::Ok)
	{
		XETODO("log error");
	}

	/////////////////////////////////////////////////////
	//Init Shadow Texture Sampler
	//
	// MinFilter = ANISOTROPIC;
	// MagFilter = ANISOTROPIC;
	// MipFilter = ANISOTROPIC;
	// AddressU = BORDER;
	// AddressV = BORDER;
	// MaxAnisotropy = 8;
	// BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//
	ret = m_ShadowTextureSampler->Initialize(ShaderFilter::Anisotropic, ShaderTextureAddressMode::Border, ShaderTextureAddressMode::Border, ShaderTextureAddressMode::Clamp, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0.0f, 8, ShaderCompFunct::Never, XEColors::White);
	if (ret != XEResult::Ok)
	{
		XETODO("log error");
	}

	GameComponent::LoadContent();
}

void ForwardPlusRendering::Update(const TimerParams& timerParams)
{
	XEResult ret = XEResult::Ok;

	////////////////////////////////////////////////
	//Get Light Manager
	LightManager* lightManager = m_GameApp->GetLightManager();

	////////////////////////////////////////////////
	//Update Light FX Vector if needed
	if(lightManager->HasFXVectorChanged())
	{
		const LightFX* lighFXVector = lightManager->GetLightFXVector();

		XETODO("Check return");
		m_LightStructuredBuffer->UpdateBuffer(lighFXVector, XE_MAX_LIGHTS, 0, 0, GraphicResourceMap::WriteDiscard);

		lightManager->ClearFXVectorChangeFlag();
	}

	////////////////////////////////////////////////
	//Update Shadow Spot Light Info FX Vector if needed
	if (lightManager->HasFXSpotShadowVectorChanged())
	{
		uint32_t numSpotInfoShadows = lightManager->GetNumSpotLightsWithShadows();

		if (m_ShadowSpotLightInfoStructuredBuffer->GetNumElements() != numSpotInfoShadows)
		{
			if (numSpotInfoShadows != 0)
			{
				/////////////////////////////////////////////////////
				//Init Shadow Light Info Structure Buffer with new size
				ret = m_ShadowSpotLightInfoStructuredBuffer->InitializeBuffer(sizeof(SpotLightShadowInfoFX), numSpotInfoShadows, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
				if (ret != XEResult::Ok)
				{
					XETODO("log error");
				}
			}
			else
			{
				/////////////////////////////////////////////////////
				//De-initialize Array
				ret = m_ShadowSpotLightInfoStructuredBuffer->Deinitialize();
				if (ret != XEResult::Ok)
				{
					XETODO("log error");
				}
			}
		}

		if (numSpotInfoShadows != 0)
		{
			const SpotLightShadowInfoFX* shadowLightInfoFXVec = lightManager->GetSpotLightShadowInfoFXVector();

			XETODO("Check return");
			m_ShadowSpotLightInfoStructuredBuffer->UpdateBuffer(shadowLightInfoFXVec, numSpotInfoShadows, 0, 0, GraphicResourceMap::WriteDiscard);
		}

		lightManager->ClearFXSpotShadowVectorChangedFlag();
	}

	////////////////////////////////////////////////
	//Update Shadow Directional Light Info FX Vector if needed
	if (lightManager->HasFXDirShadowVectorChanged())
	{
		uint32_t numDirInfoShadows = lightManager->GetNumDirLightsWithShadows();

		if (m_ShadowDirLightInfoStructuredBuffer->GetNumElements() != numDirInfoShadows)
		{
			if (numDirInfoShadows != 0)
			{
				/////////////////////////////////////////////////////
				//Init Shadow Light Info Structure Buffer with new size
				ret = m_ShadowDirLightInfoStructuredBuffer->InitializeBuffer(sizeof(LightCascadeInfoFX), numDirInfoShadows, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
				if (ret != XEResult::Ok)
				{
					XETODO("log error");
				}
			}
			else
			{
				/////////////////////////////////////////////////////
				//De-initialize Array
				ret = m_ShadowDirLightInfoStructuredBuffer->Deinitialize();
				if (ret != XEResult::Ok)
				{
					XETODO("log error");
				}
			}
		}

		if (numDirInfoShadows != 0)
		{
			const LightCascadeInfoFX* shadowLightInfoFXVec = lightManager->GetDirLightShadowInfoFXVector();

			XETODO("Check return");
			m_ShadowDirLightInfoStructuredBuffer->UpdateBuffer(shadowLightInfoFXVec, numDirInfoShadows, 0, 0, GraphicResourceMap::WriteDiscard);
		}

		lightManager->ClearFXDirShadowVectorChangedFlag();
	}

	////////////////////////////////////////////////
	//Finish
	GameComponent::Update(timerParams);
}

void ForwardPlusRendering::OnResetDevice()
{
	GraphicDevice* graphicDevice = m_GameApp->GetGraphicsDevice();

	m_NumTiles = ForwardPlusHelper::GetNumTiles(graphicDevice->GetGraphicPP().m_BackBufferWidth, m_GameApp->GetGraphicsDevice()->GetGraphicPP().m_BackBufferHeight);

	XETODO("Check returns");

	InitForwardPlusDS();

	InitPerTileLightIndexBuffer();

	GameComponent::OnResetDevice();
}

XEResult ForwardPlusRendering::InitForwardPlusDS()
{
	XEAssert(m_ForwardPlusDS != nullptr);
	if(m_ForwardPlusDS == nullptr)
	{
		return XEResult::NullObj;
	}

	GraphicDevice* graphicDevice = m_GameApp->GetGraphicsDevice();

	XEResult ret = XEResult::Ok;

	uint32_t screenWidth = graphicDevice->GetGraphicPP().m_BackBufferWidth;
	uint32_t screenHeight = graphicDevice->GetGraphicPP().m_BackBufferHeight;

	ret = m_ForwardPlusDS->InitializeDepthStencilSurface(screenWidth, screenHeight, DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
	if(ret != XEResult::Ok)
	{
		XETODO("Add log");
		return ret;
	}

	return XEResult::Ok;
}

XEResult ForwardPlusRendering::InitPerTileLightIndexBuffer()
{
	XEAssert(m_PerTileLightIndexBuffer != nullptr);
	if(m_PerTileLightIndexBuffer == nullptr)
	{
		return XEResult::NullObj;
	}

	XEResult ret = XEResult::Ok;

	//Use +1 for MAX_LIGHTS, that way we can add a sentry at the end of the Tile Light Buffer telling us
	//it has ended
	uint32_t sizeOfTileLightBuffer = (XE_MAX_LIGHTS + 1) * (m_NumTiles.x * m_NumTiles.y);

	ret = m_PerTileLightIndexBuffer->InitializeBuffer(sizeOfTileLightBuffer, GraphicBufferUsage::Default, GraphicBufferAccess::None);
	if(ret != XEResult::Ok)
	{
		XETODO("log error");
		return ret;
	}

	return XEResult::Ok;
}