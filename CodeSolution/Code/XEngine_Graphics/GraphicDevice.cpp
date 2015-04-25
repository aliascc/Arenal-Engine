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
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Utils\Viewport.h"
#include "Models\MeshPart.h"
#include "XML\XEXMLParser.h"
#include "Shaders\ShaderDefs.h"
#include "Base\BaseFunctions.h"
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
#include "Localization\LocalizationManager.h"
#include "Shaders\Variables\Texture2DArray.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GraphicDevice::GraphicDevice(HWND hMainWnd, const GraphicOptsPreferred& graphicOpts)
{
	XEAssert(hMainWnd != nullptr);

	ZeroMemory(&m_ScreenViewportDX, sizeof(D3D11_VIEWPORT));

	m_gPP.m_DeviceWindow =  hMainWnd;

	m_gPP.m_SingleThreaded = graphicOpts.m_SingleThreaded;

	if(graphicOpts.m_FullScreen)
	{
		m_gPP.m_BackBufferWidth = graphicOpts.m_ScreenResolution.x;
		m_gPP.m_BackBufferHeight = graphicOpts.m_ScreenResolution.y;

		m_gPP.m_BackBufferFormatFullScreen = XEGraphicHelpers::GetPXFormatFromString(graphicOpts.m_BackBufferFormatFullScreen);

		m_gPP.m_Windowed = false;
	}
	else 
	{
		glm::ivec2 wndSize = XEGraphicHelpers::GetWindowSize(hMainWnd);
		
		m_gPP.m_BackBufferWidth = wndSize.x;
		m_gPP.m_BackBufferHeight = wndSize.y;
		
		m_gPP.m_BackBufferFormatWindowed = XEGraphicHelpers::GetPXFormatFromString(graphicOpts.m_BackBufferFormatWindowed);

		m_gPP.m_Windowed = true;
	}

	m_QuadShape2D = new QuadShape2D(this, true);
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

#if defined(_DEBUG) || defined(PROFILE)
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
	VertexPosition::GetVertexLayout(this);
	VertexPositionColor::GetVertexLayout(this);
	VertexPositionNormal::GetVertexLayout(this);
	VertexPositionTexture::GetVertexLayout(this);
	VertexPositionNormalColor::GetVertexLayout(this);
	VertexPositionNormalTexture::GetVertexLayout(this);
	VertexPositionNormalTextureTexture2::GetVertexLayout(this);
	VertexPositionNormalTextureIndicesWeight::GetVertexLayout(this);
	VertexPositionNormalTangentBinormalTexture::GetVertexLayout(this);
	VertexPositionNormalTangentBinormalTextureTexture2::GetVertexLayout(this);
	VertexPositionNormalTangentBinormalTextureIndicesWeight::GetVertexLayout(this);
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

XEResult GraphicDevice::CreateDefaultTextures()
{
	if (m_DefaultTexture2D != nullptr)
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	m_DefaultTexture2D = new Texture2D(this, XE_DEFAULT_TEX_2D_NAME);

	ret = m_DefaultTexture2D->CreateColorTexture(128, 128, XEColors::Fuchsia);
	if (ret != XEResult::Ok)
	{
		XERelease(m_DefaultTexture2D);

		return ret;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::ReleaseDefaultTextures()
{
	XERelease(m_DefaultTexture2D);

	return XEResult::Ok;
}

void GraphicDevice::ResetHalfPixel()
{
	m_HalfPixel.x = (0.5f / (float)m_gPP.m_BackBufferWidth);
	m_HalfPixel.y = (0.5f / (float)m_gPP.m_BackBufferHeight);
}

XEResult GraphicDevice::OnLostDevice()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	//First all Related to the Graphic Device
	ReleaseDefaultRTDS();

	return XEResult::Ok;
}

XEResult GraphicDevice::OnResetDevice()
{	
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	//First all Related to the Graphic Device
	CreateDefaultRTDS();

	//Second The Rest
	ResetHalfPixel();
	
	return XEResult::Ok;
}

XEResult GraphicDevice::DrawFullScreenQuad(const glm::vec4& texCoord)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	RECT size = { 0, 0, m_gPP.m_BackBufferWidth, m_gPP.m_BackBufferHeight };

	return DrawQuad2D(size, texCoord);
}

XEResult GraphicDevice::DrawQuad2D(const RECT& size, const glm::vec4& texCoord)
{
	XEAssert(m_QuadShape2D != nullptr);

	if(!m_IsReady || m_QuadShape2D == nullptr)
	{
		return XEResult::NotReady;
	}

	m_QuadShape2D->Resize(size, texCoord);

	return m_QuadShape2D->DrawQuad();
}

XEResult GraphicDevice::CheckDevCaps(const std::wstring& file)
{
	XEAssert(!file.empty());

	GraphicsCheckDevCaps devCaps;

	XEXMLParser newFile;
	if (newFile.LoadFile(file) != XEResult::Ok)
	{
		std::wstring msg_error = L"";
		fastformat::fmt(msg_error, XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, file);
		
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::Fail;
	}

	XEXMLParser devCapsXML = newFile[L"DevCaps"];
	if ( !devCapsXML.IsReady() )
	{
		return XEResult::Fail;
	}

	uint16_t l_Count = devCapsXML.GetNumChildren();

	for( uint16_t i = 0; i < l_Count; ++i )
	{
		XEXMLParser child = devCapsXML(i);

		std::wstring l_Type = child.GetName();

		if( l_Type.compare(L"PixelShader") == 0 )
		{
			devCaps.PS_V = child.GetVect2i(L"Ver", glm::ivec2(2, 0), false);
		}
		else if ( l_Type.compare(L"VertexShader") == 0 )
		{
			devCaps.VS_V = child.GetVect2i(L"Ver", glm::ivec2(2, 0), false);
		}
		else if( l_Type.compare(L"PureDevice") == 0 )
		{
			devCaps.PureDevice = true;
		}
		else if( l_Type.compare(L"HWTransformedLight") == 0 )
		{
			devCaps.HWTransformedLight = true;
		}
		else if( l_Type.compare(L"ScissorTest") == 0 )
		{
			devCaps.ScissorTest = true;
		}
		else if( l_Type.compare(L"PixelFormat") == 0 )
		{
			std::wstring displayFormat = child.GetString(L"DisplayFormat", L"NOT_FOUND");
			std::wstring backFufferFormat = child.GetString(L"BackBufferFormat", L"NOT_FOUND");
			bool windowed = child.GetBool(L"Windowed");

			GraphicsCheckFormat chFmt;

			chFmt.Windowed			= windowed;
			chFmt.BackBufferFormat	= XEGraphicHelpers::GetPXFormatFromString(backFufferFormat);
			chFmt.DisplayFormat		= XEGraphicHelpers::GetPXFormatFromString(displayFormat);

			devCaps.CheckFormatsVect.push_back(chFmt);
		}
		else if( l_Type.compare(L"MAXRTS") == 0 )
		{
			devCaps.MaxSimultaneousRTs = child.GetUInt(L"num", 4, false);
		}
	}

	return this->CheckDevCaps(devCaps);
}

XEResult GraphicDevice::InitDXConfiguration()
{
	if(InitSwapChainDesc() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::InitSwapChainDesc()
{
	ZeroMemory(&m_SwapChainDescDX, sizeof(DXGI_SWAP_CHAIN_DESC));
	
	//Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	m_SwapChainDescDX.BufferDesc.Width						= m_gPP.m_BackBufferWidth;
	m_SwapChainDescDX.BufferDesc.Height						= m_gPP.m_BackBufferHeight;
	XETODO("Check into Scaling");
	m_SwapChainDescDX.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	m_SwapChainDescDX.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	XETODO("Check Numerator and Denominator for Refresh Rate");
	m_SwapChainDescDX.BufferDesc.RefreshRate.Numerator		= 60;
	m_SwapChainDescDX.BufferDesc.RefreshRate.Denominator	= 1;

	if(m_gPP.m_Windowed)
	{
		m_SwapChainDescDX.BufferDesc.Format = m_gPP.m_BackBufferFormatWindowed;
	}
	else
	{
		m_SwapChainDescDX.BufferDesc.Format = m_gPP.m_BackBufferFormatFullScreen;
	}

	m_SwapChainDescDX.OutputWindow		= m_gPP.m_DeviceWindow;
	m_SwapChainDescDX.Windowed			= m_gPP.m_Windowed;
	m_SwapChainDescDX.SampleDesc		= m_gPP.m_MultiSample;

	XETODO("Check into Usage Render Target Output");
	m_SwapChainDescDX.BufferCount		= m_gPP.m_BackBufferCount; // = 2;                               // Use double buffering to minimize latency.
	m_SwapChainDescDX.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	//DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Metro style apps must use this SwapEffect.
	m_SwapChainDescDX.SwapEffect		= m_gPP.m_SwapEffect;
	
	XETODO("Check into Swap Chain Flags");
	m_SwapChainDescDX.Flags				= 0;
	
	return XEResult::Ok;
}

XEResult GraphicDevice::CreateSwapChain()
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

		return XEResult::Fail;
	}

	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

	if(hr != S_OK)
	{
		DisplayError(hr);

		ReleaseCOM(dxgiDevice);

		return XEResult::Fail;
	}

	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

	if(hr != S_OK)
	{
		DisplayError(hr);
		
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiDevice);

		return XEResult::Fail;
	}

	hr = dxgiFactory->CreateSwapChain(m_DeviceDX, &m_SwapChainDescDX, &m_SwapChainDX);
	
	if(hr != S_OK)
	{
		DisplayError(hr);
		
		ReleaseCOM(dxgiFactory);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiDevice);

		return XEResult::Fail;
	}

	XEGraphicHelpers::SetDebugObjectName<IDXGISwapChain>(m_SwapChainDX, XE_DEBUG_MAIN_SC_NAME);

	//XETODO("Check into this");
	//// Ensure that DXGI does not queue more than one frame at a time. This both reduces
	//// latency and ensures that the application will only render after each VSync, minimizing
	//// power consumption.
	//dxgiDevice->SetMaximumFrameLatency(1);
	
	ReleaseCOM(dxgiFactory);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiDevice);

	return XEResult::Ok;
}

