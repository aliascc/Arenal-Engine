
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/14/2012
*
* Last Major Update: 6/26/2012
* 
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _GRAPHIC_DEVICE_H
#define _GRAPHIC_DEVICE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <Windows.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "GraphicsDefs.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"
#include "Shaders\ShaderDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Shader;
class Sampler;
class Texture;
class Viewport;
class Texture2D;
class HullShader;
class IndexBuffer;
class QuadShape2D;
class PixelShader;
class TextureArray;
class ShaderBuffer;
class RenderTarget;
class VertexShader;
class DomainShader;
class IVertexBuffer;
class GraphicDevice;
class ComputeShader;
class GeometryShader;
class ConstantBuffer;
class Texture2DArray;
class DepthStencilSurface;
class SamplerStateCollection;

/*****************
*   Class Decl   *
******************/
class GraphicDevice sealed : public XEObject
{
private:

		/*************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

		/// <summary>
		/// DirectX 11 Device Pointer
		/// </summary>
		ID3D11Device* m_DeviceDX = nullptr;

		/// <summary>
		/// DirectX 11 Device Context
		/// </summary>
		ID3D11DeviceContext* m_DeviceContextDX = nullptr;

		/// <summary>
		/// DirectX 11 Swap Chain
		/// </summary>
		IDXGISwapChain* m_SwapChainDX = nullptr;

		/// <summary>
		/// DirectX 11 Default Depth Stencil Buffer
		/// </summary>
		ID3D11Texture2D* m_DefaultDepthStencilBufferDX = nullptr;

		/// <summary>
		/// DirectX 11 Default Render Target
		/// </summary>
		ID3D11RenderTargetView* m_DefaultRenderTargetViewDX = nullptr;

		/// <summary>
		/// DirectX 11 Default Depth Stencil View
		/// </summary>
		ID3D11DepthStencilView* m_DefaultDepthStencilViewDX = nullptr;

		/// <summary>
		/// Max Render Targets that can be set
		/// </summary>
		uint32_t m_MaxNumRenderTargets = 0;

		/// <summary>
		/// The Highest Stage ID of a render target present. 
		/// Use to determine how many views we will set when we call the set Render Targets and DS Function
		/// </summary>
		uint32_t m_HighestRenderTargetStage = 0;

		/// <summary>
		/// DirectX 11 Current Render Targets
		/// </summary>
		ID3D11RenderTargetView** m_CurrentRenderTargetViewsDX = nullptr;

		/// <summary>
		/// DirectX 11 Current Depth Stencil View
		/// </summary>
		ID3D11DepthStencilView* m_CurrentDepthStencilViewDX = nullptr;

		/// <summary>
		/// DirectX 11 Default Screen View port
		/// </summary>
		D3D11_VIEWPORT m_ScreenViewportDX;

		/// <summary>
		/// DirectX Swap Chain Description
		/// </summary>
		DXGI_SWAP_CHAIN_DESC m_SwapChainDescDX;

		Texture2D* m_DefaultTexture2D = nullptr;

		bool m_IsReady = false;

		glm::vec2 m_HalfPixel = XEMathHelpers::Vec2fZero;

		QuadShape2D* m_QuadShape2D = nullptr;

		SamplerStateCollection* m_SamplerStateCol = nullptr;

		GraphicsPresentationParameters m_gPP;

#if defined(_DEBUG) || defined(PROFILE)

		ID3DUserDefinedAnnotation*	m_UserDefinedAnnotationDX = nullptr;

#endif

#pragma endregion

		/***********************
		 *   Private Methods   *
		 ***********************/
#pragma region Private Methods

		XEResult CreateDefaultTextures();

		XEResult ReleaseDefaultTextures();

		XEResult InitDXConfiguration();

		XEResult InitSwapChainDesc();

		XEResult CreateSwapChain();

		XEResult InitViewport();

		XEResult CreateDefaultRTDS();

		XEResult CreateDepthStencil();

		XEResult CreateRenderTarget();

		void ReleaseDefaultRTDS();

		XEResult InitPreGraphicObjects();

		XEResult InitGraphicObjects();

