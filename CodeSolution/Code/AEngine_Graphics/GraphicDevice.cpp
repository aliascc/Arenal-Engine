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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicDevice.h"
#include "Utils\Viewport.h"
#include "Models\MeshPart.h"
#include "Shaders\ShaderDefs.h"
#include "Shapes\QuadShape2D.h"
#include "Vertex\IndexBuffer.h"
#include "Textures\Texture2D.h"
#include "Shaders\HullShader.h"
#include "Shaders\PixelShader.h"
#include "Vertex\VertexLayout.h"
#include "Shaders\DomainShader.h"
#include "Shaders\VertexShader.h"
#include "Vertex\IVertexBuffer.h"
#include "Textures\RenderTarget.h"
#include "Shaders\ComputeShader.h"
#include "Shaders\GeometryShader.h"
#include "Vertex\Types\VertexTypes.h"
#include "Shaders\Variables\Sampler.h"
#include "Textures\DepthStencilSurface.h"
#include "Shaders\Variables\TextureArray.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Variables\Texture2DArray.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GraphicDevice::GraphicDevice(HWND hMainWnd, const GraphicOptsPreferred& graphicOpts)
{
    AEAssert(hMainWnd != nullptr);

    m_GraphicPP.m_DeviceWindow =  hMainWnd;

    m_GraphicPP.m_SingleThreaded = graphicOpts.m_SingleThreaded;

#ifndef AE_EDITOR_MODE
    if(graphicOpts.m_FullScreen)
    {
        m_gPP.m_GameBackBufferWidth         = graphicOpts.m_ScreenResolution.x;
        m_gPP.m_GameBackBufferHeight        = graphicOpts.m_ScreenResolution.y;

        m_gPP.m_BackBufferFormatFullScreen  = AEGraphicHelpers::GetPXFormatFromString(graphicOpts.m_BackBufferFormatFullScreen);

        m_gPP.m_Windowed = false;
    }
    else 
#endif
    {
        glm::ivec2 wndSize = AEGraphicHelpers::GetWindowSize(hMainWnd);

#ifdef AE_EDITOR_MODE
        m_GraphicPP.m_EditorBackBufferWidth   = wndSize.x;
        m_GraphicPP.m_EditorBackBufferHeight  = wndSize.y;

        AETODO("Check default sizes");
        m_GraphicPP.m_GameBackBufferWidth     = 256;
        m_GraphicPP.m_GameBackBufferHeight    = 256;
#else
        m_gPP.m_GameBackBufferWidth     = wndSize.x;
        m_gPP.m_GameBackBufferHeight    = wndSize.y;
#endif
        
        m_GraphicPP.m_BackBufferFormatWindowed = AEGraphicHelpers::GetPXFormatFromString(graphicOpts.m_BackBufferFormatWindowed);

        m_GraphicPP.m_Windowed = true;
    }

    m_QuadShape2D = new QuadShape2D(*this, true);
}

GraphicDevice::~GraphicDevice()
{
    ReleaseDefaultTextures();

    CleanUpBasicVertexTypes();

    GraphicBlendStates::ReleaseStates();

    DeleteMem(m_QuadShape2D);

    DeleteMemArr(m_CurrentRenderTargetViewsDX);

    CleanUpGameDepthStencil();
    CleanUpGameRenderTargets();

#ifdef AE_EDITOR_MODE
    CleanUpEditorRenderTargets();
#endif

    ReleaseCOM(m_SwapChainDX);

#if defined(AE_GRAPHIC_DEBUG_DEVICE)
    ReleaseCOM(m_UserDefinedAnnotationDX);
#endif

    // Restore all default settings.
    if(m_DeviceContextDX != nullptr)
    {
        m_DeviceContextDX->ClearState();
        m_DeviceContextDX->Flush();
    }
    
    ReleaseCOM(m_DeviceContextDX);
    ReleaseCOM(m_DeviceDX);
}

void GraphicDevice::InitBasicVertexTypes()
{
    VertexPosition::GetVertexLayout(*this);
    VertexPositionColor::GetVertexLayout(*this);
    VertexPositionNormal::GetVertexLayout(*this);
    VertexPositionTexture::GetVertexLayout(*this);
    VertexPositionNormalColor::GetVertexLayout(*this);
    VertexPositionNormalTexture::GetVertexLayout(*this);
    VertexPositionNormalTextureTexture2::GetVertexLayout(*this);
    VertexPositionNormalTextureIndicesWeight::GetVertexLayout(*this);
    VertexPositionNormalTangentBinormalTexture::GetVertexLayout(*this);
    VertexPositionNormalTangentBinormalTextureTexture2::GetVertexLayout(*this);
    VertexPositionNormalTangentBinormalTextureIndicesWeight::GetVertexLayout(*this);
}

void GraphicDevice::CleanUpBasicVertexTypes()
{
    VertexPosition::DeleteVertexLayout();
    VertexPositionColor::DeleteVertexLayout();
    VertexPositionNormal::DeleteVertexLayout();
    VertexPositionTexture::DeleteVertexLayout();
    VertexPositionNormalColor::DeleteVertexLayout();
    VertexPositionNormalTexture::DeleteVertexLayout();
    VertexPositionNormalTextureTexture2::DeleteVertexLayout();
    VertexPositionNormalTextureIndicesWeight::DeleteVertexLayout();
    VertexPositionNormalTangentBinormalTexture::DeleteVertexLayout();
    VertexPositionNormalTangentBinormalTextureTexture2::DeleteVertexLayout();
    VertexPositionNormalTangentBinormalTextureIndicesWeight::DeleteVertexLayout();
}

AEResult GraphicDevice::CreateDefaultTextures()
{
    if (m_DefaultTexture2D != nullptr)
    {
        return AEResult::Ok;
    }

    AEResult ret = AEResult::Ok;

    m_DefaultTexture2D = new Texture2D(*this, AE_DEFAULT_TEX_2D_NAME);

    ret = m_DefaultTexture2D->CreateColorTexture(128, 128, AEColors::Fuchsia);
    if (ret != AEResult::Ok)
    {
        AERelease(m_DefaultTexture2D);

        return ret;
    }

    return AEResult::Ok;
}

void GraphicDevice::ReleaseDefaultTextures()
{
    AERelease(m_DefaultTexture2D);
}

void GraphicDevice::ResetHalfPixel()
{
    m_HalfPixel.x = (0.5f / (float)m_GraphicPP.m_GameBackBufferWidth);
    m_HalfPixel.y = (0.5f / (float)m_GraphicPP.m_GameBackBufferHeight);
}