XEResult GraphicDevice::InitViewport()
{
	// Set the viewport Transform.
	m_ScreenViewportDX.TopLeftX = 0;
	m_ScreenViewportDX.TopLeftY = 0;
	m_ScreenViewportDX.Width    = static_cast<float>(m_gPP.m_BackBufferWidth);
	m_ScreenViewportDX.Height   = static_cast<float>(m_gPP.m_BackBufferHeight);
	m_ScreenViewportDX.MinDepth = 0.0f;
	m_ScreenViewportDX.MaxDepth = 1.0f;

	m_DeviceContextDX->RSSetViewports(1, &m_ScreenViewportDX);

	return XEResult::Ok;
}

XEResult GraphicDevice::CreateDefaultRTDS()
{	
	if(CreateDepthStencil() == XEResult::Fail)
	{
		return XEResult::Fail;
	}

	if(CreateRenderTarget() == XEResult::Fail)
	{
		ReleaseCOM(m_DefaultDepthStencilBufferDX);
		ReleaseCOM(m_DefaultDepthStencilViewDX);

		return XEResult::Fail;
	}

	//////////////////////////////////////////////////////////////////////////////////
	//Bind the render target view and depth/stencil view to the pipeline.
	memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);
	m_CurrentRenderTargetViewsDX[0] = m_DefaultRenderTargetViewDX;
	m_HighestRenderTargetStage = 1;

	m_CurrentDepthStencilViewDX = m_DefaultDepthStencilViewDX;

	m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

	return XEResult::Ok;
}

