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

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Viewport.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Viewport::Viewport()
{
    memset(&m_ViewportDX, 0, sizeof(D3D11_VIEWPORT));
}

Viewport::~Viewport()
{
}

AEResult Viewport::Initialize(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
    m_ViewportDX.TopLeftX    = topLeftX;
    m_ViewportDX.TopLeftY    = topLeftY;
    m_ViewportDX.Width       = width;
    m_ViewportDX.Height      = height;
    m_ViewportDX.MinDepth    = minDepth;
    m_ViewportDX.MaxDepth    = maxDepth;

    return AEResult::Ok;
}
