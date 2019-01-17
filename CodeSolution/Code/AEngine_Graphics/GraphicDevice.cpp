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

    ZeroMemory(&m_ScreenViewportDX, sizeof(D3D11_VIEWPORT));

    m_gPP.m_DeviceWindow =  hMainWnd;

    m_gPP.m_SingleThreaded = graphicOpts.m_SingleThreaded;

    if(graphicOpts.m_FullScreen)
    {
        m_gPP.m_BackBufferWidth = graphicOpts.m_ScreenResolution.x;
        m_gPP.m_BackBufferHeight = graphicOpts.m_ScreenResolution.y;

        m_gPP.m_BackBufferFormatFullScreen = AEGraphicHelpers::GetPXFormatFromString(graphicOpts.m_BackBufferFormatFullScreen);

        m_gPP.m_Windowed = false;
    }
    else 
    {
        glm::ivec2 wndSize = AEGraphicHelpers::GetWindowSize(hMainWnd);
        
        m_gPP.m_BackBufferWidth = wndSize.x;
        m_gPP.m_BackBufferHeight = wndSize.y;
        
        m_gPP.m_BackBufferFormatWindowed = AEGraphicHelpers::GetPXFormatFromString(graphicOpts.m_BackBufferFormatWindowed);

        m_gPP.m_Windowed = true;
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

    ReleaseCOM(m_DefaultRenderTargetViewDX);
    ReleaseCOM(m_DefaultDepthStencilViewDX);
    ReleaseCOM(m_SwapChainDX);
    ReleaseCOM(m_DefaultDepthStencilBufferDX);

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

AEResult GraphicDevice::ReleaseDefaultTextures()
{
    AERelease(m_DefaultTexture2D);

    return AEResult::Ok;
}

void GraphicDevice::ResetHalfPixel()
{
    m_HalfPixel.x = (0.5f / (float)m_gPP.m_BackBufferWidth);
    m_HalfPixel.y = (0.5f / (float)m_gPP.m_BackBufferHeight);
}

AEResult GraphicDevice::OnLostDevice()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    //First all Related to the Graphic Device
    ReleaseDefaultRTDS();

    return AEResult::Ok;
}

AEResult GraphicDevice::OnResetDevice()
{    
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    //First all Related to the Graphic Device
    CreateDefaultRTDS();

    //Second The Rest
    ResetHalfPixel();
    
    return AEResult::Ok;
}

AEResult GraphicDevice::DrawFullScreenQuad(const glm::vec4& texCoord)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    RECT size = { 0, 0, (LONG)m_gPP.m_BackBufferWidth, (LONG)m_gPP.m_BackBufferHeight };

    return DrawQuad2D(size, texCoord);
}

AEResult GraphicDevice::DrawQuad2D(const RECT& size, const glm::vec4& texCoord)
{
    AEAssert(m_QuadShape2D != nullptr);

    if(!m_IsReady || m_QuadShape2D == nullptr)
    {
        return AEResult::NotReady;
    }

    m_QuadShape2D->Resize(size, texCoord);

    return m_QuadShape2D->DrawQuad();
}

