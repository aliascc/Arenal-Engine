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
#include "GraphicDevice.h"
#include "SpotLightShape.h"
#include "Math\AEMathDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SpotLightShape::SpotLightShape(GraphicDevice* graphicDevice, uint32_t numEdges)
    : LightShape(graphicDevice)
    , m_NumEdges(numEdges)
{
}

SpotLightShape::~SpotLightShape()
{
    DeleteMemArr(m_InnerConeAndLinesVtxs);
    DeleteMemArr(m_NearFallOfAngleRingVtxs);
    DeleteMemArr(m_NearAttenuationRingVtxs);
    DeleteMemArr(m_FarFallOfAngleRingVtxs);
    DeleteMemArr(m_FarAttenuationRingVtxs);
}

AEResult SpotLightShape::BuildLightShape()
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
    DeleteMemArr(m_InnerConeAndLinesVtxs);
    DeleteMemArr(m_NearFallOfAngleRingVtxs);
    DeleteMemArr(m_NearAttenuationRingVtxs);
    DeleteMemArr(m_FarFallOfAngleRingVtxs);
    DeleteMemArr(m_FarAttenuationRingVtxs);

    ///////////////////////////////////////////////////
    //Variables 
    AEResult ret = AEResult::Ok;

    ///////////////////////////////////////////////////
    //Set Size of Inner Cone and Lines
    // It is, 1 Ring (Edges * 2) + 24 (12 * 2 Lines
    //  4 from Start to Inner Ring
    //  4 from Inner Ring to Near Attenuation and 
    //  4 from there to Far Attenuation
    m_SizeOfInnerCone = (m_NumEdges * 2) + 24;

    ///////////////////////////////////////////////////
    //Set Size for Rings
    m_SizeOfRings = m_NumEdges * 2;

    ///////////////////////////////////////////////////
    //Set Size in Vectors
    m_InnerConeAndLinesVtxs     = new VertexPositionColor[m_SizeOfInnerCone];
    m_NearFallOfAngleRingVtxs   = new VertexPositionColor[m_SizeOfRings];
    m_NearAttenuationRingVtxs   = new VertexPositionColor[m_SizeOfRings];
    m_FarFallOfAngleRingVtxs    = new VertexPositionColor[m_SizeOfRings];
    m_FarAttenuationRingVtxs    = new VertexPositionColor[m_SizeOfRings];

    memset(m_InnerConeAndLinesVtxs, 0, m_SizeOfInnerCone);
    memset(m_NearFallOfAngleRingVtxs, 0, m_SizeOfRings);
    memset(m_NearAttenuationRingVtxs, 0, m_SizeOfRings);
    memset(m_FarFallOfAngleRingVtxs, 0, m_SizeOfRings);
    memset(m_FarAttenuationRingVtxs, 0, m_SizeOfRings);

    ///////////////////////////////////////////////////
    //Create Vertex Buffers
    m_InnerConeAndLines      = new VertexBuffer<VertexPositionColor>(m_GraphicDevice, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
    m_NearFallOfAngleRing    = new VertexBuffer<VertexPositionColor>(m_GraphicDevice, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
    m_NearAttenuationRing    = new VertexBuffer<VertexPositionColor>(m_GraphicDevice, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
    m_FarFallOfAngleRing     = new VertexBuffer<VertexPositionColor>(m_GraphicDevice, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
    m_FarAttenuationRing     = new VertexBuffer<VertexPositionColor>(m_GraphicDevice, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);

    AETODO("Check return");
    m_InnerConeAndLines->CopyVertexBuffer(m_InnerConeAndLinesVtxs, m_SizeOfInnerCone);
    m_NearFallOfAngleRing->CopyVertexBuffer(m_NearFallOfAngleRingVtxs, m_SizeOfRings);
    m_NearAttenuationRing->CopyVertexBuffer(m_NearAttenuationRingVtxs, m_SizeOfRings);
    m_FarFallOfAngleRing->CopyVertexBuffer(m_FarFallOfAngleRingVtxs, m_SizeOfRings);
    m_FarAttenuationRing->CopyVertexBuffer(m_FarAttenuationRingVtxs, m_SizeOfRings);
    
    ///////////////////////////////////////////////////
    //Add to Vector
    m_VertexBufferVector.push_back(m_InnerConeAndLines);
    m_VertexBufferVector.push_back(m_NearFallOfAngleRing);
    m_VertexBufferVector.push_back(m_NearAttenuationRing);
    m_VertexBufferVector.push_back(m_FarFallOfAngleRing);
    m_VertexBufferVector.push_back(m_FarAttenuationRing);

    ///////////////////////////////////////////////////
    //Finish
    m_IsReady = true;

    return AEResult::Ok;
}

AEResult SpotLightShape::UpdateLightInfo(float nearAttenuation, float farAttenuation, float angle, float fallOfAngle)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    bool angleChanged = (angle != m_Angle);
    bool fallOffAngleChanged = (fallOfAngle != m_FallOffAngle);
    bool nearAttChanged = (nearAttenuation != m_NearAttenuation);
    bool farAttChanged = (farAttenuation != m_FarAttenuation);

    m_Angle = angle;
    m_FallOffAngle = fallOfAngle;
    m_NearAttenuation = nearAttenuation;
    m_FarAttenuation = farAttenuation;

    if (angleChanged || nearAttChanged)
    {
        UpdateNearAttenuationRing();

        AETODO("Check return");
        m_NearAttenuationRing->SetVertexData(0, m_NearAttenuationRingVtxs, 0, m_SizeOfRings, D3D11_MAP_WRITE_DISCARD);
    }

    if (angleChanged || farAttChanged)
    {
        UpdateFarAttenuationRing();

        AETODO("Check return");
        m_FarAttenuationRing->SetVertexData(0, m_FarAttenuationRingVtxs, 0, m_SizeOfRings, D3D11_MAP_WRITE_DISCARD);
    }

    if (fallOffAngleChanged || nearAttChanged)
    {
        UpdateNearFallOffAttenuationRing();

        AETODO("Check return");
        m_NearFallOfAngleRing->SetVertexData(0, m_NearFallOfAngleRingVtxs, 0, m_SizeOfRings, D3D11_MAP_WRITE_DISCARD);
    }

    if (fallOffAngleChanged || farAttChanged)
    {
        UpdateFarFallOffAttenuationRing();

        AETODO("Check return");
        m_FarFallOfAngleRing->SetVertexData(0, m_FarFallOfAngleRingVtxs, 0, m_SizeOfRings, D3D11_MAP_WRITE_DISCARD);
    }

    if (angleChanged || fallOffAngleChanged || farAttChanged || nearAttChanged)
    {
        UpdateInnerConeAndLines();

        AETODO("Check return");
        m_InnerConeAndLines->SetVertexData(0, m_InnerConeAndLinesVtxs, 0, m_SizeOfInnerCone, D3D11_MAP_WRITE_DISCARD);
    }

    return AEResult::Ok;
}

void SpotLightShape::UpdateInnerConeAndLines()
{
    //Formula to get Size
    // Size = 2 * distance * tan(angle / 2)
    float size = 2.0f * 1.0f * glm::tan(glm::radians(m_FallOffAngle / 2.0f));
    float nearSize = 2.0f * m_NearAttenuation * glm::tan(glm::radians(m_FallOffAngle / 2.0f));
    float farSize = 2.0f * m_FarAttenuation * glm::tan(glm::radians(m_FallOffAngle / 2.0f));

    UpdateRingInfo(m_InnerConeAndLinesVtxs, size, 1.0f, m_InnerCircleColor);

    ////////////////////////////////////////////////
    //Update Lines

    //Where to start setting lines
    uint32_t idxStart = m_SizeOfRings;

    ////////////////////////////////////////////////
    //4 from Start to inner ring
    glm::vec3 startPoints[4];
    glm::vec3 endPoints[4];

    ////////////////////////////////////////////////
    //Update from Start to Inner Circle
    startPoints[0] = startPoints[1] = startPoints[2] = startPoints[3] = AEMathHelpers::Vec3fZero;

    endPoints[0] = glm::vec3(size / 2.0f, 0.0f, 1.0f);
    endPoints[1] = glm::vec3(0.0f, -size / 2.0f, 1.0f);
    endPoints[2] = glm::vec3(0.0f, size / 2.0f, 1.0f);
    endPoints[3] = glm::vec3(-size / 2.0f, 0.0f, 1.0f);

    UpdateLines(idxStart, startPoints, endPoints, m_LineInnerColor);
    idxStart += 8;

    ////////////////////////////////////////////////
    //Update from Inner Circle to Near Fall Off Attenuation Ring
    startPoints[0] = endPoints[0];
    startPoints[1] = endPoints[1];
    startPoints[2] = endPoints[2];
    startPoints[3] = endPoints[3];

    endPoints[0] = glm::vec3(nearSize / 2.0f, 0.0f, m_NearAttenuation);
    endPoints[1] = glm::vec3(0.0f, -nearSize / 2.0f, m_NearAttenuation);
    endPoints[2] = glm::vec3(0.0f, nearSize / 2.0f, m_NearAttenuation);
    endPoints[3] = glm::vec3(-nearSize / 2.0f, 0.0f, m_NearAttenuation);

    UpdateLines(idxStart, startPoints, endPoints, m_LineNearColor);
    idxStart += 8;

    ////////////////////////////////////////////////
    //Update from Near Fall Off Attenuation Ring to Far Fall off Attenuation Ring
    startPoints[0] = endPoints[0];
    startPoints[1] = endPoints[1];
    startPoints[2] = endPoints[2];
    startPoints[3] = endPoints[3];

    endPoints[0] = glm::vec3(farSize / 2.0f, 0.0f, m_FarAttenuation);
    endPoints[1] = glm::vec3(0.0f, -farSize / 2.0f, m_FarAttenuation);
    endPoints[2] = glm::vec3(0.0f, farSize / 2.0f, m_FarAttenuation);
    endPoints[3] = glm::vec3(-farSize / 2.0f, 0.0f, m_FarAttenuation);

    UpdateLines(idxStart, startPoints, endPoints, m_LineFarColor);
}

void SpotLightShape::UpdateLines(uint32_t startIndex, glm::vec3 startPoints[4], glm::vec3 endPoints[4], Color color)
{
    for (uint32_t i = 0; i < 4; i++, startIndex += 2)
    {
        m_InnerConeAndLinesVtxs[startIndex].m_Position = startPoints[i];
        m_InnerConeAndLinesVtxs[startIndex].m_Color = color;

        m_InnerConeAndLinesVtxs[startIndex + 1].m_Position = endPoints[i];
        m_InnerConeAndLinesVtxs[startIndex + 1].m_Color = color;
    }
}

void SpotLightShape::UpdateNearAttenuationRing()
{
    //Formula to get Size
    // Size = 2 * distance * tan(angle / 2)
    float size = 2.0f * m_NearAttenuation * glm::tan(glm::radians(m_Angle / 2.0f));

    UpdateRingInfo(m_NearAttenuationRingVtxs, size, m_NearAttenuation, m_NearAttColor);
}

void SpotLightShape::UpdateNearFallOffAttenuationRing()
{
    //Formula to get Size
    // Size = 2 * distance * tan(angle / 2)
    float size = 2.0f * m_NearAttenuation * glm::tan(glm::radians(m_FallOffAngle / 2.0f));

    UpdateRingInfo(m_NearFallOfAngleRingVtxs, size, m_NearAttenuation, m_NearAttColor);
}

void SpotLightShape::UpdateFarAttenuationRing()
{
    //Formula to get Size
    // Size = 2 * distance * tan(angle)
    float size = 2.0f * m_FarAttenuation * glm::tan(glm::radians(m_Angle / 2.0f));

    UpdateRingInfo(m_FarAttenuationRingVtxs, size, m_FarAttenuation, m_FarAttColor);
}

void SpotLightShape::UpdateFarFallOffAttenuationRing()
{
    //Formula to get Size
    // Size = 2 * distance * tan(angle / 2)
    float size = 2.0f * m_FarAttenuation * glm::tan(glm::radians(m_FallOffAngle / 2.0f));

    UpdateRingInfo(m_FarFallOfAngleRingVtxs, size, m_FarAttenuation, m_FarAttColor);
}

void SpotLightShape::UpdateRingInfo(VertexPositionColor vtxVector[], float size, float distance, Color color)
{
    AEAssert(vtxVector != nullptr);
    if (vtxVector == nullptr)
    {
        return;
    }

    ///////////////////////////////////////////////////
    //Init variable info
    float edgesFloat = static_cast<float>(m_NumEdges);
    float edgesHalf = edgesFloat / 2.0f;
    float radius = size / 2.0f;

    float radiusRingRad = glm::radians(180.0f * edgesHalf) / edgesFloat;
    float radiusRing = radius * glm::sin(radiusRingRad);

    ///////////////////////////////////////////////////
    //Construct circle
    uint32_t indexVtx = 0;
    for (float i = 0; i < m_NumEdges; i++, indexVtx += 2)
    {
        //Calculate Point A
        vtxVector[indexVtx].m_Position.x = radiusRing * glm::cos(glm::radians((360.0f * i) / edgesFloat));
        vtxVector[indexVtx].m_Position.z = radius * glm::cos(glm::radians(180.0f * edgesHalf) / edgesFloat);
        vtxVector[indexVtx].m_Position.y = radiusRing * glm::sin(glm::radians((360.0f * i) / edgesFloat));
        vtxVector[indexVtx].m_Color = color;

        //Calculate Point B
        vtxVector[indexVtx + 1].m_Position.x = radiusRing * glm::cos(glm::radians((360.0f * (i + 1)) / edgesFloat));
        vtxVector[indexVtx + 1].m_Position.z = radius * glm::cos(glm::radians(180.0f * edgesHalf) / edgesFloat);
        vtxVector[indexVtx + 1].m_Position.y = radiusRing * glm::sin(glm::radians((360.0f * (i + 1)) / edgesFloat));
        vtxVector[indexVtx + 1].m_Color = color;

        vtxVector[indexVtx].m_Position.z += distance;
        vtxVector[indexVtx + 1].m_Position.z += distance;
    }
}
