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
#include "Math\AEMathDefs.h"
#include "Base\BaseFunctions.h"
#include "Vertex\VertexBuffer.h"
#include "DirectionalLightShape.h"
#include "Vertex\Types\VertexPosition.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DirectionalLightShape::DirectionalLightShape(GraphicDevice* graphicDevice)
    : LightShape(graphicDevice)
{
}

DirectionalLightShape::~DirectionalLightShape()
{
}

AEResult DirectionalLightShape::BuildLightShape()
{
    ///////////////////////////////////////////////////
    //Pre-checks
    AEAssert(m_GraphicDevice != nullptr);
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    ///////////////////////////////////////////////////
    //Delete Current and set ready to false
    m_IsReady = false;
    CleanUp();

    ///////////////////////////////////////////////////
    //Variables 

    //Size if Calculate as 4 Lines for Arrow Tip and 1 for Arrow Line
    //For total of 10 Vertexes
    const uint32_t sizeOfVtx = 10;

    VertexPosition vtxs[sizeOfVtx];
    memset(vtxs, 0, sizeof(VertexPosition)* sizeOfVtx);

    ///////////////////////////////////////////////////
    //Init Vertexs

    //Arrow Line
    vtxs[0].m_Position = AEMathHelpers::Vec3fZero;
    vtxs[1].m_Position = glm::vec3(0.0f, 0.0f, 1.0f);

    //Arrow Tip

    // #1
    vtxs[2].m_Position = glm::vec3(0.0f, 0.0f, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
    vtxs[3].m_Position = glm::vec3(0.0f, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

    // #2
    vtxs[4].m_Position = glm::vec3(0.0f, 0.0f, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
    vtxs[5].m_Position = glm::vec3(0.0f, -AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

    // #3
    vtxs[6].m_Position = glm::vec3(0.0f, 0.0f, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
    vtxs[7].m_Position = glm::vec3(AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, 0.0f, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

    // #4
    vtxs[8].m_Position = glm::vec3(0.0f, 0.0f, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
    vtxs[9].m_Position = glm::vec3(-AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, 0.0f, AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

    ///////////////////////////////////////////////////
    //Create Vertex Buffer
    VertexBuffer<VertexPosition>* vertexBuffer = new VertexBuffer<VertexPosition>(m_GraphicDevice);

    if (vertexBuffer->CopyVertexBuffer(vtxs, sizeOfVtx) != AEResult::Ok)
    {
        AETODO("Better return message");
        return AEResult::Fail;
    }

    ///////////////////////////////////////////////////
    //Add to Vector
    m_VertexBufferVector.push_back(vertexBuffer);

    ///////////////////////////////////////////////////
    //Finish
    m_IsReady = true;

    return AEResult::Ok;
}