void GraphicDevice::OnLostDevice()
{
    AEAssert(m_IsReady);

    //First all Related to the Graphic Device
#ifdef AE_EDITOR_MODE
    CleanUpEditorRenderTargets();
#else
    CleanUpGameDepthStencil();
    CleanUpGameRenderTargets();
#endif
}

void GraphicDevice::OnResetDevice()
{
    AEAssert(m_IsReady);

    //First all Related to the Graphic Device

#ifdef AE_EDITOR_MODE
    CreateEditorRenderTarget();
#else
    InitGameRTDS();
#endif

    //Second The Rest
    ResetHalfPixel();
}

AEResult GraphicDevice::InitDXConfiguration()
{
    InitSwapChainDesc();

    return AEResult::Ok;
}

void GraphicDevice::InitSwapChainDesc()
{
    ZeroMemory(&m_SwapChainDescDX, sizeof(DXGI_SWAP_CHAIN_DESC));
    
    //Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
#ifdef AE_EDITOR_MODE
    m_SwapChainDescDX.BufferDesc.Width                      = m_GraphicPP.m_EditorBackBufferWidth;
    m_SwapChainDescDX.BufferDesc.Height                     = m_GraphicPP.m_EditorBackBufferHeight;
#else
    m_SwapChainDescDX.BufferDesc.Width                      = m_GraphicPP.m_GameBackBufferWidth;
    m_SwapChainDescDX.BufferDesc.Height                     = m_GraphicPP.m_GameBackBufferHeight;
#endif

    AETODO("Check into Scaling");
    m_SwapChainDescDX.BufferDesc.Scaling                    = DXGI_MODE_SCALING_UNSPECIFIED;
    m_SwapChainDescDX.BufferDesc.ScanlineOrdering           = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    AETODO("Check Numerator and Denominator for Refresh Rate");
    m_SwapChainDescDX.BufferDesc.RefreshRate.Numerator      = 60;
    m_SwapChainDescDX.BufferDesc.RefreshRate.Denominator    = 1;

    if(m_GraphicPP.m_Windowed)
    {
        m_SwapChainDescDX.BufferDesc.Format = m_GraphicPP.m_BackBufferFormatWindowed;
    }
    else
    {
        m_SwapChainDescDX.BufferDesc.Format = m_GraphicPP.m_BackBufferFormatFullScreen;
    }

    m_SwapChainDescDX.OutputWindow          = m_GraphicPP.m_DeviceWindow;
    m_SwapChainDescDX.Windowed              = m_GraphicPP.m_Windowed;
    m_SwapChainDescDX.SampleDesc            = m_GraphicPP.m_MultiSample;

    AETODO("Check into Usage Render Target Output");
    AETODO("Add option to Back Buffer");
    m_SwapChainDescDX.BufferCount           = m_GraphicPP.m_BackBufferCount; // = 2;                               // Use double buffering to minimize latency.
    m_SwapChainDescDX.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    
    //DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Metro style apps must use this SwapEffect.
    m_SwapChainDescDX.SwapEffect            = m_GraphicPP.m_SwapEffect;
    
    AETODO("Check into Swap Chain Flags");
    m_SwapChainDescDX.Flags                 = 0;
}

AEResult GraphicDevice::CreateSwapChain()
{
    // To correctly create the swap chain, we must use the IDXGIFactory that was
    // used to create the device.  If we tried to use a different IDXGIFactory instance
    // (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
    // This function is being called with a device from a different IDXGIFactory."
    IDXGIDevice* dxgiDevice     = nullptr;
    IDXGIAdapter* dxgiAdapter   = nullptr;
    IDXGIFactory* dxgiFactory   = nullptr;
    HRESULT hr = S_OK;

    hr = m_DeviceDX->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
    
    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

    if(hr != S_OK)
    {
        DisplayError(hr);

        ReleaseCOM(dxgiDevice);

        return AEResult::Fail;
    }

    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

    if(hr != S_OK)
    {
        DisplayError(hr);
        
        ReleaseCOM(dxgiAdapter);
        ReleaseCOM(dxgiDevice);

        return AEResult::Fail;
    }

    hr = dxgiFactory->CreateSwapChain(m_DeviceDX, &m_SwapChainDescDX, &m_SwapChainDX);
    
    if(hr != S_OK)
    {
        DisplayError(hr);
        
        ReleaseCOM(dxgiFactory);
        ReleaseCOM(dxgiAdapter);
        ReleaseCOM(dxgiDevice);

        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName(m_SwapChainDX, AE_DEBUG_MAIN_SC_NAME);

    AETODO("Check into this");
    //// Ensure that DXGI does not queue more than one frame at a time. This both reduces
    //// latency and ensures that the application will only render after each VSync, minimizing
    //// power consumption.
    //dxgiDevice->SetMaximumFrameLatency(1);
    
    ReleaseCOM(dxgiFactory);
    ReleaseCOM(dxgiAdapter);
    ReleaseCOM(dxgiDevice);

    return AEResult::Ok;
}

void GraphicDevice::InitGameViewport()
{
    // Set the viewport Transform.
    m_GameViewportDX.TopLeftX = 0;
    m_GameViewportDX.TopLeftY = 0;
    m_GameViewportDX.Width    = static_cast<float>(m_GraphicPP.m_GameBackBufferWidth);
    m_GameViewportDX.Height   = static_cast<float>(m_GraphicPP.m_GameBackBufferHeight);
    m_GameViewportDX.MinDepth = 0.0f;
    m_GameViewportDX.MaxDepth = 1.0f;

    m_DeviceContextDX->RSSetViewports(1, &m_GameViewportDX);
}

AEResult GraphicDevice::InitGameRTDS()
{    
    if(CreateGameDepthStencil() == AEResult::Fail)
    {
        return AEResult::Fail;
    }

    if(CreateGameRenderTarget() == AEResult::Fail)
    {
        CleanUpGameDepthStencil();

        return AEResult::Fail;
    }

    //////////////////////////////////////////////////////////////////////////////////
    //Bind the render target view and depth/stencil view to the pipeline.
    memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);

    m_CurrentRenderTargetViewsDX[0] = m_GameRenderTargetViewDX;
    m_HighestRenderTargetStage      = 1;

    m_CurrentDepthStencilViewDX     = m_GameDepthStencilViewDX;

    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::CreateGameDepthStencil()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };

    depthStencilDesc.Format             = m_GraphicPP.m_DepthStencilFormat;

    depthStencilDesc.Width              = m_GraphicPP.m_GameBackBufferWidth;
    depthStencilDesc.Height             = m_GraphicPP.m_GameBackBufferHeight;

    depthStencilDesc.MipLevels          = 1; //Depth Stencil for Default has 1 Mip Map
    depthStencilDesc.ArraySize          = 1; //Depth Stencil for Default has 1 Array Size
    
    depthStencilDesc.SampleDesc         = m_GraphicPP.m_MultiSample;

    AETODO("Check Flags");
    depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags     = 0; 
    depthStencilDesc.MiscFlags          = 0;

    HRESULT hr = m_DeviceDX->CreateTexture2D(&depthStencilDesc, nullptr, &m_GameDepthStencilBufferDX);
    if(hr != S_OK)
    {
        DisplayError(hr);
        return AEResult::Fail;
    }
    AEGraphicHelpers::SetDebugObjectName(m_GameDepthStencilBufferDX, AE_DEBUG_GAME_DST_NAME);

    AETODO("Check Depth Stencil View Desc");
    hr = m_DeviceDX->CreateDepthStencilView(m_GameDepthStencilBufferDX, 0, &m_GameDepthStencilViewDX);
    if(hr != S_OK)
    {
        CleanUpGameDepthStencil();

        DisplayError(hr);
        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName(m_GameDepthStencilViewDX, AE_DEBUG_GAME_DSV_NAME);

    return AEResult::Ok;
}

