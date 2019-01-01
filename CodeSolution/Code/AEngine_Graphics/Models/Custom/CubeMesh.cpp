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
#include "CubeMesh.h"
#include "GraphicDevice.h"
#include "GameContentDefs.h"
#include "Models\MeshPart.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\VertexBuffer.h"
#include "Content\ContentDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
CubeMesh::CubeMesh(GraphicDevice& graphicDevice, const std::string& resourceName)
    : Mesh(graphicDevice, resourceName)
{
}

CubeMesh::~CubeMesh()
{
}

AEResult CubeMesh::Load()
{
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    if (m_IsLoaded)
    {
        return AEResult::Ok;
    }

    /////////////////////////////////////////////
    //Clean Up memory before loading File
    CleanUp();

    /////////////////////////////////////////////
    //Create the Vertexes for the Cube and add 
    //indexes to the buffer
    VertexPositionNormalTexture vtx;
    memset(&vtx, 0, sizeof(VertexPositionNormalTexture));

    ///
    ///      ------------
    ///     /|          /|
    ///    / |         / |
    ///   /  |        /  |
    ///   ------------   |
    ///   |  --------|--/
    ///   | /        | /
    ///   |/         |/
    ///   ------------
    ///

    //////////////////////////////////////
    //Front

    //Top Left
    vtx.m_Position = glm::vec3(0.5f, 0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 0.0f);
    uint16_t idxFTL = AddVtx(vtx);

    //Top Right
    vtx.m_Position = glm::vec3(-0.5f, 0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 0.0f);
    uint16_t idxFTR = AddVtx(vtx);

    //Bottom Right
    vtx.m_Position = glm::vec3(-0.5f, -0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 1.0f);
    uint16_t idxFBR = AddVtx(vtx);

    //Bottom Left
    vtx.m_Position = glm::vec3(0.5f, -0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 1.0f);
    uint16_t idxFBL = AddVtx(vtx);

    //////////////////////////////////////
    //Right

    //Top Left
    vtx.m_Position = glm::vec3(-0.5f, 0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 0.0f);
    uint16_t idxRTL = AddVtx(vtx);

    //Top Right
    vtx.m_Position = glm::vec3(-0.5f, 0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 0.0f);
    uint16_t idxRTR = AddVtx(vtx);

    //Bottom Right
    vtx.m_Position = glm::vec3(-0.5f, -0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 1.0f);
    uint16_t idxRBR = AddVtx(vtx);

    //Bottom Left
    vtx.m_Position = glm::vec3(-0.5f, -0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 1.0f);
    uint16_t idxRBL = AddVtx(vtx);

    //////////////////////////////////////
    //Rear

    //Top Left
    vtx.m_Position = glm::vec3(-0.5f, 0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 0.0f);
    uint16_t idxRETL = AddVtx(vtx);

    //Top Right
    vtx.m_Position = glm::vec3(0.5f, 0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 0.0f);
    uint16_t idxRETR = AddVtx(vtx);

    //Bottom Right
    vtx.m_Position = glm::vec3(0.5f, -0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 1.0f);
    uint16_t idxREBR = AddVtx(vtx);

    //Bottom Left
    vtx.m_Position = glm::vec3(-0.5f, -0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 1.0f);
    uint16_t idxREBL = AddVtx(vtx);

    //////////////////////////////////////
    //Left

    //Top Left
    vtx.m_Position = glm::vec3(0.5f, 0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 0.0f);
    uint16_t idxLTL = AddVtx(vtx);

    //Top Right
    vtx.m_Position = glm::vec3(0.5f, 0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 0.0f);
    uint16_t idxLTR = AddVtx(vtx);

    //Bottom Right
    vtx.m_Position = glm::vec3(0.5f, -0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 1.0f);
    uint16_t idxLBR = AddVtx(vtx);

    //Bottom Left
    vtx.m_Position = glm::vec3(0.5f, -0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 1.0f);
    uint16_t idxLBL = AddVtx(vtx);

    //////////////////////////////////////
    //Top

    //Top Left
    vtx.m_Position = glm::vec3(0.5f, 0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 0.0f);
    uint16_t idxTTL = AddVtx(vtx);

    //Top Right
    vtx.m_Position = glm::vec3(-0.5f, 0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 0.0f);
    uint16_t idxTTR = AddVtx(vtx);

    //Bottom Right
    vtx.m_Position = glm::vec3(-0.5f, 0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 1.0f);
    uint16_t idxTBR = AddVtx(vtx);

    //Bottom Left
    vtx.m_Position = glm::vec3(0.5f, 0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 1.0f);
    uint16_t idxTBL = AddVtx(vtx);

    //////////////////////////////////////
    //Bottom

    //Top Left
    vtx.m_Position = glm::vec3(0.5f, -0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 0.0f);
    uint16_t idxBTL = AddVtx(vtx);

    //Top Right
    vtx.m_Position = glm::vec3(-0.5f, -0.5f, -0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 0.0f);
    uint16_t idxBTR = AddVtx(vtx);

    //Bottom Right
    vtx.m_Position = glm::vec3(-0.5f, -0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(1.0f, 1.0f);
    uint16_t idxBBR = AddVtx(vtx);

    //Bottom Left
    vtx.m_Position = glm::vec3(0.5f, -0.5f, 0.5f);
    vtx.m_TexCoord = glm::vec2(0.0f, 1.0f);
    uint16_t idxBBL = AddVtx(vtx);

    /////////////////////////////////////////////
    //Push Indexes into buffer to create cube

    //////////////////////////////////////
    //Front

    //1st Triangle
    m_IdxBuff.push_back(idxFTL);
    m_IdxBuff.push_back(idxFTR);
    m_IdxBuff.push_back(idxFBR);

    //2st Triangle
    m_IdxBuff.push_back(idxFBR);
    m_IdxBuff.push_back(idxFBL);
    m_IdxBuff.push_back(idxFTL);

    //////////////////////////////////////
    //Right

    //1st Triangle
    m_IdxBuff.push_back(idxRTL);
    m_IdxBuff.push_back(idxRTR);
    m_IdxBuff.push_back(idxRBR);

    //2st Triangle
    m_IdxBuff.push_back(idxRBR);
    m_IdxBuff.push_back(idxRBL);
    m_IdxBuff.push_back(idxRTL);

    //////////////////////////////////////
    //Rear

    //1st Triangle
    m_IdxBuff.push_back(idxRETL);
    m_IdxBuff.push_back(idxRETR);
    m_IdxBuff.push_back(idxREBR);

    //2st Triangle
    m_IdxBuff.push_back(idxREBR);
    m_IdxBuff.push_back(idxREBL);
    m_IdxBuff.push_back(idxRETL);

    //////////////////////////////////////
    //Left

    //1st Triangle
    m_IdxBuff.push_back(idxLTL);
    m_IdxBuff.push_back(idxLTR);
    m_IdxBuff.push_back(idxLBR);

    //2st Triangle
    m_IdxBuff.push_back(idxLBR);
    m_IdxBuff.push_back(idxLBL);
    m_IdxBuff.push_back(idxLTL);

    //////////////////////////////////////
    //Top

    //1st Triangle
    m_IdxBuff.push_back(idxTTL);
    m_IdxBuff.push_back(idxTTR);
    m_IdxBuff.push_back(idxTBR);

    //2st Triangle
    m_IdxBuff.push_back(idxTBR);
    m_IdxBuff.push_back(idxTBL);
    m_IdxBuff.push_back(idxTTL);

    //////////////////////////////////////
    //Bottom

    //1st Triangle
    m_IdxBuff.push_back(idxBTL);
    m_IdxBuff.push_back(idxBTR);
    m_IdxBuff.push_back(idxBBR);

    //2st Triangle
    m_IdxBuff.push_back(idxBBR);
    m_IdxBuff.push_back(idxBBL);
    m_IdxBuff.push_back(idxBTL);

    /////////////////////////////////////////////
    //Create Vertex Buffer and Index Buffer Objects

    //Vertex Buffer
    VertexBuffer<VertexPositionNormalTexture>* vertexBuffer = new VertexBuffer<VertexPositionNormalTexture>(m_GraphicDevice);

    AETODO("Check return");
    vertexBuffer->CopyVertexBuffer(m_VtxBuff);
    vertexBuffer->BuildVertexBuffer();

    //Index Buffer
    IndexBuffer* indexBuffer = new IndexBuffer(m_GraphicDevice);

    AETODO("Check return");
    indexBuffer->CopyToIndexBuffer(m_IdxBuff);
    indexBuffer->BuildIndexBuffer();

    /////////////////////////////////////////////
    //Add variables to Mesh Part
    MeshPart* meshPart = new MeshPart();

    meshPart->SetIndexBuffer(indexBuffer);
    meshPart->SetVertexBuffer(vertexBuffer);

    m_MeshPartVector.push_back(meshPart);

    /////////////////////////////////////////////
    //Set name
    SetNameWithoutLock(AE_CUBE_MESH_NAME);

    //////////////////////////////////////
    //Set Bounding Shapes
    m_BoundingBox.m_MinPos        = glm::vec3(-0.5f, -0.5f, -0.5f);
    m_BoundingBox.m_MaxPos        = glm::vec3(0.5f, 0.5f, 0.5f);

    m_BoundingSphere.m_Radius    = 0.5f;

    /////////////////////////////////////////////
    //Finish
    m_IsLoaded = true;

    return AEResult::Ok;
}

uint16_t CubeMesh::AddVtx(const VertexPositionNormalTexture& vtx)
{
    uint16_t idx = 0;
    std::string vtxStr = AEVertexHelper::GetVertexString(vtx);

    if (m_VtxMap.find(vtxStr) != m_VtxMap.end())
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
