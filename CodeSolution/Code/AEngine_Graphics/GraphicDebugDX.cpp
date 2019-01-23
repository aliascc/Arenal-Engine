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
#include "GraphicDebugDX.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

/*********************
*   GraphicDebugDX   *
**********************/
GraphicDebugDX::GraphicDebugDX(GraphicDevice* graphicDevice)
    : m_GraphicDevice(graphicDevice)
{
    AEAssert(graphicDevice != nullptr);
}

GraphicDebugDX::~GraphicDebugDX()
{
    ReleaseAndReport();

    ReleaseCOM(m_DXGIInfoQueue);

    if (m_HandleDXGIDebugDLL != nullptr)
    {
        FreeLibrary(m_HandleDXGIDebugDLL);
    }
}

AEResult GraphicDebugDX::Initialize()
{
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::Fail;
    }

    m_HandleDXGIDebugDLL = LoadLibrary("dxgidebug.dll");

    if (m_HandleDXGIDebugDLL == nullptr)
    {
        //Library failed to load...

        AETODO("Add log error");

        return AEResult::Fail;
    }

    typedef HRESULT(__stdcall *fPtr)(const IID&, void**);
    fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(m_HandleDXGIDebugDLL, "DXGIGetDebugInterface");

    if (DXGIGetDebugInterface == nullptr)
    {
        FreeLibrary(m_HandleDXGIDebugDLL);

        return AEResult::Fail;
    }

    m_GraphicDevice->GetDeviceDX()->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_D3D11Debug);

    if (m_D3D11Debug == nullptr)
    {
        AETODO("Add log");

        return AEResult::Fail;
    }

    DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_DXGIDebug);
    DXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)&m_DXGIInfoQueue);

    if (m_DXGIDebug == nullptr || m_DXGIInfoQueue == nullptr)
    {
        FreeLibrary(m_HandleDXGIDebugDLL);
        ReleaseCOM(m_D3D11Debug);

        return AEResult::Fail;
    }

    m_DXGIInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
    m_DXGIInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

    return AEResult::Ok;
}

void GraphicDebugDX::ReleaseAndReport()
{
    if (m_D3D11Debug != nullptr)
    {
        OutputDebugString("\n\n--------AEngine Calling ReportLiveDeviceObjects--------\n\n\n");
        m_D3D11Debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
        OutputDebugString("\n\n--------AEngine End Call ReportLiveDeviceObjects--------\n\n\n");
    }
    ReleaseCOM(m_D3D11Debug);

    if (m_DXGIDebug != nullptr)
    {
        OutputDebugString("\n\n--------AEngine Calling ReportLiveObjects--------\n\n\n");
        m_DXGIDebug->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_DETAIL));
        OutputDebugString("\n\n--------AEngine End Call ReportLiveObjects--------\n\n\n");
    }

    ReleaseCOM(m_DXGIDebug);
}

#endif