AEResult GraphicDevice::CheckDevCaps(const std::string& file)
{
    AEAssert(!file.empty());

    GraphicsCheckDevCaps devCaps;

    AEXMLParser newFile;
    if (newFile.LoadFile(file) != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Graphics, "INIT_COULDNT_READ_FILE_MSG", __FUNCTION__, file);
        return AEResult::Fail;
    }

    AEXMLParser devCapsXML = newFile["DevCaps"];
    if ( !devCapsXML.HasElement() )
    {
        return AEResult::Fail;
    }

    for (AEXMLParser child = devCapsXML.GetFirstChildElement(); child.HasElement(); child = child.GetNextSiblingElement())
    {
        std::string l_Type = child.GetName();

        if( l_Type.compare("PixelShader") == 0 )
        {
            devCaps.PS_V = child.GetVect2i("Ver", glm::ivec2(2, 0), false);
        }
        else if ( l_Type.compare("VertexShader") == 0 )
        {
            devCaps.VS_V = child.GetVect2i("Ver", glm::ivec2(2, 0), false);
        }
        else if( l_Type.compare("PureDevice") == 0 )
        {
            devCaps.PureDevice = true;
        }
        else if( l_Type.compare("HWTransformedLight") == 0 )
        {
            devCaps.HWTransformedLight = true;
        }
        else if( l_Type.compare("ScissorTest") == 0 )
        {
            devCaps.ScissorTest = true;
        }
        else if( l_Type.compare("PixelFormat") == 0 )
        {
            std::string displayFormat = child.GetString("DisplayFormat", "NOT_FOUND");
            std::string backFufferFormat = child.GetString("BackBufferFormat", "NOT_FOUND");
            bool windowed = child.GetBool("Windowed");

            GraphicsCheckFormat chFmt;

            chFmt.Windowed            = windowed;
            chFmt.BackBufferFormat    = AEGraphicHelpers::GetPXFormatFromString(backFufferFormat);
            chFmt.DisplayFormat        = AEGraphicHelpers::GetPXFormatFromString(displayFormat);

            devCaps.CheckFormatsVect.push_back(chFmt);
        }
        else if( l_Type.compare("MAXRTS") == 0 )
        {
            devCaps.MaxSimultaneousRTs = child.GetUInt("num", 4, false);
        }
    }

    return this->CheckDevCaps(devCaps);
}

