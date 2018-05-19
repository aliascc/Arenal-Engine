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
#include "VertexDefs.h"
#include "Types\VertexTypes.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/*********************
*   AEVertexHelper   *
**********************/
namespace AEVertexHelper
{
    uint32_t GetVertexSizeByType(VertexType vtxType)
    {
        switch (vtxType)
        {
            case VertexType::VtxPos:
                return VertexPosition::VertexSize();

            case VertexType::VtxPosCol:
                return VertexPositionColor::VertexSize();

            case VertexType::VtxPosNor:
                return VertexPositionNormal::VertexSize();

            case VertexType::VtxPosTex:
                return VertexPositionTexture::VertexSize();

            case VertexType::VtxPosNorCol:
                return VertexPositionNormalColor::VertexSize();

            case VertexType::VtxPosNorTex:
                return VertexPositionNormalTexture::VertexSize();

            case VertexType::VtxPosNorTexTex2:
                return VertexPositionNormalTextureTexture2::VertexSize();

            case VertexType::VtxPosNorTexIdxWght:
                return VertexPositionNormalTextureIndicesWeight::VertexSize();

            case VertexType::VtxPosNorTanBinTex:
                return VertexPositionNormalTangentBinormalTexture::VertexSize();

            case VertexType::VtxPosNorTanBinTexTex2:
                return VertexPositionNormalTangentBinormalTextureTexture2::VertexSize();

            case VertexType::VtxPosNorTanBinTexIdxWght:
                return VertexPositionNormalTangentBinormalTextureIndicesWeight::VertexSize();

            case VertexType::VtxCustom:
            case VertexType::VtxNone:
            default:
                return 0;
        }
    }

    std::string GetVertexString(const VertexPositionColor& vtx)
    {
        std::string vtxString = fmt::format("{0}{1}{2}{3}",
                                            vtx.m_Position.x,            //0
                                            vtx.m_Position.y,            //1
                                            vtx.m_Position.z,            //2
                                            (uint32_t)vtx.m_Color        //3
                                );

        return vtxString;
    }

    std::string GetVertexString(const VertexPositionNormalTexture& vtx)
    {
        std::string vtxString = fmt::format("{0}{1}{2}{3}{4}{5}{6}{7}",
                                            vtx.m_Position.x,            //0
                                            vtx.m_Position.y,            //1
                                            vtx.m_Position.z,            //2
                                            vtx.m_Normal.x,              //3
                                            vtx.m_Normal.y,              //4
                                            vtx.m_Normal.z,              //5
                                            vtx.m_TexCoord.x,            //6
                                            vtx.m_TexCoord.y             //7
                                            );

        return vtxString;
    }
}