XEResult GraphicDevice::CreateDepthStencil()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };

	depthStencilDesc.Format			= m_gPP.m_DepthStencilFormat;

	depthStencilDesc.Width			= m_gPP.m_BackBufferWidth;
	depthStencilDesc.Height			= m_gPP.m_BackBufferHeight;

	depthStencilDesc.MipLevels		= 1; //Depth Stencil for Default has 1 Mip Map
	depthStencilDesc.ArraySize		= 1; //Depth Stencil for Default has 1 Array Size
	
	depthStencilDesc.SampleDesc		= m_gPP.m_MultiSample;

	XETODO("Check Flags");
	depthStencilDesc.Usage			= D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags	= 0; 
	depthStencilDesc.MiscFlags		= 0;

	HRESULT hr = S_OK;

	hr = m_DeviceDX->CreateTexture2D(&depthStencilDesc, nullptr, &m_DefaultDepthStencilBufferDX);
	
	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::Fail;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_DefaultDepthStencilBufferDX, XE_DEBUG_MAIN_DST_NAME);
	
	XETODO("Check Depth Stencil View Desc");
	hr = m_DeviceDX->CreateDepthStencilView(m_DefaultDepthStencilBufferDX, 0, &m_DefaultDepthStencilViewDX);
	
	if(hr != S_OK)
	{
		DisplayError(hr);

		ReleaseCOM(m_DefaultDepthStencilBufferDX);

		return XEResult::Fail;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11DepthStencilView>(m_DefaultDepthStencilViewDX, XE_DEBUG_MAIN_DSV_NAME);

	return XEResult::Ok;
}