AEResult GraphicDevice::InitDXConfiguration()
{
    if(InitSwapChainDesc() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::InitSwapChainDesc()
{
    ZeroMemory(&m_SwapChainDescDX, sizeof(DXGI_SWAP_CHAIN_DESC));
    
    //Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
    m_SwapChainDescDX.BufferDesc.Width                      = m_gPP.m_BackBufferWidth;
    m_SwapChainDescDX.BufferDesc.Height                     = m_gPP.m_BackBufferHeight;
    AETODO("Check into Scaling");
    m_SwapChainDescDX.BufferDesc.Scaling                    = DXGI_MODE_SCALING_UNSPECIFIED;
    m_SwapChainDescDX.BufferDesc.ScanlineOrdering           = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    AETODO("Check Numerator and Denominator for Refresh Rate");
    m_SwapChainDescDX.BufferDesc.RefreshRate.Numerator      = 60;
    m_SwapChainDescDX.BufferDesc.RefreshRate.Denominator    = 1;

    if(m_gPP.m_Windowed)
    {
        m_SwapChainDescDX.BufferDesc.Format = m_gPP.m_BackBufferFormatWindowed;
    }
    else
    {
        m_SwapChainDescDX.BufferDesc.Format = m_gPP.m_BackBufferFormatFullScreen;
    }

    m_SwapChainDescDX.OutputWindow          = m_gPP.m_DeviceWindow;
    m_SwapChainDescDX.Windowed              = m_gPP.m_Windowed;
    m_SwapChainDescDX.SampleDesc            = m_gPP.m_MultiSample;

    AETODO("Check into Usage Render Target Output");
    AETODO("Add option to Back Buffer");
    m_SwapChainDescDX.BufferCount           = m_gPP.m_BackBufferCount; // = 2;                               // Use double buffering to minimize latency.
    m_SwapChainDescDX.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    
    //DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Metro style apps must use this SwapEffect.
    m_SwapChainDescDX.SwapEffect            = m_gPP.m_SwapEffect;
    
    AETODO("Check into Swap Chain Flags");
    m_SwapChainDescDX.Flags                 = 0;
    
    return AEResult::Ok;
}

AEResult GraphicDevice::CreateSwapChain()
{
    // To correctly create the swap chain, we must use the IDXGIFactory that was
    // used to create the device.  If we tried to use a different IDXGIFactory instance
    // (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
    // This function is being called with a device from a different IDXGIFactory."
    IDXGIDevice* dxgiDevice = nullptr;
    IDXGIAdapter* dxgiAdapter = nullptr;
    IDXGIFactory* dxgiFactory = nullptr;
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

    AEGraphicHelpers::SetDebugObjectName<IDXGISwapChain>(m_SwapChainDX, AE_DEBUG_MAIN_SC_NAME);

    //AETODO("Check into this");
    //// Ensure that DXGI does not queue more than one frame at a time. This both reduces
    //// latency and ensures that the application will only render after each VSync, minimizing
    //// power consumption.
    //dxgiDevice->SetMaximumFrameLatency(1);
    
    ReleaseCOM(dxgiFactory);
    ReleaseCOM(dxgiAdapter);
    ReleaseCOM(dxgiDevice);

    return AEResult::Ok;
}

AEResult GraphicDevice::InitViewport()
{
    // Set the viewport Transform.
    m_ScreenViewportDX.TopLeftX = 0;
    m_ScreenViewportDX.TopLeftY = 0;
    m_ScreenViewportDX.Width    = static_cast<float>(m_gPP.m_BackBufferWidth);
    m_ScreenViewportDX.Height   = static_cast<float>(m_gPP.m_BackBufferHeight);
    m_ScreenViewportDX.MinDepth = 0.0f;
    m_ScreenViewportDX.MaxDepth = 1.0f;

    m_DeviceContextDX->RSSetViewports(1, &m_ScreenViewportDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::CreateDefaultRTDS()
{    
    if(CreateDepthStencil() == AEResult::Fail)
    {
        return AEResult::Fail;
    }

    if(CreateRenderTarget() == AEResult::Fail)
    {
        ReleaseCOM(m_DefaultDepthStencilBufferDX);
        ReleaseCOM(m_DefaultDepthStencilViewDX);

        return AEResult::Fail;
    }

    //////////////////////////////////////////////////////////////////////////////////
    //Bind the render target view and depth/stencil view to the pipeline.
    memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);
    m_CurrentRenderTargetViewsDX[0] = m_DefaultRenderTargetViewDX;
    m_HighestRenderTargetStage = 1;

    m_CurrentDepthStencilViewDX = m_DefaultDepthStencilViewDX;

    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };

    depthStencilDesc.Format             = m_gPP.m_DepthStencilFormat;

    depthStencilDesc.Width              = m_gPP.m_BackBufferWidth;
    depthStencilDesc.Height             = m_gPP.m_BackBufferHeight;

    depthStencilDesc.MipLevels          = 1; //Depth Stencil for Default has 1 Mip Map
    depthStencilDesc.ArraySize          = 1; //Depth Stencil for Default has 1 Array Size
    
    depthStencilDesc.SampleDesc         = m_gPP.m_MultiSample;

    AETODO("Check Flags");
    depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags     = 0; 
    depthStencilDesc.MiscFlags          = 0;

    HRESULT hr = S_OK;

    hr = m_DeviceDX->CreateTexture2D(&depthStencilDesc, nullptr, &m_DefaultDepthStencilBufferDX);
    
    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_DefaultDepthStencilBufferDX, AE_DEBUG_MAIN_DST_NAME);
    
    AETODO("Check Depth Stencil View Desc");
    hr = m_DeviceDX->CreateDepthStencilView(m_DefaultDepthStencilBufferDX, 0, &m_DefaultDepthStencilViewDX);
    
    if(hr != S_OK)
    {
        DisplayError(hr);

        ReleaseCOM(m_DefaultDepthStencilBufferDX);

        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11DepthStencilView>(m_DefaultDepthStencilViewDX, AE_DEBUG_MAIN_DSV_NAME);

    return AEResult::Ok;
}

AEResult GraphicDevice::CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;

    HRESULT hr = S_OK;

    hr = m_SwapChainDX->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    
    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    hr = m_DeviceDX->CreateRenderTargetView(backBuffer, 0, &m_DefaultRenderTargetViewDX);
    
    ReleaseCOM(backBuffer);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }
    
    AEGraphicHelpers::SetDebugObjectName<ID3D11RenderTargetView>(m_DefaultRenderTargetViewDX, AE_DEBUG_MAIN_RTV_NAME);

    return AEResult::Ok;
}

