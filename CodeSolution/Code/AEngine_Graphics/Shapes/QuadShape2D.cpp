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

/***************************
*   Game Engine Includes   *
****************************/
#include "QuadShape2D.h"
#include "GraphicDevice.h"
#include "Vertex\IndexBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
QuadShape2D::QuadShape2D(GraphicDevice& graphicDevice, bool dynamic, bool clockWise)
    : QuadShape(graphicDevice, clockWise)
    , m_Dynamic(dynamic)
{
    memset(&m_CurrentSize, 0, sizeof(RECT));
}

QuadShape2D::~QuadShape2D()
{
}

AEResult QuadShape2D::BuildQuad()
{
    m_IsReady = false;
    DeleteMem(m_IB);
    DeleteMem(m_VB);

    //Clean Vertex Size
    memset(m_Vtx, 0, sizeof(VertexPositionTexture)* 4);

    m_CurrentTexCoords = glm::vec4(0, 0, 1, 1);

    //Top Left
    m_Vtx[0].m_Position.x = -1;
    m_Vtx[0].m_Position.y = 1;
    m_Vtx[0].m_Position.z = 0;
    m_Vtx[0].m_TexCoord.x = m_CurrentTexCoords.x;
    m_Vtx[0].m_TexCoord.y = m_CurrentTexCoords.y;
    
    //Top Right
    m_Vtx[1].m_Position.x = 1;
    m_Vtx[1].m_Position.y = 1;
    m_Vtx[1].m_Position.z = 0;
    m_Vtx[1].m_TexCoord.x = m_CurrentTexCoords.z;
    m_Vtx[1].m_TexCoord.y = m_CurrentTexCoords.y;
    
    //Bottom Right
    m_Vtx[2].m_Position.x = 1;
    m_Vtx[2].m_Position.y = -1;
    m_Vtx[2].m_Position.z = 0;
    m_Vtx[2].m_TexCoord.x = m_CurrentTexCoords.z;
    m_Vtx[2].m_TexCoord.y = m_CurrentTexCoords.w;
    
    //Bottom Left
    m_Vtx[3].m_Position.x = -1;
    m_Vtx[3].m_Position.y = -1;
    m_Vtx[3].m_Position.z = 0;
    m_Vtx[3].m_TexCoord.x = m_CurrentTexCoords.x;
    m_Vtx[3].m_TexCoord.y = m_CurrentTexCoords.w;

    GraphicBufferUsage usage = GraphicBufferUsage::Static;
    GraphicBufferAccess access = GraphicBufferAccess::None;

    if(m_Dynamic)
    {
        usage = GraphicBufferUsage::Dynamic;
        access = GraphicBufferAccess::Write;
    }

    m_VB = new VertexBuffer<VertexPositionTexture>(m_GraphicDevice, usage, access);
    m_VB->CopyVertexBuffer(m_Vtx, 4);
    m_VB->BuildVertexBuffer();

    uint16_t idxs[6];

    if(m_ClockWise)
    {
        idxs[0] = 0;
        idxs[1] = 1;
        idxs[2] = 2;
        idxs[3] = 2;
        idxs[4] = 3;
        idxs[5] = 0;
    }
    else
    {
        idxs[0] = 0;
        idxs[1] = 3;
        idxs[2] = 2;
        idxs[3] = 2;
        idxs[4] = 1;
        idxs[5] = 0;
    }

    m_IB = new IndexBuffer(m_GraphicDevice);
    m_IB->CopyToIndexBuffer(idxs, 6);
    m_IB->BuildIndexBuffer();

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult QuadShape2D::Resize(const RECT& size, const glm::vec4& textCoords)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if(!m_Dynamic)
    {
        return AEResult::Fail;
    }

    if(m_CurrentTexCoords == textCoords && AEMathHelpers::EqualRECT(m_CurrentSize, size))
    {
        return AEResult::Ok;
    }

    uint32_t width  = m_GraphicDevice.GetGraphicPP().m_GameBackBufferWidth;
    uint32_t height = m_GraphicDevice.GetGraphicPP().m_GameBackBufferHeight;

    //Top Left
    m_Vtx[0].m_Position.x = ( ((float)size.left / (float)width) * 2) - 1;
    m_Vtx[0].m_Position.y = ( ((float)size.top / (float)height) * -2) + 1;
    
    m_Vtx[0].m_TexCoord.x = textCoords.x;
    m_Vtx[0].m_TexCoord.y = textCoords.y;
    
    //Bottom Right
    m_Vtx[2].m_Position.x = ( ((float)size.right / (float)width) * 2) - 1;
    m_Vtx[2].m_Position.y = ( ((float)size.bottom / (float)height) * -2) + 1;

    m_Vtx[2].m_TexCoord.x = textCoords.z;
    m_Vtx[2].m_TexCoord.y = textCoords.w;
    
    //Top Right
    m_Vtx[1].m_Position.x = m_Vtx[2].m_Position.x;
    m_Vtx[1].m_Position.y = m_Vtx[0].m_Position.y;

    m_Vtx[1].m_TexCoord.x = textCoords.z;
    m_Vtx[1].m_TexCoord.y = textCoords.y;
    
    //Bottom Left
    m_Vtx[3].m_Position.x = m_Vtx[0].m_Position.x;
    m_Vtx[3].m_Position.y = m_Vtx[2].m_Position.y;

    m_Vtx[3].m_TexCoord.x = textCoords.x;
    m_Vtx[3].m_TexCoord.y = textCoords.w;

    if(m_VB->SetVertexData(0, m_Vtx, 0, 4, D3D11_MAP_WRITE_DISCARD) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    m_CurrentSize = size;
    m_CurrentTexCoords = textCoords;

    return AEResult::Ok;
}
