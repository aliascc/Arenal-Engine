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
#include "SphereShape.h"
#include "GraphicDevice.h"
#include "Models\MeshPart.h"
#include "Vertex\IndexBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AETODO("Check if we still need this, if we are generating meshes another way");
SphereShape::SphereShape(GraphicDevice& graphicDevice, uint32_t recursionLevels, const Color& color, bool clockWise)
    : m_RecursionLevels(recursionLevels)
    , m_Color(color)
    , m_ClockWise(clockWise)
    , m_GraphicDevice(graphicDevice)
{
    BuildSphere();
}

SphereShape::~SphereShape()
{
    DeleteMem(m_IB);
    DeleteMem(m_VB);
}

AEResult SphereShape::CreateSphere(uint32_t recursionLevels, const Color& color, bool clockWise)
{
    m_RecursionLevels = recursionLevels;
    m_Color = color;
    m_ClockWise = clockWise;

    return BuildSphere();
}

uint16_t SphereShape::AddVtx(VertexPositionColor& vtx)
{
    glm::vec3 vect(vtx.m_Position.x, vtx.m_Position.y, vtx.m_Position.z);
    vect = glm::normalize(vect);

    vtx.m_Position.x = vect.x;
    vtx.m_Position.y = vect.y;
    vtx.m_Position.z = vect.z;

    std::string vtxStr = AEVertexHelper::GetVertexString(vtx);
    uint16_t idx = 0;

    if(m_VtxMap.find(vtxStr) != m_VtxMap.end())
    {
        idx = m_VtxMap[vtxStr];
    }
    else
    {
        idx = (uint16_t)m_VtxBuff.size();
        m_VtxMap[vtxStr] = (uint16_t)m_VtxBuff.size();
        m_VtxBuff.push_back(vtx);
    }

    return idx;
}

void SphereShape::AddIdx(uint16_t idx)
{
    m_IdxBuff.push_back(idx);
}

uint16_t SphereShape::GetMiddlePoint(uint32_t vtxIndex1, uint32_t vtxIndex2)
{
    glm::vec3 point1(m_VtxBuff[vtxIndex1].m_Position.x, m_VtxBuff[vtxIndex1].m_Position.y, m_VtxBuff[vtxIndex1].m_Position.z);
    glm::vec3 point2(m_VtxBuff[vtxIndex2].m_Position.x, m_VtxBuff[vtxIndex2].m_Position.y, m_VtxBuff[vtxIndex2].m_Position.z);
    
    glm::vec3 middle(
        (point1.x + point2.x) / 2.0f,
        (point1.y + point2.y) / 2.0f,
        (point1.z + point2.z) / 2.0f
        );

    VertexPositionColor newVtx;
    newVtx.m_Position.x = middle.x;
    newVtx.m_Position.y = middle.y;
    newVtx.m_Position.z = middle.z;
    newVtx.m_Color      = m_Color;

    //Add vertex makes sure point is on unit sphere
    uint16_t idx = AddVtx(newVtx); 

    return idx;
}