void GraphicDevice::ReleaseDefaultRTDS()
{
    ReleaseCOM(m_DefaultDepthStencilBufferDX);
    ReleaseCOM(m_DefaultDepthStencilViewDX);
    ReleaseCOM(m_DefaultRenderTargetViewDX);
}

AEResult GraphicDevice::ResetDevice()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    HRESULT hr = S_OK;

    //Do anything that needs to be done with graphic device when it is lost
    if(OnLostDevice() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    AETODO("Check to see reset failures to Device");

    //Resize Swap Chain
    AETODO("Check to see if we are in full screen or window");
    AETODO("Check Swap Chain Flags");
    hr = m_SwapChainDX->ResizeBuffers(1, m_gPP.m_BackBufferWidth, m_gPP.m_BackBufferHeight, m_gPP.m_BackBufferFormatFullScreen, 0);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    if(InitViewport() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    //Do Anything that needs to be done when Graphic Device is reset
    if(OnResetDevice() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::Resize(uint32_t width, uint32_t height)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    m_gPP.m_BackBufferWidth = width;
    m_gPP.m_BackBufferHeight = height;

    return ResetDevice();
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

    if(m_gPP.m_SingleThreaded)
    {
        createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
    }

    AETODO("Check Feature Levels");
    D3D_FEATURE_LEVEL featureLevelsArr[] = {
        D3D_FEATURE_LEVEL_11_1,  
        D3D_FEATURE_LEVEL_11_0
    };
    const uint32_t numFeatureLeves = _countof(featureLevelsArr);

    D3D_FEATURE_LEVEL featureLevel;

    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
    
    AETODO("Give option to default adapter");
    AETODO("Give option to Driver Type");
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
    AEGraphicHelpers::SetDebugObjectName<ID3D11Device>(m_DeviceDX, AE_DEBUG_MAIN_GD_NAME);
    AEGraphicHelpers::SetDebugObjectName<ID3D11DeviceContext>(m_DeviceContextDX, AE_DEBUG_MAIN_DC_NAME);

    AETODO("CHECK Device1 and other stuff, it is only available in windows 8");
    //hr = device->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_DX3D11Device);
    //if(hr != S_OK)
    //{
    //    ReleaseCOM(device);
    //    ReleaseCOM(deviceContext);

    //    DisplayError(hr);

    //    return AEResult::Fail;
    //}
    //
    //ReleaseCOM(device);

    //hr = deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_DX3DImmediateContext);
    //if(hr != S_OK)
    //{
    //    ReleaseCOM(m_DX3D11Device);
    //    ReleaseCOM(deviceContext);

    //    DisplayError(hr);

    //    return AEResult::Fail;
    //}
    
    //ReleaseCOM(deviceContext);

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

    //Create Default Render Targets
    if(CreateDefaultRTDS() != AEResult::Ok)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AELogHelpers::Log(LogLevel::Error, LogSystem::Graphics, "DX_11_FAIL_RENDER_TARGETS_ERR_MSG", __FUNCTION__);

        return AEResult::CreateDXDefaultRTDSFail;
    }

    //Create Viewport
    if(InitViewport() != AEResult::Ok)
    {
        ReleaseCOM(m_DeviceDX);
        ReleaseCOM(m_DeviceContextDX);

        AELogHelpers::Log(LogLevel::Error, LogSystem::Graphics, "DX_11_FAIL_VIEWPORT_ERR_MSG", __FUNCTION__);

        return AEResult::InitViewportFail;
    }

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

AEResult GraphicDevice::Clear(bool target, uint32_t rt_stage_id, bool depth, bool stencil, const Color& color, float depthVal, uint8_t stencilVal)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if(target)
    {
        AEAssert(rt_stage_id < m_MaxNumRenderTargets);
        if(rt_stage_id >= m_MaxNumRenderTargets)
        {
            AETODO("Add better return error and log");
            return AEResult::OutsideRange;
        }

        AEAssert(m_CurrentRenderTargetViewsDX[rt_stage_id] != nullptr);
        if(m_CurrentRenderTargetViewsDX[rt_stage_id] == nullptr)
        {
            AETODO("Add better return error and log");
            return AEResult::NullObj;
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

    return AEResult::Ok;
}

AEResult GraphicDevice::Present()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

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

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::CheckDevCaps(const GraphicsCheckDevCaps& devCaps)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(!"Not implemented");

    return AEResult::Ok;
}

AEResult GraphicDevice::SetRenderTarget(uint32_t stage_id, RenderTarget* rt)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ///////////////////////////////////////
    //Verify that it does not past max num of render target
    if(stage_id >= m_MaxNumRenderTargets)
    {
        AETODO("Set better fail error");
        return AEResult::OutsideRange;
    }

    ///////////////////////////////////////
    //Set Render Target
    if(rt == nullptr)
    {
        m_CurrentRenderTargetViewsDX[stage_id] = nullptr;
    }
    else
    {
        m_CurrentRenderTargetViewsDX[stage_id] = rt->GetDXRenderTargetView();

        if(m_CurrentRenderTargetViewsDX[stage_id] == nullptr)
        {
            AETODO("Log warning");
        }
    }

    ///////////////////////////////////////
    //Set Highest Render Target Stage
    m_HighestRenderTargetStage = ( (stage_id + 1) > m_HighestRenderTargetStage) ? (stage_id + 1) : m_HighestRenderTargetStage;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

    ///////////////////////////////////////
    //Set Highest Render Target Stage if
    //the last render target was null
    if(m_CurrentRenderTargetViewsDX[stage_id] == nullptr && stage_id == m_HighestRenderTargetStage)
    {
        bool setNewHigh = false;

        for (uint32_t i = stage_id; i >= 0 && !setNewHigh; i--)
        {
            if(m_CurrentRenderTargetViewsDX[i] != nullptr)
            {
                m_HighestRenderTargetStage = i;
                setNewHigh = true;
            }
        }

        if(!setNewHigh)
        {
            m_HighestRenderTargetStage = 0;
        }
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::SetDepthStencil(DepthStencilSurface* depthStencilSurface)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

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

    return AEResult::Ok;
}

AEResult GraphicDevice::SetRenderTargetsAndDepthStencil(uint32_t numRTs, RenderTarget* rts[], DepthStencilSurface* depthStencilSurface)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(rts != nullptr);
    if (rts == nullptr)
    {
        return AEResult::NullParameter;
    }

    ///////////////////////////////////////
    //Verify that it does not past max num of render target
    if(numRTs >= m_MaxNumRenderTargets)
    {
        AETODO("Set better fail error");
        return AEResult::OutsideRange;
    }

    ///////////////////////////////////////
    //Clear Render Targets
    memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);

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

    return AEResult::Ok;
}