XEResult GraphicDevice::CreateRenderTarget()
{
	ID3D11Texture2D* backBuffer = nullptr;

	HRESULT hr = S_OK;

	hr = m_SwapChainDX->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	
	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::Fail;
	}

	hr = m_DeviceDX->CreateRenderTargetView(backBuffer, 0, &m_DefaultRenderTargetViewDX);
	
	ReleaseCOM(backBuffer);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::Fail;
	}
	
	XEGraphicHelpers::SetDebugObjectName<ID3D11RenderTargetView>(m_DefaultRenderTargetViewDX, XE_DEBUG_MAIN_RTV_NAME);

	return XEResult::Ok;
}

void GraphicDevice::ReleaseDefaultRTDS()
{
	ReleaseCOM(m_DefaultDepthStencilBufferDX);
	ReleaseCOM(m_DefaultDepthStencilViewDX);
	ReleaseCOM(m_DefaultRenderTargetViewDX);
}

XEResult GraphicDevice::ResetDevice()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	HRESULT hr = S_OK;

	//Do anything that needs to be done with graphic device when it is lost
	if(OnLostDevice() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	XETODO("Check to see reset failures to Device");

	//Resize Swap Chain
	XETODO("Check to see if we are in full screen or window");
	XETODO("Check Swap Chain Flags");
	hr = m_SwapChainDX->ResizeBuffers(1, m_gPP.m_BackBufferWidth, m_gPP.m_BackBufferHeight, m_gPP.m_BackBufferFormatFullScreen, 0);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::Fail;
	}

	if(InitViewport() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	//Do Anything that needs to be done when Graphic Device is reset
	if(OnResetDevice() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::Resize(uint32_t width, uint32_t height)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	m_gPP.m_BackBufferWidth = width;
	m_gPP.m_BackBufferHeight = height;

	return ResetDevice();
}

XEResult GraphicDevice::InitDevice()
{
	//If device has already been initialize return ok
	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	if(XELOGGER == nullptr || XELOCMAN == nullptr)
	{
		return XEResult::NullObj;
	}

	//Initialize DirectX Configuration with XE_Presentation Parameter
	if(InitDXConfiguration() != XEResult::Ok)
	{
		return XEResult::CreateDXConfFail;
	}

	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if(m_gPP.m_SingleThreaded)
	{
		createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
	}

	XETODO("Check Feature Levels");
	D3D_FEATURE_LEVEL featureLevelsArr[] = {
		D3D_FEATURE_LEVEL_11_1,  
		D3D_FEATURE_LEVEL_11_0
	};
	const uint32_t numFeatureLeves = _countof(featureLevelsArr);

	D3D_FEATURE_LEVEL featureLevel;

	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	
	XETODO("Give option to default adapter");
	XETODO("Give option to Driver Type");
	hr = D3D11CreateDevice(
		nullptr,					//Default adapter
		driverType,					//Driver Type
		nullptr,					//No software device
		createDeviceFlags,			//Device Create Flags
		featureLevelsArr,			//Feature Levels
		numFeatureLeves,			//Count of feature levels
		D3D11_SDK_VERSION,			//DirectX Version 11
		&m_DeviceDX,				//DirectX Device
		&featureLevel,				//Feature Level
		&m_DeviceContextDX);		//Device Immediate Context	

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::CreateDXDeviceFail;
	}

	//Add names to DX Objects for Debugging
	XEGraphicHelpers::SetDebugObjectName<ID3D11Device>(m_DeviceDX, XE_DEBUG_MAIN_GD_NAME);
	XEGraphicHelpers::SetDebugObjectName<ID3D11DeviceContext>(m_DeviceContextDX, XE_DEBUG_MAIN_DC_NAME);

	XETODO("CHECK Device1 and other stuff, it is only available in windows 8");
	//hr = device->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_DX3D11Device);
	//if(hr != S_OK)
	//{
	//	ReleaseCOM(device);
	//	ReleaseCOM(deviceContext);

	//	DisplayError(hr);

	//	return XEResult::Fail;
	//}
	//
	//ReleaseCOM(device);

	//hr = deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_DX3DImmediateContext);
	//if(hr != S_OK)
	//{
	//	ReleaseCOM(m_DX3D11Device);
	//	ReleaseCOM(deviceContext);

	//	DisplayError(hr);

	//	return XEResult::Fail;
	//}
	
	//ReleaseCOM(deviceContext);

	if(featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		ReleaseCOM(m_DeviceDX);
		ReleaseCOM(m_DeviceContextDX);

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"DX_11_UNSUPPORTED_ERR_MSG"), __FUNCTIONW__);
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return XEResult::FeatureLvlUnsupported;
	}

	//Init Pre Graphic Objects
	if(InitPreGraphicObjects() != XEResult::Ok)
	{
		ReleaseCOM(m_DeviceDX);
		ReleaseCOM(m_DeviceContextDX);

		XETODO("Log Error");

		XETODO("Return error");
		return XEResult::Fail;
	}

	//Create Swap Chain
	if(CreateSwapChain() != XEResult::Ok)
	{
		ReleaseCOM(m_DeviceDX);
		ReleaseCOM(m_DeviceContextDX);

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"DX_11_FAIL_SWAP_CHAIN_ERR_MSG"), __FUNCTIONW__);
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return XEResult::CreateDXSwapChainFail;
	}

	//Create Default Render Targets
	if(CreateDefaultRTDS() != XEResult::Ok)
	{
		ReleaseCOM(m_DeviceDX);
		ReleaseCOM(m_DeviceContextDX);

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"DX_11_FAIL_RENDER_TARGETS_ERR_MSG"), __FUNCTIONW__);
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return XEResult::CreateDXDefaultRTDSFail;
	}

	//Create Viewport
	if(InitViewport() != XEResult::Ok)
	{
		ReleaseCOM(m_DeviceDX);
		ReleaseCOM(m_DeviceContextDX);

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"DX_11_FAIL_VIEWPORT_ERR_MSG"), __FUNCTIONW__);
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return XEResult::InitViewportFail;
	}

