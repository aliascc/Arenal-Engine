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

/***************************
*   Game Engine Includes   *
****************************/
#include "QuadShape.h"
#include "GraphicDevice.h"
#include "Vertex\IndexBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
QuadShape::QuadShape(GraphicDevice& graphicDevice, bool clockWise)
    : m_ClockWise(clockWise)
    , m_GraphicDevice(graphicDevice)
{
}

QuadShape::~QuadShape()
{
    DeleteMem(m_IB);
    DeleteMem(m_VB);
}

void QuadShape::DrawQuad()
{
    AEAssert(m_IsReady);

    m_GraphicDevice.SetVertexBuffer(m_VB);

    m_GraphicDevice.SetIndexBuffer(m_IB);

    m_GraphicDevice.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_GraphicDevice.DrawIndexed(0, 0, m_IB->GetSize());
}
