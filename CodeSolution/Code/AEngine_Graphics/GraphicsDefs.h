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

#pragma once

#ifndef _GRAPHICS_DEFS_H
#define _GRAPHICS_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shaders\ShaderDefs.h"
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
/// Debug Name for AEngine Graphic Device for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_MAIN_GD_NAME                        "AEngine Graphic Device"

/// <summary>
/// Debug Name for AEngine Main Graphic Device Context for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_MAIN_DC_NAME                        "AEngine Main Device Context"

/// <summary>
/// Debug Name for AEngine Main Swap Chain for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_MAIN_SC_NAME                        "AEngine Main Swap Chain"

/// <summary>
/// Debug Name for AEngine Main Render Target View for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_MAIN_RTV_NAME                        "AEngine Main Render Target View"

/// <summary>
/// Debug Name for AEngine Main Depth Stencil Texture for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_MAIN_DST_NAME                        "AEngine Main Depth Stencil Texture"

/// <summary>
/// Debug Name for AEngine Main Depth Stencil View for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_MAIN_DSV_NAME                        "AEngine Main Depth Stencil View"

/// <summary>
/// Debug Name for AEngine Default Blend State for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_DEFAULT_BLEND_STATE_NAME            "AEngine Default Blend State"

/// <summary>
/// Debug Name for AEngine Alpha Blend State for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_ALPHA_BLEND_STATE_NAME              "AEngine Alpha Blend State"

/// <summary>
/// Debug Name for AEngine Addictive Blend State for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_ADDICTIVE_BLEND_STATE_NAME          "AEngine Addictive Blend State"

/// <summary>
/// Debug Name for AEngine Opaque Blend State for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_OPAQUE_BLEND_STATE_NAME             "AEngine Opaque Blend State"

/// <summary>
/// Debug Name for AEngine Non-Premultiplied Blend State for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_NON_PREMULTIPLIED_BLEND_STATE_NAME  "AEngine Non-Premultiplied Blend State"

/// <summary>
/// Default Texture 2D Name
/// </summary>
#define AE_DEFAULT_TEX_2D_NAME                       "AEngine Default Texture 2D"

/// <summary>
/// Debug Prefix for Vertex Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_VB_NAME_PREFIX                      "_AE_DBG_VB "

/// <summary>
/// Debug Prefix for Index Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_IB_NAME_PREFIX                      "_AE_DBG_IB "

/// <summary>
/// Debug Prefix for Structured Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_SB_NAME_PREFIX                      "_AE_DBG_SB "

/// <summary>
/// Debug Prefix for Structured Buffer Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_SB_SRV_NAME_PREFIX                  "_AE_DBG_SB_SRV "

/// <summary>
/// Debug Prefix for Structured Buffer Unordered Access View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_SB_UAV_NAME_PREFIX                  "_AE_DBG_SB_UAV "

/// <summary>
/// Debug Prefix for Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_B_NAME_PREFIX                       "_AE_DBG_B "

/// <summary>
/// Debug Prefix for Buffer Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_B_SRV_NAME_PREFIX                   "_AE_DBG_B_SRV "

/// <summary>
/// Debug Prefix for Buffer Unordered Access View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_B_UAV_NAME_PREFIX                    "_AE_DBG_B_UAV "

/// <summary>
/// Debug Prefix for Constant Buffer when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_CB_NAME_PREFIX                       "_AE_DBG_CB "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_VS_NAME_PREFIX                       "_AE_DBG_VS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_PS_NAME_PREFIX                       "_AE_DBG_PS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_GS_NAME_PREFIX                       "_AE_DBG_GS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_DS_NAME_PREFIX                       "_AE_DBG_DS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_HS_NAME_PREFIX                       "_AE_DBG_HS "

/// <summary>
/// Debug Prefix for Compute Shader when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_CS_NAME_PREFIX                       "_AE_DBG_CS "

/// <summary>
/// Debug Prefix for Depth Stencil View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_DEPTH_S_NAME_PREFIX                  "_AE_DBG_DEPTH_S "

/// <summary>
/// Debug Prefix for Depth Stencil Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_DEPTH_SRV_NAME_PREFIX                "_AE_DBG_DEPTH_SRV "

/// <summary>
/// Debug Prefix for Depth Stencil Texture when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_DEPTH_T_NAME_PREFIX                  "_AE_DBG_DEPTH_T "

/// <summary>
/// Debug Prefix for Render Target View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_RT_NAME_PREFIX                       "_AE_DBG_RT "

/// <summary>
/// Debug Prefix for Render Target Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_RT_SRV_NAME_PREFIX                   "_AE_DBG_RT_SRV "

/// <summary>
/// Debug Prefix for Render Target Texture when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_RT_T_NAME_PREFIX                     "_AE_DBG_RT_T "

/// <summary>
/// Debug Prefix for Vertex Input Layout when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_VIL_NAME_PREFIX                      "_AE_DBG_VIL "

