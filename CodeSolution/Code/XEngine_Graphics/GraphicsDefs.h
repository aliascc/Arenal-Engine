/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GRAPHICS_DEFS_H
#define _GRAPHICS_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <string>
#include <stdint.h>
#include <Windows.h>
#include <d3d11_1.h>

#if defined(_DEBUG) || defined(PROFILE)
#include <Initguid.h> 
#include <dxgidebug.h>
#include <d3dcommon.h>
#endif

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
#include "Shaders\ShaderDefs.h"
#include "Base\BaseFunctions.h"
#include "Textures\TextureDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/**************
*   Defines   *
***************/

/// <summary>
/// Debug Name for XEngine Graphic Device for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_MAIN_GD_NAME						"XEngine Graphic Device"

/// <summary>
/// Debug Name for XEngine Main Graphic Device Context for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_MAIN_DC_NAME						"XEngine Main Device Context"

/// <summary>
/// Debug Name for XEngine Main Swap Chain for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_MAIN_SC_NAME						"XEngine Main Swap Chain"

/// <summary>
/// Debug Name for XEngine Main Render Target View for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_MAIN_RTV_NAME						"XEngine Main Render Target View"

/// <summary>
/// Debug Name for XEngine Main Depth Stencil Texture for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_MAIN_DST_NAME						"XEngine Main Depth Stencil Texture"

/// <summary>
/// Debug Name for XEngine Main Depth Stencil View for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_MAIN_DSV_NAME						"XEngine Main Depth Stencil View"

/// <summary>
/// Debug Name for XEngine Default Blend State for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_DEFAULT_BLEND_STATE_NAME			"XEngine Default Blend State"

/// <summary>
/// Debug Name for XEngine Alpha Blend State for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_ALPHA_BLEND_STATE_NAME				"XEngine Alpha Blend State"

/// <summary>
/// Debug Name for XEngine Addictive Blend State for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_ADDICTIVE_BLEND_STATE_NAME			"XEngine Addictive Blend State"

/// <summary>
/// Debug Name for XEngine Opaque Blend State for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_OPAQUE_BLEND_STATE_NAME			"XEngine Opaque Blend State"

/// <summary>
/// Debug Name for XEngine Non-Premultiplied Blend State for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_NON_PREMULTIPLIED_BLEND_STATE_NAME	"XEngine Non-Premultiplied Blend State"

/// <summary>
/// Default Texture 2D Name
/// </summary>
#define XE_DEFAULT_TEX_2D_NAME						L"XEngine Default Texture 2D"

/// <summary>
/// Debug Prefix for Vertex Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_VB_NAME_PREFIX						"_XE_DBG_VB "

/// <summary>
/// Debug Prefix for Index Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_IB_NAME_PREFIX						"_XE_DBG_IB "

/// <summary>
/// Debug Prefix for Structured Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_SB_NAME_PREFIX						"_XE_DBG_SB "

/// <summary>
/// Debug Prefix for Structured Buffer Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_SB_SRV_NAME_PREFIX					"_XE_DBG_SB_SRV "

/// <summary>
/// Debug Prefix for Structured Buffer Unordered Access View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_SB_UAV_NAME_PREFIX					"_XE_DBG_SB_UAV "

/// <summary>
/// Debug Prefix for Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_B_NAME_PREFIX						"_XE_DBG_B "

/// <summary>
/// Debug Prefix for Buffer Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_B_SRV_NAME_PREFIX					"_XE_DBG_B_SRV "

/// <summary>
/// Debug Prefix for Buffer Unordered Access View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_B_UAV_NAME_PREFIX					"_XE_DBG_B_UAV "

/// <summary>
/// Debug Prefix for Constant Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_CB_NAME_PREFIX						"_XE_DBG_CB "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_VS_NAME_PREFIX						"_XE_DBG_VS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_PS_NAME_PREFIX						"_XE_DBG_PS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_GS_NAME_PREFIX						"_XE_DBG_GS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_DS_NAME_PREFIX						"_XE_DBG_DS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_HS_NAME_PREFIX						"_XE_DBG_HS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_CS_NAME_PREFIX						"_XE_DBG_CS "