AEResult GraphicDevice::SetRenderTargets(uint32_t numRTs, uint32_t idxs[], Texture2DArray* rtArray)
{
    ///////////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(rtArray != nullptr);
    AEAssert(idxs != nullptr);
    if (rtArray == nullptr || idxs == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(rtArray->GetTextureBindOption() == TextureBindOption::RenderTarget);
    if (rtArray->GetTextureBindOption() != TextureBindOption::RenderTarget)
    {
        return AEResult::InvalidObjType;
    }

    AEAssert(numRTs <= rtArray->GetSize())
    if (numRTs > rtArray->GetSize())
    {
        return AEResult::OutsideRange;
    }

    ///////////////////////////////////////
    //Verify that it does not past max num of render target
    if (numRTs >= m_MaxNumRenderTargets)
    {
        AETODO("Set better fail error");
        return AEResult::OutsideRange;
    }

    ///////////////////////////////////////
    //Clear Render Targets
    memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*)* m_MaxNumRenderTargets);

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

    return AEResult::Ok;
}

AEResult GraphicDevice::ResetRenderTarget()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ///////////////////////////////////////
    //Clear Render Targets
    memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);

    ///////////////////////////////////////
    //Set Default Render Targets
    m_CurrentRenderTargetViewsDX[0] = m_DefaultRenderTargetViewDX;

    ///////////////////////////////////////
    //Set Highest Render Target Stage 
    m_HighestRenderTargetStage = 1;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::ResetDepthStencil()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ///////////////////////////////////////
    //Set Default Depth Stencil
    m_CurrentDepthStencilViewDX = m_DefaultDepthStencilViewDX;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::ResetRenderTargetAndSetDepthStencil()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ///////////////////////////////////////
    //Clear Render Targets
    memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);

    ///////////////////////////////////////
    //Set Default Render Targets
    m_CurrentRenderTargetViewsDX[0] = m_DefaultRenderTargetViewDX;

    ///////////////////////////////////////
    //Set Highest Render Target Stage 
    m_HighestRenderTargetStage = 1;

    ///////////////////////////////////////
    //Set Default Depth Stencil
    m_CurrentDepthStencilViewDX = m_DefaultDepthStencilViewDX;

    ///////////////////////////////////////
    //Set Render Targets and Depth Stencil
    m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetViewport(Viewport* viewport)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    const D3D11_VIEWPORT viewportDX[1] = { viewport->GetViewportDX() };

    m_DeviceContextDX->RSSetViewports(1, viewportDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetViewport(uint32_t viewportNums, Viewport* viewports[])
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (viewportNums >= 8)
    {
        return AEResult::OutsideRange;
    }

    AETODO("Check where to set max available viewports");
    D3D11_VIEWPORT viewportsDX[8] = { 0 };

    for (uint32_t i = 0; i < viewportNums; i++)
    {
        viewportsDX[i] = viewports[i]->GetViewportDX();
    }

    m_DeviceContextDX->RSSetViewports(viewportNums, viewportsDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::ResetViewport()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    m_DeviceContextDX->RSSetViewports(1, &m_ScreenViewportDX);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetIndexBuffer(IndexBuffer* ib, uint32_t offset)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }
    
    AEAssert(ib != nullptr);

    if(ib == nullptr)
    {
        return AEResult::NullParameter;
    }

    if(!ib->IsReady())
    {
        AEResult ret = ib->BuildIndexBuffer();

        if(ret != AEResult::Ok)
        {
            return AEResult::Fail;
        }
    }

    DXGI_FORMAT dxFormat = DXGI_FORMAT_R16_UINT;

    if(ib->GetIndexBufferType() == IndexBufferType::Index32)
    {
        dxFormat = DXGI_FORMAT_R32_UINT;
    }
    //Else it is 16

    m_DeviceContextDX->IASetIndexBuffer(ib->GetDXBuffer(), dxFormat, offset);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetVertexBuffer(IVertexBuffer* vb, uint32_t stream, uint32_t offset)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }
    
    AEAssert(vb != nullptr);

    if(vb == nullptr)
    {
        return AEResult::NullParameter;
    }

    if(!vb->IsReady())
    {
        AEResult ret = vb->BuildVertexBuffer();

        if(ret != AEResult::Ok)
        {
            return AEResult::Fail;
        }
    }

    const VertexLayout* vtxLayout = vb->GetVertexLayout();

    if(vtxLayout == nullptr)
    {
        return AEResult::Fail;
    }

    if(!vtxLayout->IsReady())
    {
        return AEResult::Fail;
    }

    m_DeviceContextDX->IASetInputLayout(vtxLayout->GetDXLayout());

    uint32_t vbSize = vb->VertexSize();
    ID3D11Buffer* vbDx = vb->GetDXBuffer();

    m_DeviceContextDX->IASetVertexBuffers(stream, 1, &vbDx, &vbSize, &offset);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitive)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    m_DeviceContextDX->IASetPrimitiveTopology(primitive);

    return AEResult::Ok;
}