AEResult GraphicDevice::CreateGameRenderTarget()
{
#ifdef AE_EDITOR_MODE
    D3D11_TEXTURE2D_DESC dxDesc = { 0 };

    dxDesc.Width                = m_GraphicPP.m_GameBackBufferWidth;
    dxDesc.Height               = m_GraphicPP.m_GameBackBufferHeight;
    dxDesc.MipLevels            = 1;
    dxDesc.ArraySize            = 1;
    dxDesc.Usage                = D3D11_USAGE_DEFAULT;
    dxDesc.BindFlags            = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    dxDesc.CPUAccessFlags       = 0;
    dxDesc.Format               = m_GraphicPP.m_BackBufferFormatWindowed;    
    dxDesc.MiscFlags            = 0;
    dxDesc.SampleDesc.Count     = 1;
    dxDesc.SampleDesc.Quality   = 0;
    AETODO("Check multisample");
    AETODO("Check other flags for RT and if Mip Map is needed");

    HRESULT hr = m_DeviceDX->CreateTexture2D(&dxDesc, nullptr, &m_GameRenderTargetBufferDX);
    if (hr != S_OK)
    {
        CleanUpGameRenderTargets();
        DisplayError(hr);
        return AEResult::CreateTextureFailed;
    }
    AEGraphicHelpers::SetDebugObjectName(m_GameRenderTargetBufferDX, AE_DEBUG_GAME_RTT_NAME);

    hr = m_DeviceDX->CreateShaderResourceView(m_GameRenderTargetBufferDX, nullptr, &m_GameRenderTargetSRV);
    if (hr != S_OK)
    {
        CleanUpGameRenderTargets();
        DisplayError(hr);
        return AEResult::CreateSRViewFailed;
    }
    AEGraphicHelpers::SetDebugObjectName(m_GameRenderTargetSRV, AE_DEBUG_GAME_SRV_NAME);

    AETODO("Check Render Target Desc");
    hr = m_DeviceDX->CreateRenderTargetView(m_GameRenderTargetBufferDX, nullptr, &m_GameRenderTargetViewDX);
    if (hr != S_OK)
    {
        CleanUpGameRenderTargets();
        DisplayError(hr);
        return AEResult::CreateRTViewFailed;
    }
    AEGraphicHelpers::SetDebugObjectName(m_GameRenderTargetViewDX, AE_DEBUG_GAME_RTV_NAME);

#else

    ID3D11Texture2D* backBuffer = nullptr;

    HRESULT hr = m_SwapChainDX->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (hr != S_OK)
    {
        DisplayError(hr);
        return AEResult::Fail;
    }

    hr = m_DeviceDX->CreateRenderTargetView(backBuffer, 0, &m_GameRenderTargetViewDX);
    ReleaseCOM(backBuffer);
    if (hr != S_OK)
    {
        DisplayError(hr);
        return AEResult::Fail;
    }
    AEGraphicHelpers::SetDebugObjectName(m_GameRenderTargetViewDX, AE_DEBUG_GAME_RTV_NAME);

#endif

    return AEResult::Ok;
}

void GraphicDevice::ResetDevice()
{
    AEAssert(m_IsReady);

    HRESULT hr = S_OK;

    //Do anything that needs to be done with graphic device when it is lost
    OnLostDevice();

    AETODO("Check to see reset failures to Device");

    //Resize Swap Chain
    AETODO("Check to see if we are in full screen or window");
    AETODO("Check Swap Chain Flags");
#ifdef AE_EDITOR_MODE
    hr = m_SwapChainDX->ResizeBuffers(1, m_GraphicPP.m_EditorBackBufferWidth, m_GraphicPP.m_EditorBackBufferHeight, m_GraphicPP.m_BackBufferFormatFullScreen, 0);
#else
    hr = m_SwapChainDX->ResizeBuffers(1, m_GraphicPP.m_GameBackBufferWidth, m_GraphicPP.m_GameBackBufferHeight, m_GraphicPP.m_BackBufferFormatFullScreen, 0);
#endif

    AEAssert(hr == S_OK);
    if(hr != S_OK)
    {
        DisplayError(hr);
        return;
    }

#ifdef AE_EDITOR_MODE
    InitEditorViewport();
#else
    InitGameViewport();
#endif

    //Do Anything that needs to be done when Graphic Device is reset
    OnResetDevice();
}

void GraphicDevice::Resize(uint32_t width, uint32_t height)
{
    AEAssert(m_IsReady);

    m_GraphicPP.m_GameBackBufferWidth     = width;
    m_GraphicPP.m_GameBackBufferHeight    = height;

#ifdef AE_EDITOR_MODE
    ResetGameRenderTargetView();
#else
    ResetDevice();
#endif
}