/// <summary>
/// Debug Prefix for Depth Stencil View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_DEPTH_S_NAME_PREFIX				"_XE_DBG_DEPTH_S "

/// <summary>
/// Debug Prefix for Depth Stencil Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_DEPTH_SRV_NAME_PREFIX				"_XE_DBG_DEPTH_SRV "

/// <summary>
/// Debug Prefix for Depth Stencil Texture when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_DEPTH_T_NAME_PREFIX				"_XE_DBG_DEPTH_T "

/// <summary>
/// Debug Prefix for Render Target View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_RT_NAME_PREFIX						"_XE_DBG_RT "

/// <summary>
/// Debug Prefix for Render Target Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_RT_SRV_NAME_PREFIX					"_XE_DBG_RT_SRV "

/// <summary>
/// Debug Prefix for Render Target Texture when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_RT_T_NAME_PREFIX					"_XE_DBG_RT_T "

/// <summary>
/// Debug Prefix for Vertex Input Layout when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_VIL_NAME_PREFIX					"_XE_DBG_VIL "

/// <summary>
/// Debug Prefix for Texture 2D when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_TEX_2D_NAME_PREFIX					"_XE_DBG_TEX_2D "

/// <summary>
/// Debug Prefix for Texture 2D Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_TEX_2D_SRV_NAME_PREFIX				"_XE_DBG_TEX_2D_SRV "

/// <summary>
/// Debug Prefix for Texture Cube when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_TEX_CUBE_NAME_PREFIX				"_XE_DBG_TEX_CUBE "

/// <summary>
/// Debug Prefix for Texture Cube Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_TEX_CUBE_SRV_NAME_PREFIX			"_XE_DBG_TEX_CUBE_SRV "

/// <summary>
/// Debug Prefix for Texture 2D Array when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_TEX_2D_ARRAY_NAME_PREFIX			"_XE_DBG_TEX_2D_ARR "

/// <summary>
/// Debug Prefix for Texture 2D Array Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_TEX_2D_ARRAY_SRV_NAME_PREFIX		"_XE_DBG_TEX_2D_ARR_SRV "

/// <summary>
/// Debug Prefix for Texture 2D Array Render Targets Views when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define XE_DEBUG_TEX_2D_ARRAY_RTVS_NAME_PREFIX		"_XE_DBG_TEX_2D_ARR_RTVS "

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class GraphicBufferUsage : uint32_t
{
	Default = 0,
	Static,
	Dynamic,
	ReadWrite
};

enum class GraphicBufferAccess : uint32_t
{
	None = 0,
	Read,
	Write,
	ReadWrite
};

enum class GraphicResourceMap : uint32_t
{
	Read = 0,
	Write,
	ReadWrite,
	WriteDiscard,
	WriteNoOvewrite
};

/*************
*   Struct   *
**************/
#if defined(_DEBUG) || defined(PROFILE)

struct GraphicDebugDX sealed : public XEObject
{
	///<summary>
	///DirectX Debugger, Helper to debug
	///</summary>
	ID3D11Debug* m_D3D11Debug = nullptr;

	///<summary>
	///DirectX DXGI Debugger, Helper to debug
	///</summary>
	IDXGIDebug* m_DXGIDebug = nullptr;

	HMODULE m_HandleDXGIDebugDLL = nullptr;

	IDXGIInfoQueue* m_DXGIInfoQueue = nullptr;

	GraphicDevice* m_GraphicDevice = nullptr;

	GraphicDebugDX(GraphicDevice* graphicDevice);
	virtual ~GraphicDebugDX();

	XEResult Initialize();
	void Report();
};

#endif