AEResult GraphicDevice::Draw(uint32_t vertexCount, uint32_t startVertex)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    m_DeviceContextDX->Draw(vertexCount, startVertex);

    return AEResult::Ok;
}

AEResult GraphicDevice::DrawIndexed(uint32_t startVertex, uint32_t startIndex, uint32_t indexCount)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    m_DeviceContextDX->DrawIndexed(indexCount, startIndex, startVertex);

    return AEResult::Ok;
}

AEResult GraphicDevice::DispatchComputeShader(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    m_DeviceContextDX->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetShader(Shader* shader)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    switch (shader->GetShaderType())
    {
        case ShaderType::VertexShader:
            return SetVertexShader(reinterpret_cast<VertexShader*>(shader));
        case ShaderType::PixelShader:
            return SetPixelShader(reinterpret_cast<PixelShader*>(shader));
        case ShaderType::GeometryShader:
            return SetGeometryShader(reinterpret_cast<GeometryShader*>(shader));
        case ShaderType::HullShader:
            return SetHullShader(reinterpret_cast<HullShader*>(shader));
        case ShaderType::ComputeShader:
            return SetComputeShader(reinterpret_cast<ComputeShader*>(shader));
        case ShaderType::DomainShader:
            return SetDomainShader(reinterpret_cast<DomainShader*>(shader));
        default:
            return AEResult::InvalidShaderType;
    }
}

