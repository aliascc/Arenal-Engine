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

#pragma once

#ifndef _VERTEX_POSITION_NORMAL_TEXTURE_TEXTURE2_H
#define _VERTEX_POSITION_NORMAL_TEXTURE_TEXTURE2_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Vertex\VertexDefs.h"
#include "Vertex\VertexLayout.h"

#if defined(DEBUG) | defined(_DEBUG)
#include "Compiled Materials\HLSL\DummyVertexPositionNormalTextureTexture2_x64_d.h"        
#else
#include "Compiled Materials\HLSL\DummyVertexPositionNormalTextureTexture2_x64.h"
#endif

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/******************
*   Struct Decl   *
*******************/
struct VertexPositionNormalTextureTexture2
{
    //Position
    glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

    //Normal 
    glm::vec3 m_Normal = AEMathHelpers::Vec3fZero;

    //Texture Coordinates
    glm::vec2 m_TexCoord = AEMathHelpers::Vec2fZero;

    //Texture Coordinates 2
    glm::vec2 m_TexCoord2 = AEMathHelpers::Vec2fZero;

    //Vertex Layout
    static VertexLayout* m_VertexLayout;

    //Get Vertex Declaration
    static const VertexLayout* GetVertexLayout(GraphicDevice* graphicDevice)
    {
        if(m_VertexLayout != nullptr)
        {
            return m_VertexLayout;
        }

        D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
        {
            {"POSITION",    0,    DXGI_FORMAT_R32G32B32_FLOAT,    0,     0,    D3D11_INPUT_PER_VERTEX_DATA,    0},
            {"NORMAL",      0,    DXGI_FORMAT_R32G32B32_FLOAT,    0,    12,    D3D11_INPUT_PER_VERTEX_DATA,    0},
            {"TEXCOORD",    0,    DXGI_FORMAT_R32G32_FLOAT,       0,    24,    D3D11_INPUT_PER_VERTEX_DATA,    0},
            {"TEXCOORD",    1,    DXGI_FORMAT_R32G32_FLOAT,       0,    32,    D3D11_INPUT_PER_VERTEX_DATA,    0}
        };

        m_VertexLayout = new VertexLayout();

        AEResult ret = m_VertexLayout->BuildVertexLayout(graphicDevice, DummyVertexPositionNormalTextureTexture2, sizeof(DummyVertexPositionNormalTextureTexture2), vertexDesc, 4, "VertexPositionNormalTextureTexture2Layout");

        if(ret != AEResult::Ok)
        {
            DeleteMem(m_VertexLayout);
        }

        return m_VertexLayout;
    }

    //Delete Vertex Layout
    static void DeleteVertexLayout()
    {
        DeleteMem(m_VertexLayout);
    }

    static VertexType GetVertexType()
    {
        return VertexType::VtxPosNorTexTex2;
    }

    static uint32_t VertexSize()
    {
        return sizeof(VertexPositionNormalTextureTexture2);
    }
};

#endif
