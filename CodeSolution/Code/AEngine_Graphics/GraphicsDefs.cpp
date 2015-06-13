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

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicsDefs.h"
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Logger\LoggerDefs.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

#if defined(_DEBUG) || defined(PROFILE)

/*********************
*   GraphicDebugDX   *
**********************/
GraphicDebugDX::GraphicDebugDX(GraphicDevice* graphicDevice)
	: m_GraphicDevice(graphicDevice)
{
	AEAssert(graphicDevice != nullptr);
}

AEResult GraphicDebugDX::Initialize()
{
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::Fail;
	}

	m_HandleDXGIDebugDLL = LoadLibrary(L"dxgidebug.dll");
	
	if(m_HandleDXGIDebugDLL == nullptr)
	{
		//Library failed to load...

		AETODO("Add log error");

		return AEResult::Fail;
	}

	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);  
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(m_HandleDXGIDebugDLL, "DXGIGetDebugInterface");  

	if(DXGIGetDebugInterface  == nullptr)
	{
		FreeLibrary(m_HandleDXGIDebugDLL);
	
		return AEResult::Fail;
	}

	m_GraphicDevice->GetDeviceDX()->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_D3D11Debug);

	if(m_D3D11Debug == nullptr)
	{
		AETODO("Add log");

		return AEResult::Fail;
	}

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_DXGIDebug);
	DXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)&m_DXGIInfoQueue);
	
	if(m_DXGIDebug == nullptr || m_DXGIInfoQueue == nullptr)
	{
		FreeLibrary(m_HandleDXGIDebugDLL);
		ReleaseCOM(m_D3D11Debug);
		
		return AEResult::Fail;
	}

	return AEResult::Ok;
}

void GraphicDebugDX::Report()
{
	if(m_D3D11Debug != nullptr)
	{
		OutputDebugString(L"\n\n--------AEngine Calling ReportLiveDeviceObjects--------\n\n\n");
		m_D3D11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		OutputDebugString(L"\n\n--------AEngine End Call ReportLiveDeviceObjects--------\n\n\n");
	}

	if(m_DXGIDebug != nullptr)
	{		
		OutputDebugString(L"\n\n--------AEngine Calling ReportLiveObjects--------\n\n\n");
		m_DXGIDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		OutputDebugString(L"\n\n--------AEngine End Call ReportLiveObjects--------\n\n\n");
	}
}

GraphicDebugDX::~GraphicDebugDX()
{	
	ReleaseCOM(m_D3D11Debug);
	ReleaseCOM(m_DXGIDebug);
	ReleaseCOM(m_DXGIInfoQueue);

	if(m_HandleDXGIDebugDLL != nullptr)
	{
		FreeLibrary(m_HandleDXGIDebugDLL);
	}
}

#endif

/***************************
*   GraphicOptsPreferred   *
****************************/
GraphicOptsPreferred::GraphicOptsPreferred()
{
}

/*************************************
*   GraphicsPresentationParameters   *
**************************************/
AETODO("CHECK VARIABLES TO REMOVE");
GraphicsPresentationParameters::GraphicsPresentationParameters()
{
	m_MultiSample.Count = 1;
	m_MultiSample.Quality = 0;
}

/**************************
*   GraphicsCheckFormat   *
***************************/
GraphicsCheckFormat::GraphicsCheckFormat()
{
}

/***************************
*   GraphicsCheckDevCaps   *
****************************/
GraphicsCheckDevCaps::GraphicsCheckDevCaps()
{
}

/***************************
*   GraphicsCheckDevCaps   *
****************************/
ID3D11BlendState* GraphicBlendStates::m_DefaultState			= nullptr;
ID3D11BlendState* GraphicBlendStates::m_AdditiveState			= nullptr;
ID3D11BlendState* GraphicBlendStates::m_AlphaBlendState			= nullptr;
ID3D11BlendState* GraphicBlendStates::m_OpaqueState				= nullptr;
ID3D11BlendState* GraphicBlendStates::m_NonPremultipliedState	= nullptr;