#if defined(_DEBUG) || defined(PROFILE)

	hr = m_DeviceContextDX->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void**>(&m_UserDefinedAnnotationDX));
	
	if(hr != S_OK)
	{
		DisplayError(hr);
		
		XETODO("Log error");
	}

#endif

	//Reset Half Pixel
	ResetHalfPixel();

	//Init Graphic Objects
	if(InitGraphicObjects() != XEResult::Ok)
	{
		ReleaseCOM(m_DeviceDX);
		ReleaseCOM(m_DeviceContextDX);

		XETODO("Log Error");

		XETODO("Return error");
		return XEResult::Fail;
	}

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult GraphicDevice::InitPreGraphicObjects()
{
	XETODO("Check what is the max of render targets and set");
	m_MaxNumRenderTargets = 4;

	m_CurrentRenderTargetViewsDX = new ID3D11RenderTargetView*[m_MaxNumRenderTargets];

	memset(m_CurrentRenderTargetViewsDX, 0, sizeof(ID3D11RenderTargetView*) * m_MaxNumRenderTargets);

	return XEResult::Ok;
}

XEResult GraphicDevice::InitGraphicObjects()
{
	InitBasicVertexTypes();

	XETODO("check return and Log error");
	CreateDefaultTextures();

	if(GraphicBlendStates::InitStates(this) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(m_QuadShape2D->BuildQuad() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::Clear(bool target, uint32_t rt_stage_id, bool depth, bool stencil, const Color& color, float depthVal, uint8_t stencilVal)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(target)
	{
		XEAssert(rt_stage_id < m_MaxNumRenderTargets);
		if(rt_stage_id >= m_MaxNumRenderTargets)
		{
			XETODO("Add better return error and log");
			return XEResult::OutsideRange;
		}

		XEAssert(m_CurrentRenderTargetViewsDX[rt_stage_id] != nullptr);
		if(m_CurrentRenderTargetViewsDX[rt_stage_id] == nullptr)
		{
			XETODO("Add better return error and log");
			return XEResult::NullObj;
		}

		XETODO("Back buffer color argb order check");
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

	return XEResult::Ok;
}

XEResult GraphicDevice::Present()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XETODO("Check Present1");
	//XETODO("CHECK DXGI_PRESENT_PARAMETERS");
 //   // The application may optionally specify "dirty" or "scroll" rects to improve efficiency
 //   // in certain scenarios.  In this sample, however, we do not utilize those features.
	//DXGI_PRESENT_PARAMETERS parameters;
 //   parameters.DirtyRectsCount = 0;
 //   parameters.pDirtyRects = nullptr;
 //   parameters.pScrollRect = nullptr;
 //   parameters.pScrollOffset = nullptr;
	
	XETODO("Correct Parameters pass to Present to make it more flexible");
	HRESULT hr = m_SwapChainDX->Present(0, 0);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::CheckDevCaps(const GraphicsCheckDevCaps& devCaps)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(!"Not implemented");

	return XEResult::Ok;
}

XEResult GraphicDevice::SetRenderTarget(uint32_t stage_id, RenderTarget* rt)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	///////////////////////////////////////
	//Verify that it does not past max num of render target
	if(stage_id >= m_MaxNumRenderTargets)
	{
		XETODO("Set better fail error");
		return XEResult::OutsideRange;
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
			XETODO("Log warning");
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

	return XEResult::Ok;
}

XEResult GraphicDevice::SetDepthStencil(DepthStencilSurface* depthStencilSurface)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
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

	return XEResult::Ok;
}

XEResult GraphicDevice::SetRenderTargetsAndDepthStencil(uint32_t numRTs, RenderTarget* rts[], DepthStencilSurface* depthStencilSurface)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(rts != nullptr);
	if (rts == nullptr)
	{
		return XEResult::NullParameter;
	}

	///////////////////////////////////////
	//Verify that it does not past max num of render target
	if(numRTs >= m_MaxNumRenderTargets)
	{
		XETODO("Set better fail error");
		return XEResult::OutsideRange;
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

	return XEResult::Ok;
}

XEResult GraphicDevice::SetRenderTargets(uint32_t numRTs, uint32_t idxs[], Texture2DArray* rtArray)
{
	///////////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(rtArray != nullptr);
	XEAssert(idxs != nullptr);
	if (rtArray == nullptr || idxs == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(rtArray->GetTextureBindOption() == TextureBindOption::RenderTarget);
	if (rtArray->GetTextureBindOption() != TextureBindOption::RenderTarget)
	{
		return XEResult::InvalidObjType;
	}

	XEAssert(numRTs <= rtArray->GetSize())
	if (numRTs > rtArray->GetSize())
	{
		return XEResult::OutsideRange;
	}

	///////////////////////////////////////
	//Verify that it does not past max num of render target
	if (numRTs >= m_MaxNumRenderTargets)
	{
		XETODO("Set better fail error");
		return XEResult::OutsideRange;
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

	return XEResult::Ok;
}

XEResult GraphicDevice::ResetRenderTarget()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
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

	return XEResult::Ok;
}

XEResult GraphicDevice::ResetDepthStencil()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	///////////////////////////////////////
	//Set Default Depth Stencil
	m_CurrentDepthStencilViewDX = m_DefaultDepthStencilViewDX;

	///////////////////////////////////////
	//Set Render Targets and Depth Stencil
	m_DeviceContextDX->OMSetRenderTargets(m_HighestRenderTargetStage, m_CurrentRenderTargetViewsDX, m_CurrentDepthStencilViewDX);

	return XEResult::Ok;
}

XEResult GraphicDevice::ResetRenderTargetAndSetDepthStencil()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
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

	return XEResult::Ok;
}

