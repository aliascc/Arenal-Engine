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
#include "FrustumShape.h"
#include "Utils\Frustum.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
FrustumShape::FrustumShape(GraphicDevice* graphicDevice)
    : m_GraphicDevice(graphicDevice)
{
    AEAssert(m_GraphicDevice != nullptr);

    memset(m_Vtx, 0, sizeof(VertexPositionColor) * 24);
}

FrustumShape::~FrustumShape()
{
    DeleteMem(m_VB);
}

AEResult FrustumShape::BuildFrustumShape()
{
    DeleteMem(m_VB);
    m_IsReady = false;

    memset(m_Vtx, 0, sizeof(VertexPositionColor) * AE_FRUSTUM_SHAPE_SIZE);

    //Near Plane
    m_Vtx[0].m_Color = AEColors::Red;
    m_Vtx[1].m_Color = AEColors::Red;
    m_Vtx[2].m_Color = AEColors::Red;
    m_Vtx[3].m_Color = AEColors::Red;
    m_Vtx[4].m_Color = AEColors::Red;
    m_Vtx[5].m_Color = AEColors::Red;
    m_Vtx[6].m_Color = AEColors::Red;
    m_Vtx[7].m_Color = AEColors::Red;

    //Far Plane
    m_Vtx[8].m_Color = AEColors::Cyan;
    m_Vtx[9].m_Color = AEColors::Cyan;
    m_Vtx[10].m_Color = AEColors::Cyan;
    m_Vtx[11].m_Color = AEColors::Cyan;
    m_Vtx[12].m_Color = AEColors::Cyan;
    m_Vtx[13].m_Color = AEColors::Cyan;
    m_Vtx[14].m_Color = AEColors::Cyan;
    m_Vtx[15].m_Color = AEColors::Cyan;

    //Left Plane
    m_Vtx[16].m_Color = AEColors::White;
    m_Vtx[17].m_Color = AEColors::White;
    m_Vtx[18].m_Color = AEColors::White;
    m_Vtx[19].m_Color = AEColors::White;

    //Right Plane
    m_Vtx[20].m_Color = AEColors::White;
    m_Vtx[21].m_Color = AEColors::White;
    m_Vtx[22].m_Color = AEColors::White;
    m_Vtx[23].m_Color = AEColors::White;

    for (uint32_t x = 0, i = 24; x < 19; ++x)
    {
        //Near Plane
        m_Vtx[i].m_Color        = AEColors::Red;
        m_Vtx[i + 1].m_Color    = AEColors::Red;
        i += 2;

        //Far Plane
        m_Vtx[i].m_Color        = AEColors::Cyan;
        m_Vtx[i + 1].m_Color    = AEColors::Cyan;
        i += 2;

        //Right Plane
        m_Vtx[i].m_Color        = AEColors::White;
        m_Vtx[i + 1].m_Color    = AEColors::White;
        i += 2;

        //Left Plane
        m_Vtx[i].m_Color        = AEColors::White;
        m_Vtx[i + 1].m_Color    = AEColors::White;
        i += 2;

        //Top
        m_Vtx[i].m_Color        = AEColors::Yellow;
        m_Vtx[i + 1].m_Color    = AEColors::Yellow;
        i += 2;

        //Bottom
        m_Vtx[i].m_Color        = AEColors::Yellow;
        m_Vtx[i + 1].m_Color    = AEColors::Yellow;
        i += 2;
    }

    m_VB = new VertexBuffer<VertexPositionColor>(m_GraphicDevice, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
    m_VB->CopyVertexBuffer(m_Vtx, AE_FRUSTUM_SHAPE_SIZE);
    m_VB->BuildVertexBuffer();

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult FrustumShape::UpdateFrustumShapeVtx(const Frustum* frustum)
{
    AEAssert(frustum != nullptr);
    if (frustum == nullptr)
    {
        return AEResult::NullObj;
    }

    const FrustumCorners& corners = frustum->GetCorners();

    //Near Plane
    m_Vtx[0].m_Position = corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT];
    m_Vtx[1].m_Position = corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT];
    m_Vtx[2].m_Position = corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT];
    m_Vtx[3].m_Position = corners[AE_FRUSTUM_NEAR_TOP_RIGHT];
    m_Vtx[4].m_Position = corners[AE_FRUSTUM_NEAR_TOP_RIGHT];
    m_Vtx[5].m_Position = corners[AE_FRUSTUM_NEAR_TOP_LEFT];
    m_Vtx[6].m_Position = corners[AE_FRUSTUM_NEAR_TOP_LEFT];
    m_Vtx[7].m_Position = corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT];

    //Far Plane
    m_Vtx[8].m_Position = corners[AE_FRUSTUM_FAR_BOTTOM_LEFT];
    m_Vtx[9].m_Position = corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT];
    m_Vtx[10].m_Position = corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT];
    m_Vtx[11].m_Position = corners[AE_FRUSTUM_FAR_TOP_RIGHT];
    m_Vtx[12].m_Position = corners[AE_FRUSTUM_FAR_TOP_RIGHT];
    m_Vtx[13].m_Position = corners[AE_FRUSTUM_FAR_TOP_LEFT];
    m_Vtx[14].m_Position = corners[AE_FRUSTUM_FAR_TOP_LEFT];
    m_Vtx[15].m_Position = corners[AE_FRUSTUM_FAR_BOTTOM_LEFT];

    //Left Plane
    m_Vtx[16].m_Position = corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT];
    m_Vtx[17].m_Position = corners[AE_FRUSTUM_FAR_BOTTOM_LEFT];
    m_Vtx[18].m_Position = corners[AE_FRUSTUM_NEAR_TOP_LEFT];
    m_Vtx[19].m_Position = corners[AE_FRUSTUM_FAR_TOP_LEFT];

    //Right Plane
    m_Vtx[20].m_Position = corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT];
    m_Vtx[21].m_Position = corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT];
    m_Vtx[22].m_Position = corners[AE_FRUSTUM_NEAR_TOP_RIGHT];
    m_Vtx[23].m_Position = corners[AE_FRUSTUM_FAR_TOP_RIGHT];

    for (uint32_t x = 0, i = 24; x < 19; ++x)
    {
        //NearPlane
        m_Vtx[i].m_Position.y        = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].y,       corners[AE_FRUSTUM_NEAR_TOP_LEFT].y,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.y    = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].y,    corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].y,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.x        = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].x,       corners[AE_FRUSTUM_NEAR_TOP_LEFT].x,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.x    = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].x,    corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].x,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.z        = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].z,       corners[AE_FRUSTUM_NEAR_TOP_LEFT].z,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.z    = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].z,    corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].z,    ((float)x + 1.0f) / 20.0f);

        i += 2;

        //FarPlane
        m_Vtx[i].m_Position.y        = glm::lerp(corners[AE_FRUSTUM_FAR_TOP_RIGHT].y,        corners[AE_FRUSTUM_FAR_TOP_LEFT].y,        ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.y    = glm::lerp(corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].y,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].y,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.x        = glm::lerp(corners[AE_FRUSTUM_FAR_TOP_RIGHT].x,        corners[AE_FRUSTUM_FAR_TOP_LEFT].x,        ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.x    = glm::lerp(corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].x,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].x,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.z        = glm::lerp(corners[AE_FRUSTUM_FAR_TOP_RIGHT].z,        corners[AE_FRUSTUM_FAR_TOP_LEFT].z,        ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.z    = glm::lerp(corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].z,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].z,    ((float)x + 1.0f) / 20.0f);

        i += 2;

        //RightPlane
        m_Vtx[i].m_Position.y        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].y,    corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].y,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.y    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].y,       corners[AE_FRUSTUM_FAR_TOP_RIGHT].y,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.x        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].x,    corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].x,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.x    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].x,       corners[AE_FRUSTUM_FAR_TOP_RIGHT].x,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.z        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].z,    corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].z,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.z    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].z,       corners[AE_FRUSTUM_FAR_TOP_RIGHT].z,    ((float)x + 1.0f) / 20.0f);

        i += 2;

        //LeftPlane
        m_Vtx[i].m_Position.y        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].y,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].y,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.y    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_LEFT].y,        corners[AE_FRUSTUM_FAR_TOP_LEFT].y,       ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.x        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].x,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].x,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.x    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_LEFT].x,        corners[AE_FRUSTUM_FAR_TOP_LEFT].x,       ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.z        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].z,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].z,    ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.z    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_LEFT].z,        corners[AE_FRUSTUM_FAR_TOP_LEFT].z,       ((float)x + 1.0f) / 20.0f);

        i += 2;

        //Top
        m_Vtx[i].m_Position.y        = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].y,       corners[AE_FRUSTUM_FAR_TOP_RIGHT].y,      ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.y    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_LEFT].y,        corners[AE_FRUSTUM_FAR_TOP_LEFT].y,       ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.x        = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].x,       corners[AE_FRUSTUM_FAR_TOP_RIGHT].x,      ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.x    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_LEFT].x,        corners[AE_FRUSTUM_FAR_TOP_LEFT].x,       ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.z        = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_RIGHT].z,       corners[AE_FRUSTUM_FAR_TOP_RIGHT].z,      ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.z    = glm::lerp(corners[AE_FRUSTUM_NEAR_TOP_LEFT].z,        corners[AE_FRUSTUM_FAR_TOP_LEFT].z,       ((float)x + 1.0f) / 20.0f);

        i += 2;

        //Bottom
        m_Vtx[i].m_Position.y        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].y,    corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].y,   ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.y    = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].y,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].y,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.x        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].x,    corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].x,   ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.x    = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].x,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].x,    ((float)x + 1.0f) / 20.0f);

        m_Vtx[i].m_Position.z        = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_RIGHT].z,    corners[AE_FRUSTUM_FAR_BOTTOM_RIGHT].z,   ((float)x + 1.0f) / 20.0f);
        m_Vtx[i + 1].m_Position.z    = glm::lerp(corners[AE_FRUSTUM_NEAR_BOTTOM_LEFT].z,     corners[AE_FRUSTUM_FAR_BOTTOM_LEFT].z,    ((float)x + 1.0f) / 20.0f);

        i += 2;
    }


    return AEResult::Ok;
}

AEResult FrustumShape::UpdateFrustumShape(const Frustum* frustum)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(frustum != nullptr);
    if (frustum == nullptr)
    {
        return AEResult::NullObj;
    }

    AETODO("Change returns");
    if (UpdateFrustumShapeVtx(frustum) != AEResult::Ok)
    {
        return AEResult::Ok;
    }

    if (m_VB->SetVertexData(0, m_Vtx, 0, AE_FRUSTUM_SHAPE_SIZE, D3D11_MAP_WRITE_DISCARD) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult FrustumShape::DrawFrustumShape(bool simple)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AETODO("add better return codes");
    if (m_GraphicDevice->SetVertexBuffer(m_VB) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    if (m_GraphicDevice->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    uint32_t vtxSize = m_VB->GetSize();

    if (simple)
    {
        vtxSize = AE_FRUSTUM_SIMPLE_SHAPE_SIZE;
    }

    if (m_GraphicDevice->Draw(vtxSize, 0) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}
