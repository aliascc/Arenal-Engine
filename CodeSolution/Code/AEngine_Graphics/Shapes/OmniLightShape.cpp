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
#include "GraphicDevice.h"
#include "OmniLightShape.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPosition.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
OmniLightShape::OmniLightShape(GraphicDevice* graphicDevice, uint32_t numEdges)
    : LightShape(graphicDevice)
    , m_NumEdges(numEdges)
{
}

OmniLightShape::~OmniLightShape()
{
}

AEResult OmniLightShape::BuildLightShape()
{
    ///////////////////////////////////////////////////
    //Pre-checks
    AEAssert(m_GraphicDevice != nullptr);
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    AEAssert(m_NumEdges != 0);
    if (m_NumEdges == 0)
    {
        return AEResult::ZeroSize;
    }

    ///////////////////////////////////////////////////
    //Delete Current and set ready to false
    m_IsReady = false;
    CleanUp();

    ///////////////////////////////////////////////////
    //Variables 
    AEResult ret = AEResult::Ok;
    uint32_t sizeOfVtx = m_NumEdges * 2;

    VertexPosition* vtxH = new VertexPosition[sizeOfVtx];
    VertexPosition* vtxV = new VertexPosition[sizeOfVtx];

    memset(vtxH, 0, sizeof(VertexPosition) * sizeOfVtx);
    memset(vtxV, 0, sizeof(VertexPosition) * sizeOfVtx);

    ///////////////////////////////////////////////////
    //Init variable info
    float edgesFloat = static_cast<float>(m_NumEdges);
    float edgesHalf = edgesFloat / 2.0f;
    float radius = m_Size / 2.0f;

    float radiusRingRad = glm::radians(180.0f * edgesHalf) / edgesFloat;
    float radiusRing = radius * glm::sin(radiusRingRad);

    ///////////////////////////////////////////////////
    //Construct circle
    uint32_t indexVtx = 0;
    for (float i = 0; i < m_NumEdges; i++, indexVtx += 2)
    {        
        //Calculate Point A
        vtxV[indexVtx].m_Position.x = vtxH[indexVtx].m_Position.x = radiusRing * glm::cos(glm::radians((360.0f * i) / edgesFloat));
        vtxV[indexVtx].m_Position.z = vtxH[indexVtx].m_Position.y = radius * glm::cos(glm::radians(180.0f * edgesHalf) / edgesFloat);
        vtxV[indexVtx].m_Position.y = vtxH[indexVtx].m_Position.z = radiusRing * glm::sin(glm::radians((360.0f * i) / edgesFloat));

        //Calculate Point B
        vtxV[indexVtx + 1].m_Position.x = vtxH[indexVtx + 1].m_Position.x = radiusRing * glm::cos(glm::radians((360.0f * (i + 1)) / edgesFloat));
        vtxV[indexVtx + 1].m_Position.z = vtxH[indexVtx + 1].m_Position.y = radius * glm::cos(glm::radians(180.0f * edgesHalf) / edgesFloat);
        vtxV[indexVtx + 1].m_Position.y = vtxH[indexVtx + 1].m_Position.z = radiusRing * glm::sin(glm::radians((360.0f * (i + 1)) / edgesFloat));
    }

    ///////////////////////////////////////////////////
    //Create Vertex Buffers
    VertexBuffer<VertexPosition>* vertexBufferHorizontal = new VertexBuffer<VertexPosition>(m_GraphicDevice);
    VertexBuffer<VertexPosition>* vertexBufferVertical = new VertexBuffer<VertexPosition>(m_GraphicDevice);

    if (vertexBufferHorizontal->CopyVertexBuffer(vtxH, sizeOfVtx) != AEResult::Ok || vertexBufferVertical->CopyVertexBuffer(vtxV, sizeOfVtx) != AEResult::Ok)
    {
        AETODO("Better return message");
        ret = AEResult::Fail;
    }

    DeleteMemArr(vtxH);
    DeleteMemArr(vtxV);

    if (ret != AEResult::Ok)
    {
        DeleteMem(vertexBufferHorizontal);
        DeleteMem(vertexBufferVertical);
        return ret;
    }

    ///////////////////////////////////////////////////
    //Add to Vector
    m_VertexBufferVector.push_back(vertexBufferHorizontal);
    m_VertexBufferVector.push_back(vertexBufferVertical);

    ///////////////////////////////////////////////////
    //Finish
    m_IsReady = true;

    return AEResult::Ok;
}
