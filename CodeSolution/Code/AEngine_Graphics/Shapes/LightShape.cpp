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
#include "LightShape.h"
#include "GraphicDevice.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\IVertexBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
LightShape::LightShape(GraphicDevice& graphicDevice)
    : m_GraphicDevice(graphicDevice)
{
}

LightShape::~LightShape()
{
    CleanUp();
}

void LightShape::CleanUp()
{
    for (size_t i = 0; i < m_VertexBufferVector.size(); i++)
    {
        DeleteMem(m_VertexBufferVector[i]);
    }

    m_VertexBufferVector.clear();
}

AEResult LightShape::DrawLightShape()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AETODO("add better return codes");
    for (size_t i = 0; i < m_VertexBufferVector.size(); i++)
    {
        if (m_GraphicDevice.SetVertexBuffer(m_VertexBufferVector[i]) != AEResult::Ok)
        {
            return AEResult::Fail;
        }

        if (m_GraphicDevice.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST) != AEResult::Ok)
        {
            return AEResult::Fail;
        }

        if (m_GraphicDevice.Draw(m_VertexBufferVector[i]->GetSize(), 0) != AEResult::Ok)
        {
            return AEResult::Fail;
        }
    }

    return AEResult::Ok;
}