XEResult GraphicDevice::SetViewport(Viewport* viewport)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	const D3D11_VIEWPORT viewportDX[1] = { viewport->GetViewportDX() };

	m_DeviceContextDX->RSSetViewports(1, viewportDX);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetViewport(uint32_t viewportNums, Viewport* viewports[])
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (viewportNums >= 8)
	{
		return XEResult::OutsideRange;
	}

	XETODO("Check where to set max available viewports");
	D3D11_VIEWPORT viewportsDX[8] = { 0 };

	for (uint32_t i = 0; i < viewportNums; i++)
	{
		viewportsDX[i] = viewports[i]->GetViewportDX();
	}

	m_DeviceContextDX->RSSetViewports(viewportNums, viewportsDX);

	return XEResult::Ok;
}

XEResult GraphicDevice::ResetViewport()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	m_DeviceContextDX->RSSetViewports(1, &m_ScreenViewportDX);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetIndexBuffer(IndexBuffer* ib, uint32_t offset)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}
	
	XEAssert(ib != nullptr);

	if(ib == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(!ib->IsReady())
	{
		XEResult ret = ib->BuildIndexBuffer();

		if(ret != XEResult::Ok)
		{
			return XEResult::Fail;
		}
	}

	DXGI_FORMAT dxFormat = DXGI_FORMAT_R16_UINT;

	if(ib->GetIndexBufferType() == IndexBufferType::Index32)
	{
		dxFormat = DXGI_FORMAT_R32_UINT;
	}
	//Else it is 16

	m_DeviceContextDX->IASetIndexBuffer(ib->GetDXBuffer(), dxFormat, offset);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetVertexBuffer(IVertexBuffer* vb, uint32_t stream, uint32_t offset)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}
	
	XEAssert(vb != nullptr);

	if(vb == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(!vb->IsReady())
	{
		XEResult ret = vb->BuildVertexBuffer();

		if(ret != XEResult::Ok)
		{
			return XEResult::Fail;
		}
	}

	const VertexLayout* vtxLayout = vb->GetVertexLayout();

	if(vtxLayout == nullptr)
	{
		return XEResult::Fail;
	}

	if(!vtxLayout->IsReady())
	{
		return XEResult::Fail;
	}

	m_DeviceContextDX->IASetInputLayout(vtxLayout->GetDXLayout());

	uint32_t vbSize = vb->VertexSize();
	ID3D11Buffer* vbDx = vb->GetDXBuffer();

	m_DeviceContextDX->IASetVertexBuffers(stream, 1, &vbDx, &vbSize, &offset);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitive)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	m_DeviceContextDX->IASetPrimitiveTopology(primitive);

	return XEResult::Ok;
}