		void InitBasicVertexTypes();

		void CleanUpBasicVertexTypes();

		void ResetHalfPixel();

		XEResult OnLostDevice();

		XEResult OnResetDevice();

		XEResult SetShaderResourceView(ShaderType type, uint32_t index, ID3D11ShaderResourceView* srv);

		XEResult SetShaderResourceViews(ShaderType type, uint32_t index, uint32_t numView, ID3D11ShaderResourceView** srv);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GraphicDevice Constructor
		/// </summary>
		/// <param name='hMainWnd'>Main Window of the Game Application</param>
		/// <param name='graphicOpts'>Graphic Options to use when initializing the device</param>
		GraphicDevice(HWND hMainWnd, const GraphicOptsPreferred& graphicOpts);

		/// <summary>
		/// Default GraphicDevice Destructor
		/// </summary>
		virtual ~GraphicDevice();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline GraphicsPresentationParameters& GetGraphicPP()
		{
			return m_gPP;
		}

		inline const glm::vec2& GetHalfPixel() const
		{ 
			return m_HalfPixel; 
		}

		inline ID3D11Device* GetDeviceDX() const
		{ 
			return m_DeviceDX; 
		}

		inline ID3D11DeviceContext* GetDeviceContextDX() const
		{ 
			return m_DeviceContextDX; 
		}

		inline Texture2D* GetDefaultTexture2D() const
		{
			return m_DefaultTexture2D;
		}

#pragma endregion

		XEResult						ResetDevice				();
		XEResult						Resize					(uint32_t width, uint32_t height);
		XEResult						InitDevice				();

		XEResult						Clear(bool target = true, uint32_t rt_stage_id = 0, bool depth = true, bool stencil = true, const Color& color = XEColors::WorldEngineBackground, float depthVal = 1.0f, uint8_t stencilVal = 0);
		XEResult						Present					();
		XEResult						CheckDevCaps			(const GraphicsCheckDevCaps& devCaps);

		XEResult						SetPixelShader			(PixelShader* ps);
		XEResult						SetVertexShader			(VertexShader* vs);
		XEResult						SetGeometryShader		(GeometryShader* gs);
		XEResult						SetHullShader			(HullShader* hs);
		XEResult						SetDomainShader			(DomainShader* ds);
		XEResult						SetComputeShader		(ComputeShader* cs);
		XEResult						SetShader				(Shader* cs);
		XEResult						SetSamplerState			(ShaderType type, uint32_t index, ID3D11SamplerState* sampler);
		XEResult						SetSamplerStates		(ShaderType type, uint32_t index, uint32_t numStates, ID3D11SamplerState** sampler);

		XEResult						SetConstantBuffer		(ShaderType type, ConstantBuffer* cb, bool overrideBindIndex = false, uint32_t newIndex = 0);

		XEResult SetTexture(ShaderType type, uint32_t index, Texture* texture);

		XEResult SetTextureArray(ShaderType type, uint32_t index, TextureArray* textureArray);

		/// <summary>
		/// Sets a Sampler to the Shader stage
		/// </summary>
		/// <param name="type">Type of Shader that will use this buffer</param>
		/// <param name="sampler">Sampler to set</param>
		/// <param name="overrideBindIndex">If true, overrides the Sampler's bind index</param>
		/// <param name="newIndex">New Bind Index if overwritten was enabled</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetSampler(ShaderType type, Sampler* sampler, bool overrideBindIndex = false, uint32_t newIndex = 0);

		/// <summary>
		/// Sets a Shader Buffer to the Shader stage
		/// </summary>
		/// <param name="type">Type of Shader that will use this buffer</param>
		/// <param name="shaderBuffer">Shader Buffer to set</param>
		/// <param name="overrideBindIndex">If true, overrides the Shader Buffer's bind index</param>
		/// <param name="newIndex">New Bind Index if overwritten was enabled</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetShaderBuffer(ShaderType type, ShaderBuffer* shaderBuffer, bool overrideBindIndex = false, uint32_t newIndex = 0);