AEResult GraphicDevice::InitDevice()
{
    //If device has already been initialize return ok
    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    //Initialize DirectX Configuration with AE_Presentation Parameter
    if(InitDXConfiguration() != AEResult::Ok)
    {
        return AEResult::CreateDXConfFail;
    }

    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef AE_GRAPHIC_DEBUG_DEVICE
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    if(m_GraphicPP.m_SingleThreaded)
    {
        createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
    }

    D3D_FEATURE_LEVEL featureLevelsArr[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };
    const uint32_t numFeatureLeves = _countof(featureLevelsArr);

    D3D_FEATURE_LEVEL featureLevel;

    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

    hr = D3D11CreateDevice(
        nullptr,                    //Default adapter
        driverType,                 //Driver Type
        nullptr,                    //No software device
        createDeviceFlags,          //Device Create Flags
        featureLevelsArr,           //Feature Levels
        numFeatureLeves,            //Count of feature levels
        D3D11_SDK_VERSION,          //DirectX Version 11
        &m_DeviceDX,                //DirectX Device
        &featureLevel,              //Feature Level
        &m_DeviceContextDX);        //Device Immediate Context    

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::CreateDXDeviceFail;
    }

    //Add names to DX Objects for Debugging
    AEGraphicHelpers::SetDebugObjectName(m_DeviceDX, AE_DEBUG_MAIN_GD_NAME);
    AEGraphicHelpers::SetDebugObjectName(m_DeviceContextDX, AE_DEBUG_MAIN_DC_NAME);

    if(featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AELogHelpers::Log(LogLevel::Error, LogSystem::Graphics, "DX_11_UNSUPPORTED_ERR_MSG", __FUNCTION__);

        return AEResult::FeatureLvlUnsupported;
    }

    //Init Pre Graphic Objects
    if(InitPreGraphicObjects() != AEResult::Ok)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AETODO("Log Error");

        AETODO("Return error");
        return AEResult::Fail;
    }

    //Create Swap Chain
    if(CreateSwapChain() != AEResult::Ok)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AELogHelpers::Log(LogLevel::Error, LogSystem::Graphics, "DX_11_FAIL_SWAP_CHAIN_ERR_MSG", __FUNCTION__);

        return AEResult::CreateDXSwapChainFail;
    }

#ifdef AE_EDITOR_MODE

    //Create Editor Render Targets
    if (CreateEditorRenderTarget() != AEResult::Ok)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AELogHelpers::Log(LogLevel::Error, LogSystem::Graphics, "DX_11_FAIL_RENDER_TARGETS_ERR_MSG", __FUNCTION__);

        return AEResult::CreateDXDefaultRTDSFail;
    }

    //Create Editor Viewport
    InitEditorViewport();

#endif

    //Create Game Render Targets
    if(InitGameRTDS() != AEResult::Ok)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AELogHelpers::Log(LogLevel::Error, LogSystem::Graphics, "DX_11_FAIL_RENDER_TARGETS_ERR_MSG", __FUNCTION__);

        return AEResult::CreateDXDefaultRTDSFail;
    }

    //Create Game Viewport
    InitGameViewport();

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

    hr = m_DeviceContextDX->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void**>(&m_UserDefinedAnnotationDX));
    
    if(hr != S_OK)
    {
        DisplayError(hr);
        
        AETODO("Log error");
    }

#endif

    //Reset Half Pixel
    ResetHalfPixel();

    //Init Graphic Objects
    if(InitGraphicObjects() != AEResult::Ok)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AETODO("Log Error");

        AETODO("Return error");
        return AEResult::Fail;
    }

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult GraphicDevice::InitPreGraphicObjects()
{
    AETODO("Check what is the max of render targets and set");
    m_MaxNumRenderTargets = 4;

    m_CurrentRenderTargetViewsDX = new ID3D11RenderTargetView*[m_MaxNumRenderTargets];

    memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);

    return AEResult::Ok;
}