/// <summary>
/// Debug Prefix for Texture 2D when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_TEX_2D_NAME_PREFIX                   "_AE_DBG_TEX_2D "

/// <summary>
/// Debug Prefix for Texture 2D Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_TEX_2D_SRV_NAME_PREFIX               "_AE_DBG_TEX_2D_SRV "

/// <summary>
/// Debug Prefix for Texture Cube when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_TEX_CUBE_NAME_PREFIX                 "_AE_DBG_TEX_CUBE "

/// <summary>
/// Debug Prefix for Texture Cube Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_TEX_CUBE_SRV_NAME_PREFIX             "_AE_DBG_TEX_CUBE_SRV "

/// <summary>
/// Debug Prefix for Texture 2D Array when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_TEX_2D_ARRAY_NAME_PREFIX             "_AE_DBG_TEX_2D_ARR "

/// <summary>
/// Debug Prefix for Texture 2D Array Shader Resource View when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_TEX_2D_ARRAY_SRV_NAME_PREFIX         "_AE_DBG_TEX_2D_ARR_SRV "

/// <summary>
/// Debug Prefix for Texture 2D Array Render Targets Views when setting Debug Name for DirectX Graphic Debugger
/// </summary>
#define AE_DEBUG_TEX_2D_ARRAY_RTVS_NAME_PREFIX        "_AE_DBG_TEX_2D_ARR_RTVS "

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

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

/// <summary>
/// Graphic Device Helpers to debug the game app
/// </summary>
struct GraphicDebugDX sealed : public AEObject
{
    /// <summary>
    /// DirectX Debugger, Helper to debug
    /// </summary>
    ID3D11Debug* m_D3D11Debug = nullptr;

    /// <summary>
    /// DirectX DXGI Debugger, Helper to debug
    /// </summary>
    IDXGIDebug* m_DXGIDebug = nullptr;

    HMODULE m_HandleDXGIDebugDLL = nullptr;

    IDXGIInfoQueue* m_DXGIInfoQueue = nullptr;

    GraphicDevice* m_GraphicDevice = nullptr;

    GraphicDebugDX(GraphicDevice* graphicDevice);
    virtual ~GraphicDebugDX();

    AEResult Initialize();
    void Report();
};

#endif //AE_GRAPHIC_DEBUG_DEVICE

struct GraphicOptsPreferred sealed : public AEObject
{
    std::string m_BackBufferFormatWindowed = "";
    std::string m_BackBufferFormatFullScreen = "";
    glm::ivec2 m_ScreenResolution = AEMathHelpers::Vec2iZero;
    bool m_FullScreen = false;

    ///<summary>
    ///Creates the Graphic Device in Single Threaded Mode.
    ///Not supported on DirectX 9 Runtime
    ///</summary>
    bool m_SingleThreaded = false;

    GraphicOptsPreferred();
};

struct GraphicsPresentationParameters sealed : public AEObject
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

struct GraphicsCheckFormat sealed : public AEObject
{
    DXGI_FORMAT DisplayFormat = DXGI_FORMAT_R8G8_UNORM;

    DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8_UNORM;

    bool Windowed = false;

    //Constructor
    GraphicsCheckFormat();
};

struct GraphicsCheckDevCaps sealed : public AEObject
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

struct GraphicBlendStates sealed : public AEObject
{
    friend class GraphicDevice;

    GraphicBlendStates() = delete;

    private:
        static AEResult InitStates(GraphicDevice* grahicDevice);
        static void ReleaseStates();

    public:
        static ID3D11BlendState* m_DefaultState;
        static ID3D11BlendState* m_AdditiveState;
        static ID3D11BlendState* m_AlphaBlendState;
        static ID3D11BlendState* m_OpaqueState;
        static ID3D11BlendState* m_NonPremultipliedState;
};

namespace AEGraphicHelpers
{
    extern glm::ivec2 GetWindowSize(HWND hWnd);

    extern DXGI_FORMAT GetPXFormatFromString(const std::string& pxFormat);

    extern D3D11_USAGE GetDXUsage(GraphicBufferUsage bufferUsage);

    extern UINT GetDXBufferAccess(GraphicBufferAccess bufferAccess);

    extern D3D11_MAP GetDXResourceMap(GraphicResourceMap resourceMap);

    extern uint32_t GetSizeOfDXFormat(DXGI_FORMAT format);

    extern std::string GetShaderTypeString(ShaderType shaderType);

    extern ShaderType GetShaderTypeFromString(std::string shaderType);

    extern std::string GetTextureTypeString(TextureType textureType);

    extern TextureType GetTextureTypeString(std::string textureType);

    template<class T>
    inline void SetDebugObjectName(T* resource, const std::string& name)
    {
#if defined(AE_GRAPHIC_DEBUG_DEVICE)
        AEAssert(resource != nullptr);

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