AEResult GraphicBlendStates::InitStates(GraphicDevice* graphicDevice)
{
	AEAssert(graphicDevice != nullptr);
	if(graphicDevice == nullptr)
	{
		return AEResult::NullParameter;
	}

	ID3D11Device* dxDevice = graphicDevice->GetDeviceDX();
	HRESULT hr = S_OK;

	//Release States if already created
	ReleaseStates();

	//Create Default Desc for Blend States
	D3D11_BLEND_DESC dxDefaultBlendDesc = { 0 };
	dxDefaultBlendDesc.AlphaToCoverageEnable					= FALSE;
	dxDefaultBlendDesc.IndependentBlendEnable					= FALSE;
	dxDefaultBlendDesc.RenderTarget[0].BlendEnable				= FALSE;
	dxDefaultBlendDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_ONE;
	dxDefaultBlendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ZERO;
	dxDefaultBlendDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	dxDefaultBlendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	dxDefaultBlendDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	dxDefaultBlendDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	dxDefaultBlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	//Temp Desc for Different States
	D3D11_BLEND_DESC dxTempDesc = dxDefaultBlendDesc;

	//Create Default Blend State
	hr = dxDevice->CreateBlendState(&dxTempDesc, &m_DefaultState);

	if(hr != S_OK)
	{
		DisplayError(hr);
		
		ReleaseStates();

		AETODO("Add log here");

		return AEResult::Fail;
	}

	AEGraphicHelpers::SetDebugObjectName<ID3D11BlendState>(m_DefaultState, AE_DEBUG_DEFAULT_BLEND_STATE_NAME);

	//Create Additive Blend State
	dxTempDesc = dxDefaultBlendDesc;
	dxTempDesc.RenderTarget[0].BlendEnable	= TRUE;
	dxTempDesc.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
	dxTempDesc.RenderTarget[0].DestBlend	= D3D11_BLEND_ONE;
	hr = dxDevice->CreateBlendState(&dxTempDesc, &m_AdditiveState);

	if(hr != S_OK)
	{
		DisplayError(hr);
		
		ReleaseStates();

		AETODO("Add log here");

		return AEResult::Fail;
	}

	AEGraphicHelpers::SetDebugObjectName<ID3D11BlendState>(m_AdditiveState, AE_DEBUG_ADDICTIVE_BLEND_STATE_NAME);

	//Create Alpha Blend State
	dxTempDesc = dxDefaultBlendDesc;
	dxTempDesc.RenderTarget[0].BlendEnable	= TRUE;
	dxTempDesc.RenderTarget[0].SrcBlend		= D3D11_BLEND_ONE;
	dxTempDesc.RenderTarget[0].DestBlend	= D3D11_BLEND_INV_SRC_ALPHA;
	hr = dxDevice->CreateBlendState(&dxTempDesc, &m_AlphaBlendState);

	if(hr != S_OK)
	{
		DisplayError(hr);
		
		ReleaseStates();

		AETODO("Add log here");

		return AEResult::Fail;
	}

	AEGraphicHelpers::SetDebugObjectName<ID3D11BlendState>(m_AlphaBlendState, AE_DEBUG_ALPHA_BLEND_STATE_NAME);

	//Create Opaque Blend State
	dxTempDesc = dxDefaultBlendDesc;
	dxTempDesc.RenderTarget[0].BlendEnable	= TRUE;
	dxTempDesc.RenderTarget[0].SrcBlend		= D3D11_BLEND_ONE;
	dxTempDesc.RenderTarget[0].DestBlend	= D3D11_BLEND_ZERO;
	hr = dxDevice->CreateBlendState(&dxTempDesc, &m_OpaqueState);

	if(hr != S_OK)
	{
		DisplayError(hr);
		
		ReleaseStates();

		AETODO("Add log here");

		return AEResult::Fail;
	}
	
	AEGraphicHelpers::SetDebugObjectName<ID3D11BlendState>(m_OpaqueState, AE_DEBUG_OPAQUE_BLEND_STATE_NAME);

	//Create Non-Premultiplied Blend State
	dxTempDesc = dxDefaultBlendDesc;
	dxTempDesc.RenderTarget[0].BlendEnable	= TRUE;
	dxTempDesc.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
	dxTempDesc.RenderTarget[0].DestBlend	= D3D11_BLEND_INV_SRC_ALPHA;
	hr = dxDevice->CreateBlendState(&dxTempDesc, &m_NonPremultipliedState);

	if(hr != S_OK)
	{
		DisplayError(hr);
		
		ReleaseStates();

		AETODO("Add log here");

		return AEResult::Fail;
	}
	
	AEGraphicHelpers::SetDebugObjectName<ID3D11BlendState>(m_NonPremultipliedState, AE_DEBUG_NON_PREMULTIPLIED_BLEND_STATE_NAME);

	return AEResult::Ok;
}