AEResult SphereShape::BuildSphere()
{
    //Delete Old IB and VB
    DeleteMem(m_IB);
    DeleteMem(m_VB);

    //Set Ready Flag to False
    m_IsReady = false;

    //Clear Maps
    m_VtxMap.clear();
    m_IdxBuff.clear();
    m_VtxBuff.clear();

    //Get Base Size
    float t = (1.0f + sqrt(5.0f)) / 2.0f;

    VertexPositionColor vtx;
    vtx.m_Color = m_Color;
    
    //////////////////////////////////////////
    //Create 12 vertices of a icosahedron
    //////////////////////////////////////////

    //Vertice 1
    vtx.m_Position.x = -1;
    vtx.m_Position.y = t;
    vtx.m_Position.z = 0;
    AddVtx(vtx);

    //Vertice 2
    vtx.m_Position.x = 1;
    vtx.m_Position.y = t;
    vtx.m_Position.z = 0;
    AddVtx(vtx);

    //Vertice 3
    vtx.m_Position.x = -1;
    vtx.m_Position.y = -t;
    vtx.m_Position.z = 0;
    AddVtx(vtx);
    
    //Vertice 4
    vtx.m_Position.x = 1;
    vtx.m_Position.y = -t;
    vtx.m_Position.z = 0;
    AddVtx(vtx);
    
    //Vertice 5
    vtx.m_Position.x = 0;
    vtx.m_Position.y = -1;
    vtx.m_Position.z = t;
    AddVtx(vtx);

    //Vertice 6
    vtx.m_Position.x = 0;
    vtx.m_Position.y = 1;
    vtx.m_Position.z = t;
    AddVtx(vtx);

    //Vertice 7
    vtx.m_Position.x = 0;
    vtx.m_Position.y = -1;
    vtx.m_Position.z = -t;
    AddVtx(vtx);

    //Vertice 8
    vtx.m_Position.x = 0;
    vtx.m_Position.y = 1;
    vtx.m_Position.z = -t;
    AddVtx(vtx);

    //Vertice 9
    vtx.m_Position.x = t;
    vtx.m_Position.y = 0;
    vtx.m_Position.z = -1;
    AddVtx(vtx);
    
    //Vertice 10
    vtx.m_Position.x = t;
    vtx.m_Position.y = 0;
    vtx.m_Position.z = 1;
    AddVtx(vtx);

    //Vertice 11
    vtx.m_Position.x = -t;
    vtx.m_Position.y = 0;
    vtx.m_Position.z = -1;
    AddVtx(vtx);

    //Vertice 12
    vtx.m_Position.x = -t;
    vtx.m_Position.y = 0;
    vtx.m_Position.z = 1;
    AddVtx(vtx);
    
    //////////////////////////
    //5 faces around point 0
    //////////////////////////

    //Face 1
    AddIdx(0);
    AddIdx(11);
    AddIdx(5);
    
    //Face 2
    AddIdx(0);
    AddIdx(5);
    AddIdx(1);
    
    //Face 3
    AddIdx(0);
    AddIdx(1);
    AddIdx(7);
    
    //Face 4
    AddIdx(0);
    AddIdx(7);
    AddIdx(10);
    
    //Face 5
    AddIdx(0);
    AddIdx(10);
    AddIdx(11);
    
    //////////////////////////
    //5 adjacent faces 
    //////////////////////////
    
    //Face 1
    AddIdx(1);
    AddIdx(5);
    AddIdx(9);
    
    //Face 2
    AddIdx(5);
    AddIdx(11);
    AddIdx(4);
    
    //Face 3
    AddIdx(11);
    AddIdx(10);
    AddIdx(2);
    
    //Face 4
    AddIdx(10);
    AddIdx(7);
    AddIdx(6);
    
    //Face 5
    AddIdx(7);
    AddIdx(1);
    AddIdx(8);

    //////////////////////////
    //5 faces around point 3
    //////////////////////////
    
    //Face 1
    AddIdx(3);
    AddIdx(9);
    AddIdx(4);
    
    //Face 2
    AddIdx(3);
    AddIdx(4);
    AddIdx(2);
    
    //Face 3
    AddIdx(3);
    AddIdx(2);
    AddIdx(6);
    
    //Face 4
    AddIdx(3);
    AddIdx(6);
    AddIdx(8);
    
    //Face 5
    AddIdx(3);
    AddIdx(8);
    AddIdx(9);
    

    //////////////////////////
    //5 adjacent faces 
    //////////////////////////

    //Face 1
    AddIdx(4);
    AddIdx(9);
    AddIdx(5);
    
    //Face 2
    AddIdx(2);
    AddIdx(4);
    AddIdx(11);
    
    //Face 3
    AddIdx(6);
    AddIdx(2);
    AddIdx(10);
    
    //Face 4
    AddIdx(8);
    AddIdx(6);
    AddIdx(7);
    
    //Face 5
    AddIdx(9);
    AddIdx(8);
    AddIdx(1);

    for(uint32_t i = 0; i < m_RecursionLevels; ++i)
    {
        std::vector<uint16_t> idxBuf2;

        uint32_t oldIndexBufSize = (uint32_t)m_IdxBuff.size();

        for(uint32_t j = 0; j < oldIndexBufSize; j += 3)
        {
            //Replace triangle by 4 triangles
            uint16_t a = GetMiddlePoint(m_IdxBuff[j], m_IdxBuff[j + 1]);
            uint16_t b = GetMiddlePoint(m_IdxBuff[j + 1], m_IdxBuff[j + 2]);
            uint16_t c = GetMiddlePoint(m_IdxBuff[j + 2], m_IdxBuff[j]);

            //New Face 1
            idxBuf2.push_back(m_IdxBuff[j]);
            idxBuf2.push_back(a);
            idxBuf2.push_back(c);
            
            //New Face 2
            idxBuf2.push_back(m_IdxBuff[j + 1]);
            idxBuf2.push_back(b);
            idxBuf2.push_back(a);
            
            //New Face 3
            idxBuf2.push_back(m_IdxBuff[j + 2]);
            idxBuf2.push_back(c);
            idxBuf2.push_back(b);
           
            //New Face 4
            idxBuf2.push_back(a);
            idxBuf2.push_back(b);
            idxBuf2.push_back(c);
        }

        m_IdxBuff = idxBuf2;
    }
    
    //Create VB
    m_VB = new VertexBuffer<VertexPositionColor>(m_GraphicDevice);
    m_VB->CopyVertexBuffer(m_VtxBuff);
    m_VB->BuildVertexBuffer();

    //Create IB
    m_IB = new IndexBuffer(m_GraphicDevice);
    m_IB->CopyToIndexBuffer(m_IdxBuff);
    m_IB->BuildIndexBuffer();

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult SphereShape::DrawSphere()
{
    if(!m_IsReady)
    {
        return AEResult::Fail;
    }

    m_GraphicDevice.SetVertexBuffer(m_VB);

    m_GraphicDevice.SetIndexBuffer(m_IB);

    m_GraphicDevice.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_GraphicDevice.DrawIndexed(0, 0, m_IB->GetSize());

    return AEResult::Ok;
}