struct GraphicOptsPreferred sealed : public XEObject
{
	std::wstring m_BackBufferFormatWindowed = L"";
	std::wstring m_BackBufferFormatFullScreen = L"";
	glm::ivec2 m_ScreenResolution = XEMathHelpers::Vec2iZero;
	bool m_FullScreen = false;

	///<summary>
	///Creates the Graphic Device in Single Threaded Mode.
	///Not supported on DirectX 9 Runtime
	///</summary>
	bool m_SingleThreaded = false;

	GraphicOptsPreferred();
};

struct GraphicsPresentationParameters sealed : public XEObject
{
	uint32_t m_BackBufferWidth = 0;

	uint32_t m_BackBufferHeight = 0;

	DXGI_FORMAT m_BackBufferFormatWindowed = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_FORMAT m_BackBufferFormatFullScreen = DXGI_FORMAT_R8G8B8A8_UNORM;

	uint32_t m_BackBufferCount = 2;

	DXGI_SWAP_EFFECT m_SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HWND m_DeviceWindow = nullptr;

	bool m_Windowed = true;

	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	uint32_t m_RefreshRate = 60;

	DXGI_SAMPLE_DESC m_MultiSample;

	/// <summary>
	/// Creates the Graphic Device in Single Threaded Mode.
	/// </summary>
	bool m_SingleThreaded = false;

	//Constructor
	GraphicsPresentationParameters();
};

struct GraphicsCheckFormat sealed : public XEObject
{
	DXGI_FORMAT DisplayFormat = DXGI_FORMAT_R8G8_UNORM;

	DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8_UNORM;

	bool Windowed = false;

	//Constructor
	GraphicsCheckFormat();
};

struct GraphicsCheckDevCaps sealed : public XEObject
{
	glm::ivec2 PS_V = glm::ivec2(2, 0);

	glm::ivec2 VS_V = glm::ivec2(2, 0);

	bool PureDevice = false;

	bool HWTransformedLight = false;

	bool ScissorTest = false;

	uint32_t MaxSimultaneousRTs = 4;

	std::vector<GraphicsCheckFormat> CheckFormatsVect;

	//Constructor
	GraphicsCheckDevCaps();
};

struct GraphicBlendStates sealed : public XEObject
{
	friend class GraphicDevice;

	GraphicBlendStates() = delete;

	private:
		static XEResult InitStates(GraphicDevice* grahicDevice);
		static void ReleaseStates();

	public:
		static ID3D11BlendState* m_DefaultState;
		static ID3D11BlendState* m_AdditiveState;
		static ID3D11BlendState* m_AlphaBlendState;
		static ID3D11BlendState* m_OpaqueState;
		static ID3D11BlendState* m_NonPremultipliedState;
};

namespace XEGraphicHelpers
{
	extern glm::ivec2 GetWindowSize(HWND hWnd);

	extern DXGI_FORMAT GetPXFormatFromString(const std::wstring& pxFormat);

	extern D3D11_USAGE GetDXUsage(GraphicBufferUsage bufferUsage);

	extern UINT GetDXBufferAccess(GraphicBufferAccess bufferAccess);

	extern D3D11_MAP GetDXResourceMap(GraphicResourceMap resourceMap);

	extern uint32_t GetSizeOfDXFormat(DXGI_FORMAT format);

	extern std::wstring GetShaderTypeString(ShaderType shaderType);

	extern ShaderType GetShaderTypeFromString(std::wstring shaderType);

	extern std::wstring GetTextureTypeString(TextureType textureType);

	extern TextureType GetTextureTypeString(std::wstring textureType);

	template<class T>
	inline void SetDebugObjectName(T* resource, const std::string& name)
	{
#if defined(_DEBUG) || defined(PROFILE)
		XEAssert(resource != nullptr);

		if(resource == nullptr)
		{
			return;
		}

		uint32_t size = (uint32_t)name.size();

		resource->SetPrivateData(WKPDID_D3DDebugObjectName, size, name.c_str());
#endif
	}
}

#endif