void GraphicBlendStates::ReleaseStates()
{
	ReleaseCOM(m_DefaultState);
	ReleaseCOM(m_AdditiveState);
	ReleaseCOM(m_AlphaBlendState);
	ReleaseCOM(m_OpaqueState);
	ReleaseCOM(m_NonPremultipliedState);
}

/***********************
*   AEGraphicHelpers   *
************************/
namespace AEGraphicHelpers
{

	glm::ivec2 GetWindowSize(HWND hWnd)
	{
		RECT rectWnd;
		glm::ivec2 wndSize = AEMathHelpers::Vec2iZero;

		GetClientRect(hWnd, &rectWnd);
		wndSize.x = rectWnd.right - rectWnd.left;
		wndSize.y = rectWnd.bottom - rectWnd.top;

		return wndSize;
	}

	D3D11_USAGE GetDXUsage(GraphicBufferUsage bufferUsage)
	{
		D3D11_USAGE dxUsage = D3D11_USAGE_DEFAULT;

		switch(bufferUsage)
		{
			case GraphicBufferUsage::Default:
				dxUsage = D3D11_USAGE_DEFAULT;
				break;
			case GraphicBufferUsage::Static:
				dxUsage = D3D11_USAGE_IMMUTABLE;
				break;
			case GraphicBufferUsage::Dynamic:
				dxUsage = D3D11_USAGE_DYNAMIC;
				break;
			case GraphicBufferUsage::ReadWrite:
				dxUsage = D3D11_USAGE_STAGING;
				break;
			default:
				std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"DX_TYPE_NOT_FOUND_ERR_MSG"), __FUNCTIONW__, L"Buffer Usage", static_cast<uint32_t>(bufferUsage), L"D3D11_USAGE_DEFAULT");
				AELOGGER->AddNewLog(LogLevel::Warning, msgerr);