AEResult GraphicDevice::SetVertexShader(VertexShader* vs)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11VertexShader* vsDX = nullptr;

    if(vs != nullptr)
    {
        vsDX = vs->GetVertexShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->VSSetShader(vsDX, nullptr, 0);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetPixelShader(PixelShader* ps)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11PixelShader* psDX = nullptr;

    if(ps != nullptr)
    {
        psDX = ps->GetPixelShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->PSSetShader(psDX, nullptr, 0);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetGeometryShader(GeometryShader* gs)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11GeometryShader* gsDX = nullptr;

    if(gs != nullptr)
    {
        gsDX = gs->GetGeometryShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->GSSetShader(gsDX, nullptr, 0);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetHullShader(HullShader* hs)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11HullShader* hsDX = nullptr;

    if(hs != nullptr)
    {
        hsDX = hs->GetHullShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->HSSetShader(hsDX, nullptr, 0);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetDomainShader(DomainShader* ds)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11DomainShader* dsDX = nullptr;

    if(ds != nullptr)
    {
        dsDX = ds->GetDomainShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->DSSetShader(dsDX, nullptr, 0);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetComputeShader(ComputeShader* cs)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11ComputeShader* csDX = nullptr;

    if(cs != nullptr)
    {
        csDX = cs->GetComputeShaderDX();
    }

    AETODO("Check Effect instances");
    m_DeviceContextDX->CSSetShader(csDX, nullptr, 0);

    return AEResult::Ok;
}

AEResult GraphicDevice::SetTexture(ShaderType type, uint32_t index, Texture* texture)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11ShaderResourceView* textureSRV = nullptr;

    if(texture != nullptr)
    {
        textureSRV = texture->GetDXShaderResourceView();
    }

    return SetShaderResourceView(type, index, textureSRV);
}

AEResult GraphicDevice::SetTextureArray(ShaderType type, uint32_t index, TextureArray* textureArray)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11ShaderResourceView* textureSRV = nullptr;

    if (textureArray != nullptr)
    {
        textureSRV = textureArray->GetDXTextureArraySRV();
    }

    return SetShaderResourceView(type, index, textureSRV);
}

AEResult GraphicDevice::SetSampler(ShaderType type, Sampler* sampler, bool overrideBindIndex, uint32_t newIndex)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

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
            return AEResult::InvalidShaderType;
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::SetShaderResourceView(ShaderType type, uint32_t index, ID3D11ShaderResourceView* srv)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ID3D11ShaderResourceView* arrDX[] = { srv };

    return SetShaderResourceViews(type, index, 1, arrDX);
}

AEResult GraphicDevice::SetShaderResourceViews(ShaderType type, uint32_t index, uint32_t numView, ID3D11ShaderResourceView** srv)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(srv != nullptr);
    if(srv == nullptr)
    {
        return AEResult::NullParameter;
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
            AETODO("Add log here");

            return AEResult::InvalidShaderType;

            break;
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::SetShaderBuffer(ShaderType type, ShaderBuffer* shaderBuffer, bool overrideBindIndex, uint32_t newIndex)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

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

    return this->SetShaderResourceViews(type, bindIndex, 1, bufferSRV);
}

AEResult GraphicDevice::SetShaderRWBufferToCS(ShaderBuffer* shaderBuffer, bool overrideBindIndex, uint32_t newIndex)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

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

    return AEResult::Ok;
}