		/// <summary>
		/// Sets a Shader Buffer that is RW Enabled to the Compute Shader stage
		/// </summary>
		/// <param name="shaderBuffer">Shader Buffer to set</param>
		/// <param name="overrideBindIndex">If true, overrides the Shader Buffer's bind index</param>
		/// <param name="newIndex">New Bind Index if overwritten was enabled</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetShaderRWBufferToCS(ShaderBuffer* shaderBuffer, bool overrideBindIndex = false, uint32_t newIndex = 0);

		/// <summary>
		/// Change the Render Target and keeps the current Depth Stencil
		/// </summary>
		/// <param name="stage_id">Slot where to set the Render Target</param>
		/// <param name="rt">Render Target to set</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetRenderTarget(uint32_t stage_id, RenderTarget* rt);

		/// <summary>
		/// Change the Depth Stencil and keeps the current Render Targets
		/// </summary>
		/// <param name="depthStencilSurface">Depth Stencil to set</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetDepthStencil(DepthStencilSurface* depthStencilSurface);

		/// <summary>
		/// Change the Render Targets and Depth Stencil
		/// </summary>
		/// <param name="numRTs">Number of Render Targets to set</param>
		/// <param name="rts">Render Target Array</param>
		/// <param name="depthStencilSurface">Depth Stencil to set</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetRenderTargetsAndDepthStencil(uint32_t numRTs, RenderTarget* rts[], DepthStencilSurface* depthStencilSurface);

		/// <summary>
		/// Change the Render Targets
		/// </summary>
		/// <param name="numRTs">Number of Render Targets to set</param>
		/// <param name="idx">Indexes in the Render Array for the RTs</param>
		/// <param name="rts">Render Target Array</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetRenderTargets(uint32_t numRTs, uint32_t idxs[], Texture2DArray* rtArray);

		/// <summary>
		/// Sets the Render Targets back to the default ones and keeps the current Depth Stencil
		/// </summary>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult ResetRenderTarget();

		/// <summary>
		/// Sets the Depth Stencil back to the default one and keeps the current Render Targets
		/// </summary>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult ResetDepthStencil();

		/// <summary>
		/// Sets the Render Targets and Depth Stencil back to the default ones
		/// </summary>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult ResetRenderTargetAndSetDepthStencil();

		/// <summary>
		/// Sets the Viewport to the Graphic Device
		/// </summary>
		/// <param name="viewportNums">Number of Viewports to set</param>
		/// <param name="viewports">Array of Viewports</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetViewport(uint32_t viewportNums, Viewport* viewports[]);

		/// <summary>
		/// Sets the Viewport to the Graphic Device in slot 1
		/// </summary>
		/// <param name="viewports">Viewport to set</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetViewport(Viewport* viewport);

		/// <summary>
		/// Reset the Viewport to the default
		/// </summary>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult ResetViewport();

		XEResult						SetIndexBuffer			(IndexBuffer* ib, uint32_t offset = 0);
		XEResult						SetVertexBuffer			(IVertexBuffer* vb, uint32_t stream = 0, uint32_t offset = 0);
		XEResult						SetPrimitiveTopology	(D3D_PRIMITIVE_TOPOLOGY primitive);

		XEResult						GetBlendState			(ID3D11BlendState** blendState, glm::vec4& blendFactor, uint32_t& sampleMask);
		XEResult						SetBlendState			(ID3D11BlendState* blendState, const glm::vec4& blendFactor = XEMathHelpers::Vec4fOne, uint32_t sampleMask = 0xffffffff);

		XEResult						Draw(uint32_t vertexCount, uint32_t startVertex);
		XEResult						DrawIndexed(uint32_t startVertex, uint32_t startIndex, uint32_t indexCount);

		XEResult DispatchComputeShader(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ);

		XEResult						BeginEvent(const std::wstring& eventName);
		XEResult						EndEvent();
		XEResult						SetEventmarker(const std::wstring& eventName);

		//Framework Methods
		XEResult						CheckDevCaps			(const std::wstring& file);
		XEResult						DrawFullScreenQuad		(const glm::vec4& texCoord = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		XEResult						DrawQuad2D				(const RECT& size, const glm::vec4& texCoord = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

};

#endif