				break;
		}

		return dxUsage;
	}

	UINT GetDXBufferAccess(GraphicBufferAccess bufferAccess)
	{
		UINT dxAccess = 0;

		switch(bufferAccess)
		{
			case GraphicBufferAccess::None:
				dxAccess = 0;
				break;
			case GraphicBufferAccess::Read:
				dxAccess = D3D11_CPU_ACCESS_READ;
				break;
			case GraphicBufferAccess::Write:
				dxAccess = D3D11_CPU_ACCESS_WRITE;
				break;
			case GraphicBufferAccess::ReadWrite:
				dxAccess = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
				break;
			default:
				std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"DX_TYPE_NOT_FOUND_ERR_MSG"), __FUNCTIONW__, L"Buffer Access", static_cast<uint32_t>(bufferAccess), L"0");
				AELOGGER->AddNewLog(LogLevel::Warning, msgerr);

				break;
		}

		return dxAccess;
	}

	D3D11_MAP GetDXResourceMap(GraphicResourceMap resourceMap)
	{
		D3D11_MAP dxMap = D3D11_MAP_READ;

		switch(resourceMap)
		{
			case GraphicResourceMap::Read:
				dxMap = D3D11_MAP_READ;
				break;

			case GraphicResourceMap::Write:
				dxMap = D3D11_MAP_WRITE;
				break;

			case GraphicResourceMap::ReadWrite:
				dxMap = D3D11_MAP_READ_WRITE;
				break;

			case GraphicResourceMap::WriteDiscard:
				dxMap = D3D11_MAP_WRITE_DISCARD;
				break;

			case GraphicResourceMap::WriteNoOvewrite:
				dxMap = D3D11_MAP_WRITE_NO_OVERWRITE;
				break;

			default:
				AEAssert(false);
				AETODO("Add error message");

				break;
		}

		return dxMap;
	}

	DXGI_FORMAT GetPXFormatFromString(const std::wstring& pxFormat)
	{
		if(pxFormat.compare(L"DXGI_FORMAT_UNKNOWN") == 0)
		{
			return DXGI_FORMAT_UNKNOWN;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32A32_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R32G32B32A32_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32A32_FLOAT") == 0)
		{
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32A32_UINT") == 0)
		{
			return DXGI_FORMAT_R32G32B32A32_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32A32_SINT") == 0)
		{
			return DXGI_FORMAT_R32G32B32A32_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R32G32B32_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32_FLOAT") == 0)
		{
			return DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32_UINT") == 0)
		{
			return DXGI_FORMAT_R32G32B32_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32B32_SINT") == 0)
		{
			return DXGI_FORMAT_R32G32B32_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16B16A16_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R16G16B16A16_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16B16A16_FLOAT") == 0)
		{
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16B16A16_UNORM") == 0)
		{
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16B16A16_UINT") == 0)
		{
			return DXGI_FORMAT_R16G16B16A16_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16B16A16_SNORM") == 0)
		{
			return DXGI_FORMAT_R16G16B16A16_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16B16A16_SINT") == 0)
		{
			return DXGI_FORMAT_R16G16B16A16_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R32G32_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32_FLOAT") == 0)
		{
			return DXGI_FORMAT_R32G32_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32_UINT") == 0)
		{
			return DXGI_FORMAT_R32G32_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G32_SINT") == 0)
		{
			return DXGI_FORMAT_R32G32_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32G8X24_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R32G8X24_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_D32_FLOAT_S8X24_UINT") == 0)
		{
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT") == 0)
		{
			return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R10G10B10A2_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R10G10B10A2_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R10G10B10A2_UNORM") == 0)
		{
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R10G10B10A2_UINT") == 0)
		{
			return DXGI_FORMAT_R10G10B10A2_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R11G11B10_FLOAT") == 0)
		{
			return DXGI_FORMAT_R11G11B10_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8B8A8_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R8G8B8A8_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8B8A8_UNORM") == 0)
		{
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB") == 0)
		{
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8B8A8_UINT") == 0)
		{
			return DXGI_FORMAT_R8G8B8A8_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8B8A8_SNORM") == 0)
		{
			return DXGI_FORMAT_R8G8B8A8_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8B8A8_SINT") == 0)
		{
			return DXGI_FORMAT_R8G8B8A8_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R16G16_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16_FLOAT") == 0)
		{
			return DXGI_FORMAT_R16G16_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16_UNORM") == 0)
		{
			return DXGI_FORMAT_R16G16_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16_UINT") == 0)
		{
			return DXGI_FORMAT_R16G16_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16_SNORM") == 0)
		{
			return DXGI_FORMAT_R16G16_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16G16_SINT") == 0)
		{
			return DXGI_FORMAT_R16G16_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R32_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_D32_FLOAT") == 0)
		{
			return DXGI_FORMAT_D32_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32_FLOAT") == 0)
		{
			return DXGI_FORMAT_R32_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32_UINT") == 0)
		{
			return DXGI_FORMAT_R32_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R32_SINT") == 0)
		{
			return DXGI_FORMAT_R32_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R24G8_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R24G8_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_D24_UNORM_S8_UINT") == 0)
		{
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R24_UNORM_X8_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_X24_TYPELESS_G8_UINT") == 0)
		{
			return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R8G8_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8_UNORM") == 0)
		{
			return DXGI_FORMAT_R8G8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8_UINT") == 0)
		{
			return DXGI_FORMAT_R8G8_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8_SNORM") == 0)
		{
			return DXGI_FORMAT_R8G8_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8_SINT") == 0)
		{
			return DXGI_FORMAT_R8G8_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R16_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16_FLOAT") == 0)
		{
			return DXGI_FORMAT_R16_FLOAT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_D16_UNORM") == 0)
		{
			return DXGI_FORMAT_D16_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16_UNORM") == 0)
		{
			return DXGI_FORMAT_R16_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16_UINT") == 0)
		{
			return DXGI_FORMAT_R16_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16_SNORM") == 0)
		{
			return DXGI_FORMAT_R16_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R16_SINT") == 0)
		{
			return DXGI_FORMAT_R16_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8_TYPELESS") == 0)
		{
			return DXGI_FORMAT_R8_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8_UNORM") == 0)
		{
			return DXGI_FORMAT_R8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8_UINT") == 0)
		{
			return DXGI_FORMAT_R8_UINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8_SNORM") == 0)
		{
			return DXGI_FORMAT_R8_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8_SINT") == 0)
		{
			return DXGI_FORMAT_R8_SINT;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_A8_UNORM") == 0)
		{
			return DXGI_FORMAT_A8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R1_UNORM") == 0)
		{
			return DXGI_FORMAT_R1_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R9G9B9E5_SHAREDEXP") == 0)
		{
			return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R8G8_B8G8_UNORM") == 0)
		{
			return DXGI_FORMAT_R8G8_B8G8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_G8R8_G8B8_UNORM") == 0)
		{
			return DXGI_FORMAT_G8R8_G8B8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC1_TYPELESS") == 0)
		{
			return DXGI_FORMAT_BC1_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC1_UNORM") == 0)
		{
			return DXGI_FORMAT_BC1_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC1_UNORM_SRGB") == 0)
		{
			return DXGI_FORMAT_BC1_UNORM_SRGB;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC2_TYPELESS") == 0)
		{
			return DXGI_FORMAT_BC2_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC2_UNORM") == 0)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC2_UNORM_SRGB") == 0)
		{
			return DXGI_FORMAT_BC2_UNORM_SRGB;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC3_TYPELESS") == 0)
		{
			return DXGI_FORMAT_BC3_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC3_UNORM") == 0)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC3_UNORM_SRGB") == 0)
		{
			return DXGI_FORMAT_BC3_UNORM_SRGB;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC4_TYPELESS") == 0)
		{
			return DXGI_FORMAT_BC4_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC4_UNORM") == 0)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC4_SNORM") == 0)
		{
			return DXGI_FORMAT_BC4_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC5_TYPELESS") == 0)
		{
			return DXGI_FORMAT_BC5_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC5_UNORM") == 0)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC5_SNORM") == 0)
		{
			return DXGI_FORMAT_BC5_SNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B5G6R5_UNORM") == 0)
		{
			return DXGI_FORMAT_B5G6R5_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B5G5R5A1_UNORM") == 0)
		{
			return DXGI_FORMAT_B5G5R5A1_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B8G8R8A8_UNORM") == 0)
		{
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B8G8R8X8_UNORM") == 0)
		{
			return DXGI_FORMAT_B8G8R8X8_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM") == 0)
		{
			return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B8G8R8A8_TYPELESS") == 0)
		{
			return DXGI_FORMAT_B8G8R8A8_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B8G8R8A8_UNORM_SRGB") == 0)
		{
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B8G8R8X8_TYPELESS") == 0)
		{
			return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_B8G8R8X8_UNORM_SRGB") == 0)
		{
			return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC6H_TYPELESS") == 0)
		{
			return DXGI_FORMAT_BC6H_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC6H_UF16") == 0)
		{
			return DXGI_FORMAT_BC6H_UF16;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC6H_SF16") == 0)
		{
			return DXGI_FORMAT_BC6H_SF16;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC7_TYPELESS") == 0)
		{
			return DXGI_FORMAT_BC7_TYPELESS;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC7_UNORM") == 0)
		{
			return DXGI_FORMAT_BC7_UNORM;
		}
		else if(pxFormat.compare(L"DXGI_FORMAT_BC7_UNORM_SRGB") == 0)
		{
			return DXGI_FORMAT_BC7_UNORM_SRGB;
		}
		else
		{
			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"ShaderType::AE_ST_PX_FMT_NOT_FOUND_ERR_MSG"), __FUNCTIONW__, pxFormat, L"DXGI_FORMAT_UNKNOWN");
			AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
		
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	uint32_t GetSizeOfDXFormat(DXGI_FORMAT format)
	{
		switch( format )
		{
			case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
			case DXGI_FORMAT_R32G32B32A32_UINT:
			case DXGI_FORMAT_R32G32B32A32_SINT:
				return (128 / 8);

			case DXGI_FORMAT_R32G32B32_TYPELESS:
			case DXGI_FORMAT_R32G32B32_FLOAT:
			case DXGI_FORMAT_R32G32B32_UINT:
			case DXGI_FORMAT_R32G32B32_SINT:
				return (96 / 8);

			case DXGI_FORMAT_R16G16B16A16_TYPELESS:
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
			case DXGI_FORMAT_R16G16B16A16_UNORM:
			case DXGI_FORMAT_R16G16B16A16_UINT:
			case DXGI_FORMAT_R16G16B16A16_SNORM:
			case DXGI_FORMAT_R16G16B16A16_SINT:
			case DXGI_FORMAT_R32G32_TYPELESS:
			case DXGI_FORMAT_R32G32_FLOAT:
			case DXGI_FORMAT_R32G32_UINT:
			case DXGI_FORMAT_R32G32_SINT:
			case DXGI_FORMAT_R32G8X24_TYPELESS:
			case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
				return (64 / 8);

			case DXGI_FORMAT_R10G10B10A2_TYPELESS:
			case DXGI_FORMAT_R10G10B10A2_UNORM:
			case DXGI_FORMAT_R10G10B10A2_UINT:
			case DXGI_FORMAT_R11G11B10_FLOAT:
			case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			case DXGI_FORMAT_R8G8B8A8_UINT:
			case DXGI_FORMAT_R8G8B8A8_SNORM:
			case DXGI_FORMAT_R8G8B8A8_SINT:
			case DXGI_FORMAT_R16G16_TYPELESS:
			case DXGI_FORMAT_R16G16_FLOAT:
			case DXGI_FORMAT_R16G16_UNORM:
			case DXGI_FORMAT_R16G16_UINT:
			case DXGI_FORMAT_R16G16_SNORM:
			case DXGI_FORMAT_R16G16_SINT:
			case DXGI_FORMAT_R32_TYPELESS:
			case DXGI_FORMAT_D32_FLOAT:
			case DXGI_FORMAT_R32_FLOAT:
			case DXGI_FORMAT_R32_UINT:
			case DXGI_FORMAT_R32_SINT:
			case DXGI_FORMAT_R24G8_TYPELESS:
			case DXGI_FORMAT_D24_UNORM_S8_UINT:
			case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
			case DXGI_FORMAT_B8G8R8X8_UNORM:
				return (32 / 8);

			case DXGI_FORMAT_R8G8_TYPELESS:
			case DXGI_FORMAT_R8G8_UNORM:
			case DXGI_FORMAT_R8G8_UINT:
			case DXGI_FORMAT_R8G8_SNORM:
			case DXGI_FORMAT_R8G8_SINT:
			case DXGI_FORMAT_R16_TYPELESS:
			case DXGI_FORMAT_R16_FLOAT:
			case DXGI_FORMAT_D16_UNORM:
			case DXGI_FORMAT_R16_UNORM:
			case DXGI_FORMAT_R16_UINT:
			case DXGI_FORMAT_R16_SNORM:
			case DXGI_FORMAT_R16_SINT:
			case DXGI_FORMAT_B5G6R5_UNORM:
			case DXGI_FORMAT_B5G5R5A1_UNORM:
				return (16 / 8);

			case DXGI_FORMAT_R8_TYPELESS:
			case DXGI_FORMAT_R8_UNORM:
			case DXGI_FORMAT_R8_UINT:
			case DXGI_FORMAT_R8_SNORM:
			case DXGI_FORMAT_R8_SINT:
			case DXGI_FORMAT_A8_UNORM:
				return (8 / 8);

			// Compressed format; http://msdn2.microso.../bb694531(VS.85).aspx
			case DXGI_FORMAT_BC2_TYPELESS:
			case DXGI_FORMAT_BC2_UNORM:
			case DXGI_FORMAT_BC2_UNORM_SRGB:
			case DXGI_FORMAT_BC3_TYPELESS:
			case DXGI_FORMAT_BC3_UNORM:
			case DXGI_FORMAT_BC3_UNORM_SRGB:
			case DXGI_FORMAT_BC5_TYPELESS:
			case DXGI_FORMAT_BC5_UNORM:
			case DXGI_FORMAT_BC5_SNORM:
				return (128 / 8);

			// Compressed format; http://msdn2.microso.../bb694531(VS.85).aspx
			case DXGI_FORMAT_R1_UNORM:
			case DXGI_FORMAT_BC1_TYPELESS:
			case DXGI_FORMAT_BC1_UNORM:
			case DXGI_FORMAT_BC1_UNORM_SRGB:
			case DXGI_FORMAT_BC4_TYPELESS:
			case DXGI_FORMAT_BC4_UNORM:
			case DXGI_FORMAT_BC4_SNORM:
				return (64 / 8);

			// Compressed format; http://msdn2.microso.../bb694531(VS.85).aspx
			case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
				return (32 / 8);

			// These are compressed, but bit-size information is unclear.
			case DXGI_FORMAT_R8G8_B8G8_UNORM:
			case DXGI_FORMAT_G8R8_G8B8_UNORM:
				return (32 / 8);

			case DXGI_FORMAT_UNKNOWN:
			default:
				AETODO("Add Log Here");
				return 0;
		}
	}

	std::wstring GetShaderTypeString(ShaderType shaderType)
	{
		switch (shaderType)
		{
			case ShaderType::VertexShader:
				return L"Vertex Shader";

			case ShaderType::PixelShader:
				return L"Pixel Shader";

			case ShaderType::GeometryShader:
				return L"Geometry Shader";

			case ShaderType::HullShader:
				return L"Hull Shader";

			case ShaderType::ComputeShader:
				return L"Compute Shader";

			case ShaderType::DomainShader:
				return L"Domain Shader";

			default:
				AEAssert(false);
				return L"";
		}
	}

	ShaderType GetShaderTypeFromString(std::wstring shaderType)
	{
		if(shaderType.compare(L"Vertex Shader") == 0 || shaderType.compare(L"VertexShader") == 0)
		{
			return ShaderType::VertexShader;
		}
		else if(shaderType.compare(L"Pixel Shader") == 0 || shaderType.compare(L"PixelShader") == 0)
		{
			return ShaderType::PixelShader;
		}
		else if(shaderType.compare(L"Geometry Shader") == 0 || shaderType.compare(L"GeometryShader") == 0)
		{
			return ShaderType::GeometryShader;
		}
		else if(shaderType.compare(L"Hull Shader") == 0 || shaderType.compare(L"HullShader") == 0)
		{
			return ShaderType::HullShader;
		}
		else if(shaderType.compare(L"Compute Shader") == 0 || shaderType.compare(L"ComputeShader") == 0)
		{
			return ShaderType::ComputeShader;
		}
		else if(shaderType.compare(L"Domain Shader") == 0 || shaderType.compare(L"DomainShader") == 0)
		{
			return ShaderType::DomainShader;
		}
		else
		{
			AETODO("Add error to log");

			return ShaderType::VertexShader;
		}
	}

	std::wstring GetTextureTypeString(TextureType textureType)
	{
		switch (textureType)
		{
			case TextureType::Texture2D:
				return L"Texture 2D";

			case TextureType::TextureCube:
				return L"Texture Cube";

			default:
				AEAssert(false);
				return L"";
		}
	}

	TextureType GetTextureTypeString(std::wstring textureType)
	{
		if(textureType.compare(L"Texture2D") == 0 || textureType.compare(L"Texture 2D") == 0)
		{
			return TextureType::Texture2D;
		}
		else if(textureType.compare(L"TextureCube") == 0 || textureType.compare(L"Texture Cube") == 0)
		{
			return TextureType::TextureCube;
		}
		else
		{
			AETODO("Add error to log");

			return TextureType::Texture2D;
		}
	}

}