AEResult GraphicDevice::InitGraphicObjects()
{
    InitBasicVertexTypes();

    AETODO("check return and Log error");
    CreateDefaultTextures();

    if(GraphicBlendStates::InitStates(this) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    if(m_QuadShape2D->BuildQuad() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

void GraphicDevice::Clear(bool target, uint32_t rt_stage_id, bool depth, bool stencil, const Color& color, float depthVal, uint8_t stencilVal)
{
    AEAssert(m_IsReady);

    if(target)
    {
        AEAssert(rt_stage_id < m_MaxNumRenderTargets);
        if(rt_stage_id >= m_MaxNumRenderTargets)
        {
            return;
        }

        AEAssert(m_CurrentRenderTargetViewsDX[rt_stage_id] != nullptr);
        if(m_CurrentRenderTargetViewsDX[rt_stage_id] == nullptr)
        {
            return;
        }

        AETODO("Back buffer color argb order check");
        FLOAT clearColor[] = 
        {
            (float)color.r / 255.0f,
            (float)color.g / 255.0f,
            (float)color.b / 255.0f,
            (float)color.a / 255.0f,
        };

        m_DeviceContextDX->ClearRenderTargetView(m_CurrentRenderTargetViewsDX[rt_stage_id], clearColor);
    }

    if(depth || stencil)
    {
        UINT cFlag = 0;

        if(depth)
        {
            cFlag |= D3D11_CLEAR_DEPTH;
        }

        if(stencil)
        {
            cFlag |= D3D11_CLEAR_STENCIL;
        }

        m_DeviceContextDX->ClearDepthStencilView(m_CurrentDepthStencilViewDX, cFlag, depthVal, stencilVal);
    }
}

void GraphicDevice::Present()
{
    AEAssert(m_IsReady);

    AETODO("Check Present1");
    //AETODO("CHECK DXGI_PRESENT_PARAMETERS");
 //   // The application may optionally specify "dirty" or "scroll" rects to improve efficiency
 //   // in certain scenarios.  In this sample, however, we do not utilize those features.
    //DXGI_PRESENT_PARAMETERS parameters;
 //   parameters.DirtyRectsCount = 0;
 //   parameters.pDirtyRects = nullptr;
 //   parameters.pScrollRect = nullptr;
 //   parameters.pScrollOffset = nullptr;
    
    AETODO("Correct Parameters pass to Present to make it more flexible");
    HRESULT hr = m_SwapChainDX->Present(0, 0);

    AEAssert(hr == S_OK)
    if(hr != S_OK)
    {
        DisplayError(hr);
    }
}

void GraphicDevice::SetRenderTarget(uint32_t stage_id, RenderTarget* rt)
{
    AEAssert(m_IsReady);

    ///////////////////////////////////////
    //Verify that it does not past max num of render target
    AEAssert(stage_id < m_MaxNumRenderTargets);
    if (stage_id >= m_MaxNumRenderTargets)
    {
        return;
    }
    
    ID3D11RenderTargetView* rtDX = nullptr;

    ///////////////////////////////////////
    //Set Render Target
    if(rt != nullptr)
    {
        rtDX = rt->GetDXRenderTargetView();
        AEAssert(rtDX != nullptr);
    }

    ///////////////////////////////////////
    //Set Render Target
    m_CurrentRenderTargetViewsDX[stage_id] = rtDX;

    ///////////////////////////////////////
    //Set Highest Render Target Stage
    m_HighestRenderTargetStage = ((stage_id + 1) > m_HighestRenderTargetStage) ? (stage_id + 1) : m_HighestRenderTargetStage;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

    ///////////////////////////////////////
    //Set Highest Render Target Stage if
    //the last render target was null
    if (m_CurrentRenderTargetViewsDX[stage_id] == nullptr && stage_id == m_HighestRenderTargetStage)
    {
        bool setNewHigh = false;

        for (uint32_t i = stage_id; i >= 0 && !setNewHigh; i--)
        {
            if (m_CurrentRenderTargetViewsDX[i] != nullptr)
            {
                m_HighestRenderTargetStage = i;
                setNewHigh = true;
            }
        }

        if (!setNewHigh)
        {
            m_HighestRenderTargetStage = 0;
        }
    }
}

void GraphicDevice::SetDepthStencil(DepthStencilSurface* depthStencilSurface)
{
    AEAssert(m_IsReady);

    ///////////////////////////////////////
    //Set Depth Stencil
    if(depthStencilSurface == nullptr)
    {
        m_CurrentDepthStencilViewDX = nullptr;
    }
    else
    {
        m_CurrentDepthStencilViewDX = depthStencilSurface->GetDXDepthStencilView();
    }

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);
}

void GraphicDevice::SetRenderTargetsAndDepthStencil(uint32_t numRTs, RenderTarget* rts[], DepthStencilSurface* depthStencilSurface)
{
    AEAssert(m_IsReady);

    AEAssert(rts != nullptr);
    if (rts == nullptr)
    {
        return;
    }

    ///////////////////////////////////////
    //Verify that it does not past max num of render target
    AEAssert(numRTs <= m_MaxNumRenderTargets)
    if(numRTs > m_MaxNumRenderTargets)
    {
        return;
    }

    ///////////////////////////////////////
    //Clear Render Targets
    for (uint32_t i = 0; i < m_MaxNumRenderTargets; ++i)
    {
        m_CurrentRenderTargetViewsDX[i] = nullptr;
    }

    ///////////////////////////////////////
    //Set Render Targets
    for(uint32_t i = 0; i < numRTs; i++)
    {
        if(rts[i] != nullptr)
        {
            m_CurrentRenderTargetViewsDX[i] = rts[i]->GetDXRenderTargetView();
        }
        else
        {
            m_CurrentRenderTargetViewsDX[i] = nullptr;
        }
    }

    ///////////////////////////////////////
    //Set Highest Render Target Stage
    m_HighestRenderTargetStage = numRTs;

    ///////////////////////////////////////
    //Set Depth Stencil
    if(depthStencilSurface == nullptr)
    {
        m_CurrentDepthStencilViewDX = nullptr;
    }
    {
        m_CurrentDepthStencilViewDX = depthStencilSurface->GetDXDepthStencilView();
    }

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);
}

void GraphicDevice::SetRenderTargets(uint32_t numRTs, uint32_t idxs[], Texture2DArray* rtArray)
{
    AEAssert(m_IsReady);

    AEAssert(rtArray != nullptr && idxs != nullptr);
    if (rtArray == nullptr || idxs == nullptr)
    {
        return;
    }

    AEAssert(rtArray->GetTextureBindOption() == TextureBindOption::RenderTarget);
    if (rtArray->GetTextureBindOption() != TextureBindOption::RenderTarget)
    {
        return;
    }

    AEAssert(numRTs <= rtArray->GetSize())
    if (numRTs > rtArray->GetSize())
    {
        return;
    }

    ///////////////////////////////////////
    //Verify that it does not past max num of render target
    AEAssert(numRTs <= m_MaxNumRenderTargets)
    if (numRTs > m_MaxNumRenderTargets)
    {
        return;
    }

    ///////////////////////////////////////
    //Clear Render Targets
    for (uint32_t i = 0; i < m_MaxNumRenderTargets; ++i)
    {
        m_CurrentRenderTargetViewsDX[i] = nullptr;
    }

    ///////////////////////////////////////
    //Set Render Targets
    for (uint32_t i = 0; i < numRTs; i++)
    {
        m_CurrentRenderTargetViewsDX[i] = rtArray->GetRenderTargetDX(idxs[i]);
    }

    ///////////////////////////////////////
    //Set Highest Render Target Stage
    m_HighestRenderTargetStage = numRTs;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);
}

void GraphicDevice::ResetRenderTarget()
{
    AEAssert(m_IsReady);

    ///////////////////////////////////////
    //Clear Render Targets
    for (uint32_t i = 0; i < m_MaxNumRenderTargets; ++i)
    {
        m_CurrentRenderTargetViewsDX[i] = nullptr;
    }

    ///////////////////////////////////////
    //Set Default Render Targets
    m_CurrentRenderTargetViewsDX[0] = m_GameRenderTargetViewDX;

    ///////////////////////////////////////
    //Set Highest Render Target Stage 
    m_HighestRenderTargetStage = 1;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);
}

void GraphicDevice::ResetDepthStencil()
{
    AEAssert(m_IsReady);

    ///////////////////////////////////////
    //Set Default Depth Stencil
    m_CurrentDepthStencilViewDX = m_GameDepthStencilViewDX;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);
}

void GraphicDevice::ResetRenderTargetAndSetDepthStencil()
{
    AEAssert(m_IsReady);

    ///////////////////////////////////////
    //Clear Render Targets
    for (uint32_t i = 0; i < m_MaxNumRenderTargets; ++i)
    {
        m_CurrentRenderTargetViewsDX[i] = nullptr;
    }

    ///////////////////////////////////////
    //Set Default Render Targets
    m_CurrentRenderTargetViewsDX[0] = m_GameRenderTargetViewDX;

    ///////////////////////////////////////
    //Set Highest Render Target Stage 
    m_HighestRenderTargetStage = 1;

    ///////////////////////////////////////
    //Set Default Depth Stencil
    m_CurrentDepthStencilViewDX = m_GameDepthStencilViewDX;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);
}

void GraphicDevice::SetViewport(Viewport* viewport)
{
    AEAssert(m_IsReady);

    const D3D11_VIEWPORT viewportDX[1] = { viewport->GetViewportDX() };

    m_DeviceContextDX->RSSetViewports(1, viewportDX);
}

void GraphicDevice::SetViewport(uint32_t viewportNums, Viewport* viewports[])
{
    AEAssert(m_IsReady);

    AETODO("Check where to set max available viewports");
    AEAssert(viewportNums < 8);
    if (viewportNums >= 8)
    {
        return;
    }

    D3D11_VIEWPORT viewportsDX[8] = { 0 };

    for (uint32_t i = 0; i < viewportNums; ++i)
    {
        viewportsDX[i] = viewports[i]->GetViewportDX();
    }

    m_DeviceContextDX->RSSetViewports(viewportNums, viewportsDX);
}

