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
#include "QuadShape3D.h"
#include "GraphicDevice.h"
#include "Vertex\IndexBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
QuadShape3D::QuadShape3D(GraphicDevice& graphicDevice, bool clockWise)
    : QuadShape(graphicDevice, clockWise)
{
}

QuadShape3D::~QuadShape3D()
{
}

AEResult QuadShape3D::BuildQuad()
{
    m_IsReady = false;
    DeleteMem(m_IB);
    DeleteMem(m_VB);

    //Clean Vertex Size
    memset(m_Vtx, 0, sizeof(VertexPositionTexture)* 4);
    
    //Top Left
    m_Vtx[0].m_Position.x = -0.5f;
    m_Vtx[0].m_Position.y = 0.5f;
    m_Vtx[0].m_Position.z = 0.0f;
    m_Vtx[0].m_TexCoord.x = 0.0f;
    m_Vtx[0].m_TexCoord.y = 0.0f;
    
    //Top Right
    m_Vtx[1].m_Position.x = 0.5f;
    m_Vtx[1].m_Position.y = 0.5f;
    m_Vtx[1].m_Position.z = 0.0f;
    m_Vtx[1].m_TexCoord.x = 1.0f;
    m_Vtx[1].m_TexCoord.y = 0.0f;
    
    //Bottom Right
    m_Vtx[2].m_Position.x = 0.5f;
    m_Vtx[2].m_Position.y = -0.5f;
    m_Vtx[2].m_Position.z = 0.0f;
    m_Vtx[2].m_TexCoord.x = 1.0f;
    m_Vtx[2].m_TexCoord.y = 1.0f;
    
    //Bottom Left
    m_Vtx[3].m_Position.x = -0.5f;
    m_Vtx[3].m_Position.y = -0.5f;
    m_Vtx[3].m_Position.z = 0.0f;
    m_Vtx[3].m_TexCoord.x = 0.0f;
    m_Vtx[3].m_TexCoord.y = 1.0f;

    GraphicBufferUsage usage = GraphicBufferUsage::Static;
    GraphicBufferAccess access = GraphicBufferAccess::None;

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
