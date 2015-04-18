/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/13/2012
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _FORWARD_PLUS_RENDERING_H
#define _FORWARD_PLUS_RENDERING_H

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
#include "Math\XEMathDefs.h"
#include "XEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class FPRPreZ;
class Sampler;
class GameApp;
class SimpleBuffer;
struct TimerParams;
class FPRObjectDraw;
class FPRLightCulling;
class StructuredBuffer;
class DepthStencilSurface;

/*****************
*   Class Decl   * 
******************/
class ForwardPlusRendering sealed : public GameComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		glm::uvec2 m_NumTiles = XEMathHelpers::Vec2uZero;

		std::wstring m_ServiceName = L"";

		FPRPreZ* m_FPRPreZ = nullptr;

		FPRLightCulling* m_FPRLightCulling = nullptr;

		FPRObjectDraw* m_FPRObjectDraw = nullptr;

		DepthStencilSurface* m_ForwardPlusDS = nullptr;

		StructuredBuffer* m_LightStructuredBuffer = nullptr;

		StructuredBuffer* m_ShadowSpotLightInfoStructuredBuffer = nullptr;

		StructuredBuffer* m_ShadowDirLightInfoStructuredBuffer = nullptr;

		SimpleBuffer* m_PerTileLightIndexBuffer = nullptr;

		Sampler* m_ShadowTextureSampler = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		XEResult InitForwardPlusDS();

		XEResult InitPerTileLightIndexBuffer();

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// ForwardPlusRendering Constructor
		/// </summary>
		/// <param name="GameApp">Graphic App to be associated with this Game Component</param>
		/// <param name="gameComponentName">Game Component Name to be set</param>
		/// <param name="serviceName">Service Name to be Set for this Game Component</param>
		/// <param name="callOder">Call order of Game Component</param>
		ForwardPlusRendering(GameApp* gameApp, const std::wstring& gameComponentName = XE_FORWARD_PLUS_MAIN_DEF_COMPONENT_NAME, const std::wstring& serviceName = XE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_ForwardPlusMain);

		/// <summary>
		/// Default ForwardPlusRendering Destructor
		/// </summary>
		virtual ~ForwardPlusRendering();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline Sampler* GetShadowTextureSampler() const
		{
			return m_ShadowTextureSampler;
		}

		inline DepthStencilSurface* GetForwardPlusDS() const
		{
			return m_ForwardPlusDS;
		}

		inline StructuredBuffer* GetLightStructuredBuffer() const
		{
			return m_LightStructuredBuffer;
		}

		inline StructuredBuffer* GetShadowSpotLightInfoStructuredBuffer() const
		{
			return m_ShadowSpotLightInfoStructuredBuffer;
		}

		inline StructuredBuffer* GetShadowDirLightInfoStructuredBuffer() const
		{
				return m_ShadowDirLightInfoStructuredBuffer;
		}

		inline SimpleBuffer* GetPerTileLightIndexBuffer() const
		{
			return m_PerTileLightIndexBuffer;
		}

		inline const glm::uvec2& GetNumTiles() const
		{
			return m_NumTiles;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		void Initialize() override;

		void LoadContent() override;

		void Update(const TimerParams& timerParams) override;

		void OnResetDevice() override;

#pragma endregion

};

#endif