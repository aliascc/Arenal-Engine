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

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Graphic Device Helpers to debug the game app
/// </summary>
class GraphicDebugDX sealed : public AEObject
{
private:

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

    void ReleaseAndReport();

public:

    /// <summary>
    /// Constructor for GraphicDebugDX
    /// </summary>
    /// <param name="graphicDevice"></param>
    GraphicDebugDX(GraphicDevice* graphicDevice);

    /// <summary>
    /// Destructor for GraphicDebugDX
    /// </summary>
    virtual ~GraphicDebugDX();

    AEResult Initialize();
};

#endif //AE_GRAPHIC_DEBUG_DEVICE