void GraphicDevice::ResetViewport()
{
    AEAssert(m_IsReady);

#ifdef AE_EDITOR_MODE
    m_DeviceContextDX->RSSetViewports(1, &m_GameViewportDX);
#else
    m_DeviceContextDX->RSSetViewports(1, &m_EditorViewportDX);
#endif
}

void GraphicDevice::SetIndexBuffer(IndexBuffer* ib, uint32_t offset)
{
    AEAssert(m_IsReady);
    
    AEAssert(ib != nullptr);
    if(ib == nullptr)
    {
        return;
    }

    if(!ib->IsReady())
    {
        AEResult ret = ib->BuildIndexBuffer();

        if(ret != AEResult::Ok)
        {
            return;
        }
    }

    DXGI_FORMAT dxFormat = DXGI_FORMAT_R16_UINT;
    if(ib->GetIndexBufferType() == IndexBufferType::Index32)
    {
        dxFormat = DXGI_FORMAT_R32_UINT;
    }
    //Else it is 16

    m_DeviceContextDX->IASetIndexBuffer(ib->GetDXBuffer(), dxFormat, offset);
}

void GraphicDevice::SetVertexBuffer(IVertexBuffer* vb, uint32_t stream, uint32_t offset)
{
    AEAssert(m_IsReady);

    AEAssert(vb != nullptr);
    if(vb == nullptr)
    {
        return;
    }

    if(!vb->IsReady())
    {
        AEResult ret = vb->BuildVertexBuffer();

        AEAssert(ret == AEResult::Ok);
        if(ret != AEResult::Ok)
        {
            return;
        }
    }

    const VertexLayout* vtxLayout = vb->GetVertexLayout();

    AEAssert(vtxLayout != nullptr);
    if(vtxLayout == nullptr)
    {
        return;
    }

    AEAssert(vtxLayout->IsReady());
    if(!vtxLayout->IsReady())
    {
        return;
    }

    m_DeviceContextDX->IASetInputLayout(vtxLayout->GetDXLayout());

    uint32_t vbSize     = vb->VertexSize();
    ID3D11Buffer* vbDx  = vb->GetDXBuffer();

    m_DeviceContextDX->IASetVertexBuffers(stream, 1, &vbDx, &vbSize, &offset);
}

void GraphicDevice::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitive)
{
    AEAssert(m_IsReady);

    m_DeviceContextDX->IASetPrimitiveTopology(primitive);
}

void GraphicDevice::Draw(uint32_t vertexCount, uint32_t startVertex)
{
    AEAssert(m_IsReady);

    m_DeviceContextDX->Draw(vertexCount, startVertex);
}

void GraphicDevice::DrawIndexed(uint32_t startVertex, uint32_t startIndex, uint32_t indexCount)
{
    AEAssert(m_IsReady);

    m_DeviceContextDX->DrawIndexed(indexCount, startIndex, startVertex);
}

void GraphicDevice::DrawFullScreenQuad(const glm::vec4& texCoord)
{
    AEAssert(m_IsReady);

    RECT size = { 0, 0, (LONG)m_GraphicPP.m_GameBackBufferWidth, (LONG)m_GraphicPP.m_GameBackBufferHeight };

    DrawQuad2D(size, texCoord);
}

void GraphicDevice::DrawQuad2D(const RECT& size, const glm::vec4& texCoord)
{
    AEAssert(m_IsReady);

    AEAssert(m_QuadShape2D != nullptr);
    if (m_QuadShape2D == nullptr)
    {
        return;
    }

    m_QuadShape2D->Resize(size, texCoord);

    m_QuadShape2D->DrawQuad();
}