XEResult GraphicDevice::Draw(uint32_t vertexCount, uint32_t startVertex)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	m_DeviceContextDX->Draw(vertexCount, startVertex);

	return XEResult::Ok;
}

XEResult GraphicDevice::DrawIndexed(uint32_t startVertex, uint32_t startIndex, uint32_t indexCount)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	m_DeviceContextDX->DrawIndexed(indexCount, startIndex, startVertex);

	return XEResult::Ok;
}

XEResult GraphicDevice::DispatchComputeShader(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	m_DeviceContextDX->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetShader(Shader* shader)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
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
			return XEResult::InvalidShaderType;
	}
}

XEResult GraphicDevice::SetVertexShader(VertexShader* vs)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11VertexShader* vsDX = nullptr;

	if(vs != nullptr)
	{
		vsDX = vs->GetVertexShaderDX();
	}

	XETODO("Check Effect instances");
	m_DeviceContextDX->VSSetShader(vsDX, nullptr, 0);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetPixelShader(PixelShader* ps)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11PixelShader* psDX = nullptr;

	if(ps != nullptr)
	{
		psDX = ps->GetPixelShaderDX();
	}

	XETODO("Check Effect instances");
	m_DeviceContextDX->PSSetShader(psDX, nullptr, 0);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetGeometryShader(GeometryShader* gs)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11GeometryShader* gsDX = nullptr;

	if(gs != nullptr)
	{
		gsDX = gs->GetGeometryShaderDX();
	}

	XETODO("Check Effect instances");
	m_DeviceContextDX->GSSetShader(gsDX, nullptr, 0);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetHullShader(HullShader* hs)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11HullShader* hsDX = nullptr;

	if(hs != nullptr)
	{
		hsDX = hs->GetHullShaderDX();
	}

	XETODO("Check Effect instances");
	m_DeviceContextDX->HSSetShader(hsDX, nullptr, 0);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetDomainShader(DomainShader* ds)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11DomainShader* dsDX = nullptr;

	if(ds != nullptr)
	{
		dsDX = ds->GetDomainShaderDX();
	}

	XETODO("Check Effect instances");
	m_DeviceContextDX->DSSetShader(dsDX, nullptr, 0);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetComputeShader(ComputeShader* cs)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11ComputeShader* csDX = nullptr;

	if(cs != nullptr)
	{
		csDX = cs->GetComputeShaderDX();
	}

	XETODO("Check Effect instances");
	m_DeviceContextDX->CSSetShader(csDX, nullptr, 0);

	return XEResult::Ok;
}

XEResult GraphicDevice::SetTexture(ShaderType type, uint32_t index, Texture* texture)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11ShaderResourceView* textureSRV = nullptr;

	if(texture != nullptr)
	{
		textureSRV = texture->GetDXShaderResourceView();
	}

	return SetShaderResourceView(type, index, textureSRV);
}

XEResult GraphicDevice::SetTextureArray(ShaderType type, uint32_t index, TextureArray* textureArray)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11ShaderResourceView* textureSRV = nullptr;

	if (textureArray != nullptr)
	{
		textureSRV = textureArray->GetDXTextureArraySRV();
	}

	return SetShaderResourceView(type, index, textureSRV);
}