AEResult GraphicDevice::SetConstantBuffer(ShaderType type, ConstantBuffer* cb, bool overrideBindIndex, uint32_t newIndex)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

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
            AETODO("Add log here");

            return AEResult::InvalidShaderType;

            break;
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::SetSamplerState(ShaderType type, uint32_t index, ID3D11SamplerState* sampler)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }
    
    AEAssert(sampler != nullptr);

    if(sampler == nullptr)
    {
        return AEResult::NullParameter;
    }

    ID3D11SamplerState* arrDX[] = { sampler };

    return SetSamplerStates(type, index, 1, arrDX);
}
    
AEResult GraphicDevice::SetSamplerStates(ShaderType type, uint32_t index, uint32_t numStates, ID3D11SamplerState** sampler)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }
    
    AEAssert(sampler != nullptr);

    if(sampler == nullptr)
    {
        return AEResult::NullParameter;
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
            AETODO("Add log here");

            return AEResult::InvalidShaderType;

            break;
    }

    return AEResult::Ok;
}

AEResult GraphicDevice::GetBlendState(ID3D11BlendState** blendState, glm::vec4& blendFactor, uint32_t& sampleMask)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

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

    return AEResult::Fail;
}

AEResult GraphicDevice::SetBlendState(ID3D11BlendState* blendState, const glm::vec4& blendFactor, uint32_t sampleMask)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AETODO("Check that this works correctly");
    const FLOAT* tempBlendFactor = reinterpret_cast<const FLOAT*>(&blendFactor);

    m_DeviceContextDX->OMSetBlendState(blendState, tempBlendFactor, sampleMask); 

    return AEResult::Ok;
}

#if defined(AE_GRAPHIC_DEBUG_DEVICE)
void GraphicDevice::BeginEvent(const std::string& eventName)
{
    if (!m_IsReady || m_UserDefinedAnnotationDX == nullptr)
    {
        return;
    }

    std::wstring eventNameW = AE_Base::String2WideStr(eventName);
    if (m_UserDefinedAnnotationDX->BeginEvent(eventNameW.c_str()) != 0)
    {
        AETODO("Log Message");
    }
}

void GraphicDevice::EndEvent()
{
    if (!m_IsReady || m_UserDefinedAnnotationDX == nullptr)
    {
        return;
    }

    if (m_UserDefinedAnnotationDX->EndEvent() != 0)
    {
        AETODO("Log Message");
    }
}

void GraphicDevice::SetEventmarker(const std::string& eventName)
{
    if (!m_IsReady || m_UserDefinedAnnotationDX == nullptr)
    {
        return;
    }

    std::wstring eventNameW = AE_Base::String2WideStr(eventName);
    m_UserDefinedAnnotationDX->SetMarker(eventNameW.c_str());
}
#endif // defined(AE_GRAPHIC_DEBUG_DEVICE)