void GraphicDevice::DispatchComputeShader(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ)
{
    AEAssert(m_IsReady);

    m_DeviceContextDX->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

void GraphicDevice::SetShader(Shader* shader)
{
    AEAssert(m_IsReady);

    switch (shader->GetShaderType())
    {
        case ShaderType::VertexShader:
            SetVertexShader(reinterpret_cast<VertexShader*>(shader));
            break;

        case ShaderType::PixelShader:
            SetPixelShader(reinterpret_cast<PixelShader*>(shader));
            break;

        case ShaderType::GeometryShader:
            SetGeometryShader(reinterpret_cast<GeometryShader*>(shader));
            break;

        case ShaderType::HullShader:
            SetHullShader(reinterpret_cast<HullShader*>(shader));
            break;

        case ShaderType::ComputeShader:
            SetComputeShader(reinterpret_cast<ComputeShader*>(shader));
            break;

        case ShaderType::DomainShader:
            SetDomainShader(reinterpret_cast<DomainShader*>(shader));
            break;

        default:
            AEAssert(false);
            break;
    }
}

void GraphicDevice::SetVertexShader(VertexShader* vs)
{
    AEAssert(m_IsReady);

    ID3D11VertexShader* vsDX = nullptr;

    if(vs != nullptr)
    {
        vsDX = vs->GetVertexShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->VSSetShader(vsDX, nullptr, 0);
}

void GraphicDevice::SetPixelShader(PixelShader* ps)
{
    AEAssert(m_IsReady);

    ID3D11PixelShader* psDX = nullptr;

    if(ps != nullptr)
    {
        psDX = ps->GetPixelShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->PSSetShader(psDX, nullptr, 0);
}

void GraphicDevice::SetGeometryShader(GeometryShader* gs)
{
    AEAssert(m_IsReady);

    ID3D11GeometryShader* gsDX = nullptr;

    if(gs != nullptr)
    {
        gsDX = gs->GetGeometryShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->GSSetShader(gsDX, nullptr, 0);
}

void GraphicDevice::SetHullShader(HullShader* hs)
{
    AEAssert(m_IsReady);

    ID3D11HullShader* hsDX = nullptr;

    if(hs != nullptr)
    {
        hsDX = hs->GetHullShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->HSSetShader(hsDX, nullptr, 0);
}

void GraphicDevice::SetDomainShader(DomainShader* ds)
{
    AEAssert(m_IsReady);

    ID3D11DomainShader* dsDX = nullptr;

    if(ds != nullptr)
    {
        dsDX = ds->GetDomainShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->DSSetShader(dsDX, nullptr, 0);
}

void GraphicDevice::SetComputeShader(ComputeShader* cs)
{
    AEAssert(m_IsReady);

    ID3D11ComputeShader* csDX = nullptr;

    if(cs != nullptr)
    {
        csDX = cs->GetComputeShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->CSSetShader(csDX, nullptr, 0);
}

void GraphicDevice::SetTexture(ShaderType type, uint32_t index, Texture* texture)
{
    AEAssert(m_IsReady);

    ID3D11ShaderResourceView* textureSRV = nullptr;

    if(texture != nullptr)
    {
        textureSRV = texture->GetDXShaderResourceView();
    }

    SetShaderResourceView(type, index, textureSRV);
}

void GraphicDevice::SetTextureArray(ShaderType type, uint32_t index, TextureArray* textureArray)
{
    AEAssert(m_IsReady);

    ID3D11ShaderResourceView* textureSRV = nullptr;

    if (textureArray != nullptr)
    {
        textureSRV = textureArray->GetDXTextureArraySRV();
    }

    SetShaderResourceView(type, index, textureSRV);
}

void GraphicDevice::SetSampler(ShaderType type, Sampler* sampler, bool overrideBindIndex, uint32_t newIndex)
{
    AEAssert(m_IsReady);

    ID3D11SamplerState* dxSamplers[1] = { nullptr };
    uint32_t bindIndex = 0;

    if (sampler == nullptr || overrideBindIndex)
    {
        bindIndex = newIndex;
    }
    else
    {
        bindIndex = sampler->GetBindIndex();
        dxSamplers[0] = sampler->GetSamplerDX();
    }

    switch (type)
    {
        case ShaderType::VertexShader:
            m_DeviceContextDX->VSSetSamplers(bindIndex, 1, dxSamplers);
            break;
        case ShaderType::PixelShader:
            m_DeviceContextDX->PSSetSamplers(bindIndex, 1, dxSamplers);
            break;
        case ShaderType::GeometryShader:
            m_DeviceContextDX->GSSetSamplers(bindIndex, 1, dxSamplers);
            break;
        case ShaderType::HullShader:
            m_DeviceContextDX->HSSetSamplers(bindIndex, 1, dxSamplers);
            break;
        case ShaderType::ComputeShader:
            m_DeviceContextDX->CSSetSamplers(bindIndex, 1, dxSamplers);
            break;
        case ShaderType::DomainShader:
            m_DeviceContextDX->DSSetSamplers(bindIndex, 1, dxSamplers);
            break;
        default:
            AEAssert(false);
            break;
    }
}

void GraphicDevice::SetShaderResourceView(ShaderType type, uint32_t index, ID3D11ShaderResourceView* srv)
{
    AEAssert(m_IsReady);

    ID3D11ShaderResourceView* arrDX[] = { srv };

    SetShaderResourceViews(type, index, 1, arrDX);
}

void GraphicDevice::SetShaderResourceViews(ShaderType type, uint32_t index, uint32_t numView, ID3D11ShaderResourceView** srv)
{
    AEAssert(m_IsReady);

    AEAssert(srv != nullptr);
    if(srv == nullptr)
    {
        return;
    }

    switch (type)
    {
        case ShaderType::VertexShader:
            m_DeviceContextDX->VSSetShaderResources(index, numView, srv);
            break;
        case ShaderType::PixelShader:
            m_DeviceContextDX->PSSetShaderResources(index, numView, srv);
            break;
        case ShaderType::GeometryShader:
            m_DeviceContextDX->GSSetShaderResources(index, numView, srv);
            break;
        case ShaderType::HullShader:
            m_DeviceContextDX->HSSetShaderResources(index, numView, srv);
            break;
        case ShaderType::ComputeShader:
            m_DeviceContextDX->CSSetShaderResources(index, numView, srv);
            break;
        case ShaderType::DomainShader:
            m_DeviceContextDX->DSSetShaderResources(index, numView, srv);
            break;
        default:
            AEAssert(false);
            break;
    }
}

void GraphicDevice::SetShaderBuffer(ShaderType type, ShaderBuffer* shaderBuffer, bool overrideBindIndex, uint32_t newIndex)
{
    AEAssert(m_IsReady);

    ID3D11ShaderResourceView* bufferSRV[1] = { nullptr };
    uint32_t bindIndex = 0;
    bool isRW = false;

    if(shaderBuffer == nullptr)
    {
        bindIndex = newIndex;
    }
    else
    {
        /////////////////////////////////////////////////
        //Get SRV from Buffer
        bufferSRV[0] = shaderBuffer->GetBufferSRVDX();

        /////////////////////////////////////////////////
        //Get Bind Index and see if it is overwritten
        bindIndex = shaderBuffer->GetBindIndex();

        if(overrideBindIndex)
        {
            bindIndex = newIndex;
        }
    }

    SetShaderResourceViews(type, bindIndex, 1, bufferSRV);
}

void GraphicDevice::SetShaderRWBufferToCS(ShaderBuffer* shaderBuffer, bool overrideBindIndex, uint32_t newIndex)
{
    AEAssert(m_IsReady);

    ID3D11UnorderedAccessView* bufferUAV[1] = { nullptr };
    uint32_t bindIndex = 0;

    if(shaderBuffer == nullptr)
    {
        bindIndex = newIndex;
    }
    else
    {
        /////////////////////////////////////////////////
        //Get UAV from Buffer
        bufferUAV[0] = shaderBuffer->GetBufferUAVDX();

        /////////////////////////////////////////////////
        //Get Bind Index and see if it is overwritten
        bindIndex = shaderBuffer->GetBindIndex();

        if(overrideBindIndex)
        {
            bindIndex = newIndex;
        }
    }

    m_DeviceContextDX->CSSetUnorderedAccessViews(bindIndex, 1, bufferUAV, nullptr);
}

void GraphicDevice::SetConstantBuffer(ShaderType type, ConstantBuffer* cb, bool overrideBindIndex, uint32_t newIndex)
{
    AEAssert(m_IsReady);

    ID3D11Buffer* arrDX[1] = { nullptr };
    uint32_t bindIndex = 0;

    if(cb == nullptr)
    {
        arrDX[0] = nullptr;
        bindIndex = newIndex;
    }
    else
    {
        arrDX[0] = cb->GetConstantBufferDX();

        if(overrideBindIndex)
        {
            bindIndex = newIndex;
        }
        else
        {
            bindIndex = cb->GetBindIndex();
        }
    }

    switch (type)
    {
        case ShaderType::VertexShader:
            m_DeviceContextDX->VSSetConstantBuffers(bindIndex, 1, arrDX);
            break;
        case ShaderType::PixelShader:
            m_DeviceContextDX->PSSetConstantBuffers(bindIndex, 1, arrDX);
            break;
        case ShaderType::GeometryShader:
            m_DeviceContextDX->GSSetConstantBuffers(bindIndex, 1, arrDX);
            break;
        case ShaderType::HullShader:
            m_DeviceContextDX->HSSetConstantBuffers(bindIndex, 1, arrDX);
            break;
        case ShaderType::ComputeShader:
            m_DeviceContextDX->CSSetConstantBuffers(bindIndex, 1, arrDX);
            break;
        case ShaderType::DomainShader:
            m_DeviceContextDX->DSSetConstantBuffers(bindIndex, 1, arrDX);
            break;
        default:
            AEAssert(false);
            break;
    }
}

void GraphicDevice::SetSamplerState(ShaderType type, uint32_t index, ID3D11SamplerState* sampler)
{
    AEAssert(m_IsReady);
    
    AEAssert(sampler != nullptr);
    if(sampler == nullptr)
    {
        return;
    }

    ID3D11SamplerState* arrDX[] = { sampler };

    SetSamplerStates(type, index, 1, arrDX);
}
    
void GraphicDevice::SetSamplerStates(ShaderType type, uint32_t index, uint32_t numStates, ID3D11SamplerState** sampler)
{
    AEAssert(m_IsReady);
    
    AEAssert(sampler != nullptr);
    if(sampler == nullptr)
    {
        return;
    }

    switch (type)
    {
        case ShaderType::VertexShader:
            m_DeviceContextDX->VSSetSamplers(index, numStates, sampler);
            break;
        case ShaderType::PixelShader:
            m_DeviceContextDX->PSSetSamplers(index, numStates, sampler);
            break;
        case ShaderType::GeometryShader:
            m_DeviceContextDX->GSSetSamplers(index, numStates, sampler);
            break;
        case ShaderType::HullShader:
            m_DeviceContextDX->HSSetSamplers(index, numStates, sampler);
            break;
        case ShaderType::ComputeShader:
            m_DeviceContextDX->CSSetSamplers(index, numStates, sampler);
            break;
        case ShaderType::DomainShader:
            m_DeviceContextDX->DSSetSamplers(index, numStates, sampler);
            break;
        default:
            AEAssert(false);
            break;
    }
}

void GraphicDevice::GetBlendState(ID3D11BlendState** blendState, glm::vec4& blendFactor, uint32_t& sampleMask)
{
    AEAssert(m_IsReady);

    AEAssert(false);
    AETODO("Implement");
    AETODO("Get adds a ref");

    //AEAssert(blendState != nullptr);
    //if (blendState == nullptr)
    //{
    //    return AEResult::NullParameter;
    //}

    //AETODO("Check that this works correctly");
    //FLOAT* tempBlendFactor = reinterpret_cast<FLOAT*>(&blendFactor);

    //m_DeviceContextDX->OMGetBlendState(blendState, tempBlendFactor, &sampleMask);
}

void GraphicDevice::SetBlendState(ID3D11BlendState* blendState, const glm::vec4& blendFactor, uint32_t sampleMask)
{
    AEAssert(m_IsReady);

    AETODO("Check that this works correctly");
    const FLOAT* tempBlendFactor = reinterpret_cast<const FLOAT*>(&blendFactor);

    m_DeviceContextDX->OMSetBlendState(blendState, tempBlendFactor, sampleMask); 
}

#if defined(AE_GRAPHIC_DEBUG_DEVICE)
void GraphicDevice::BeginEvent(const std::string& eventName)
{
    AEAssert(m_IsReady);
    AEAssert(m_UserDefinedAnnotationDX != nullptr);

    std::wstring eventNameW = AE_Base::String2WideStr(eventName);
    if (m_UserDefinedAnnotationDX->BeginEvent(eventNameW.c_str()) != 0)
    {
        AETODO("Log Message");
    }
}

void GraphicDevice::EndEvent()
{
    AEAssert(m_IsReady);
    AEAssert(m_UserDefinedAnnotationDX != nullptr);

    if (m_UserDefinedAnnotationDX->EndEvent() != 0)
    {
        AETODO("Log Message");
    }
}

void GraphicDevice::SetEventmarker(const std::string& eventName)
{
    AEAssert(m_IsReady);
    AEAssert(m_UserDefinedAnnotationDX != nullptr);

    std::wstring eventNameW = AE_Base::String2WideStr(eventName);
    m_UserDefinedAnnotationDX->SetMarker(eventNameW.c_str());
}
#endif // defined(AE_GRAPHIC_DEBUG_DEVICE)

#ifdef AE_EDITOR_MODE

void GraphicDevice::ResetGameRenderTargetView()
{
    CleanUpGameDepthStencil();
    CleanUpGameRenderTargets();

    InitGameRTDS();

    InitGameViewport();
}

void GraphicDevice::InitEditorViewport()
{
    AETODO("Check if we need this, ImGui might have its own viewport");
    m_EditorViewportDX.TopLeftX = 0;
    m_EditorViewportDX.TopLeftY = 0;
    m_EditorViewportDX.Width = static_cast<float>(m_GraphicPP.m_EditorBackBufferWidth);
    m_EditorViewportDX.Height = static_cast<float>(m_GraphicPP.m_EditorBackBufferHeight);
    m_EditorViewportDX.MinDepth = 0.0f;
    m_EditorViewportDX.MaxDepth = 1.0f;

    m_DeviceContextDX->RSSetViewports(1, &m_EditorViewportDX);
}

AEResult GraphicDevice::CreateEditorRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;

    HRESULT hr = m_SwapChainDX->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (hr != S_OK)
    {
        DisplayError(hr);
        return AEResult::Fail;
    }

    hr = m_DeviceDX->CreateRenderTargetView(backBuffer, 0, &m_EditorRenderTargetViewDX);
    ReleaseCOM(backBuffer);
    if (hr != S_OK)
    {
        DisplayError(hr);
        return AEResult::Fail;
    }
    AEGraphicHelpers::SetDebugObjectName(m_EditorRenderTargetViewDX, AE_DEBUG_EDITOR_RTV_NAME);

    return AEResult::Ok;
}

void GraphicDevice::ResizeEditor(uint32_t width, uint32_t height)
{
    AEAssert(m_IsReady);

    m_GraphicPP.m_EditorBackBufferWidth   = width;
    m_GraphicPP.m_EditorBackBufferHeight  = height;

    ResetDevice();
}

void GraphicDevice::SetEditorRenderTargetAndViewPort()
{
    AEAssert(m_IsReady);

    m_HighestRenderTargetStage = 1;
    m_CurrentRenderTargetViewsDX[0] = m_EditorRenderTargetViewDX;
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, nullptr);
    m_DeviceContextDX->RSSetViewports(1, &m_EditorViewportDX);
}

#endif