XEResult GraphicDevice::SetSampler(ShaderType type, Sampler* sampler, bool overrideBindIndex, uint32_t newIndex)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
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
			return XEResult::InvalidShaderType;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::SetShaderResourceView(ShaderType type, uint32_t index, ID3D11ShaderResourceView* srv)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	ID3D11ShaderResourceView* arrDX[] = { srv };

	return SetShaderResourceViews(type, index, 1, arrDX);
}

XEResult GraphicDevice::SetShaderResourceViews(ShaderType type, uint32_t index, uint32_t numView, ID3D11ShaderResourceView** srv)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(srv != nullptr);
	if(srv == nullptr)
	{
		return XEResult::NullParameter;
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
			XETODO("Add log here");

			return XEResult::InvalidShaderType;

			break;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::SetShaderBuffer(ShaderType type, ShaderBuffer* shaderBuffer, bool overrideBindIndex, uint32_t newIndex)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
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

XEResult GraphicDevice::SetShaderRWBufferToCS(ShaderBuffer* shaderBuffer, bool overrideBindIndex, uint32_t newIndex)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
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

	return XEResult::Ok;
}

XEResult GraphicDevice::SetConstantBuffer(ShaderType type, ConstantBuffer* cb, bool overrideBindIndex, uint32_t newIndex)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
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
			XETODO("Add log here");

			return XEResult::InvalidShaderType;

			break;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::SetSamplerState(ShaderType type, uint32_t index, ID3D11SamplerState* sampler)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}
	
	XEAssert(sampler != nullptr);

	if(sampler == nullptr)
	{
		return XEResult::NullParameter;
	}

	ID3D11SamplerState* arrDX[] = { sampler };

	return SetSamplerStates(type, index, 1, arrDX);
}
	
XEResult GraphicDevice::SetSamplerStates(ShaderType type, uint32_t index, uint32_t numStates, ID3D11SamplerState** sampler)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}
	
	XEAssert(sampler != nullptr);

	if(sampler == nullptr)
	{
		return XEResult::NullParameter;
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
			XETODO("Add log here");

			return XEResult::InvalidShaderType;

			break;
	}

	return XEResult::Ok;
}

XEResult GraphicDevice::BeginEvent(const std::wstring& eventName)
{
#if defined(_DEBUG) || defined (PROFILE)

	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(m_UserDefinedAnnotationDX == nullptr)
	{
		return XEResult::NullObj;
	}

	if(m_UserDefinedAnnotationDX->BeginEvent(eventName.c_str()) != 0)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;

#else
	return XEResult::Ok;
#endif
}

XEResult GraphicDevice::EndEvent()
{
#if defined(_DEBUG) || defined (PROFILE)

	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(m_UserDefinedAnnotationDX == nullptr)
	{
		return XEResult::NullObj;
	}

	if(m_UserDefinedAnnotationDX->EndEvent() != 0)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;

#else
	return XEResult::Ok;
#endif
}

XEResult GraphicDevice::SetEventmarker(const std::wstring& eventName)
{
#if defined(_DEBUG) || defined (PROFILE)

	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(m_UserDefinedAnnotationDX == nullptr)
	{
		return XEResult::NullObj;
	}

	m_UserDefinedAnnotationDX->SetMarker(eventName.c_str());

	return XEResult::Ok;

#else
	return XEResult::Ok;
#endif
}

XEResult GraphicDevice::GetBlendState(ID3D11BlendState** blendState, glm::vec4& blendFactor, uint32_t& sampleMask)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XETODO("Implement");
	XETODO("Get adds a ref");

	//XEAssert(blendState != nullptr);
	//if (blendState == nullptr)
	//{
	//	return XEResult::NullParameter;
	//}

	//XETODO("Check that this works correctly");
	//FLOAT* tempBlendFactor = reinterpret_cast<FLOAT*>(&blendFactor);

	//m_DeviceContextDX->OMGetBlendState(blendState, tempBlendFactor, &sampleMask);

	return XEResult::Fail;
}

XEResult GraphicDevice::SetBlendState(ID3D11BlendState* blendState, const glm::vec4& blendFactor, uint32_t sampleMask)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XETODO("Check that this works correctly");
	const FLOAT* tempBlendFactor = reinterpret_cast<const FLOAT*>(&blendFactor);

	m_DeviceContextDX->OMSetBlendState(blendState, tempBlendFactor, sampleMask); 

	return XEResult::Ok;
}