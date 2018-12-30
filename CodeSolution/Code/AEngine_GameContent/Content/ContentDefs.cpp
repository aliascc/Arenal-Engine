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
#include "precomp_gamecontent.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ContentDefs.h"
#include "MeshPartContent.h"
#include "Vertex\VertexBuffer.h"
#include "VertexBufferContent.h"
#include "Models\Skinning\Bone.h"
#include "Vertex\IVertexBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/**********************
*   Struct TreeBone   *
**********************/

/***************
*   TreeBone   *
****************/
TreeBone::TreeBone()
{
}

TreeBone::~TreeBone()
{
    DeleteMem(m_Bone);
}

/*******************
*   VertexHolder   *
********************/
std::string VertexHolder::ToString() const
{
    std::string vtxString = fmt::format("{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}{11}{12}{13}{14}{15}{16}{17}{18}{19}{20}{21}{22}{23}{24}{25}{26}",
        m_Position.x,                       // 0
        m_Position.y,                       // 1
        m_Position.z,                       // 2
        m_UV.x,                             // 3
        m_UV.y,                             // 4
        m_UV2.x,                            // 5
        m_UV2.y,                            // 6
        m_Normal.x,                         // 7
        m_Normal.y,                         // 8
        m_Normal.z,                         // 9
        m_Tangent.x,                        // 10
        m_Tangent.y,                        // 11
        m_Tangent.z,                        // 12
        m_Tangent.w,                        // 13
        m_Binormal.x,                       // 14
        m_Binormal.y,                       // 15
        m_Binormal.z,                       // 16
        m_Binormal.w,                       // 17
        (uint32_t)m_Color,                  // 18
        m_BlendIndices.x,                   // 19
        m_BlendIndices.y,                   // 20
        m_BlendIndices.z,                   // 21
        m_BlendIndices.w,                   // 22
        m_BlendWeights.x,                   // 23
        m_BlendWeights.y,                   // 24
        m_BlendWeights.z,                   // 25
        m_BlendWeights.w                    // 26
        );

    return vtxString;
}

/*****************************
*   PolygonHolderContainer   *
******************************/
PolygonHolderContainer::PolygonHolderContainer()
{
}

PolygonHolderVecMap PolygonHolderContainer::GetPolygonsSeparate()
{
    PolygonHolderVecMap tempPartMap;

    uint32_t size = (uint32_t)m_Polygons.size();
    for (uint32_t i = 0; i < size; ++i)
    {
        if (tempPartMap.find(m_Polygons[i].m_MaterialIndex) == tempPartMap.end())
        {
            tempPartMap[m_Polygons[i].m_MaterialIndex] = PolygonHolderVector();
        }

        tempPartMap[m_Polygons[i].m_MaterialIndex].push_back(m_Polygons[i]);
    }

    return tempPartMap;
}

AEResult PolygonHolderContainer::SetMaterialIndexToPolygons(int32_t polyIdx, int32_t matIdx)
{
    if (polyIdx >= (int32_t)m_Polygons.size() || polyIdx < 0)
    {
        return AEResult::OutsideRange;
    }

    m_Polygons[polyIdx].m_MaterialIndex = matIdx;

    return AEResult::Ok;
}

void PolygonHolderContainer::SetMaterialIndexToAllPolygons(int32_t matIdx)
{
    uint32_t size = (uint32_t)m_Polygons.size();

    for (uint32_t i = 0; i < size; ++i)
    {
        m_Polygons[i].m_MaterialIndex = matIdx;
    }
}

void PolygonHolderContainer::SetMaterialIDToPolygons(uint64_t matID, int32_t matIdx)
{
    uint32_t size = (uint32_t)m_Polygons.size();

    for (uint32_t i = 0; i < size; ++i)
    {
        if (m_Polygons[i].m_MaterialIndex == matIdx)
        {
            m_Polygons[i].m_MaterialID = matID;
        }
    }
}

void PolygonHolderContainer::SetBlendIndicesWeightToVertex(int32_t controlPointIndex, int32_t blendIndex, float blendWeight)
{
    if (blendWeight < 0.003f)
    {
        return;
    }

    uint32_t size = (uint32_t)m_Polygons.size();

    for (uint32_t i = 0; i < size; ++i)
    {
        uint32_t vtxSize = (uint32_t)m_Polygons[i].m_Vertexs.size();;

        for (uint32_t j = 0; j < vtxSize; ++j)
        {
            if (m_Polygons[i].m_Vertexs[j].m_ControlPointIndex == controlPointIndex)
            {
                if (m_Polygons[i].m_Vertexs[j].m_BlendIndexCount >= 4)
                {
                    AETODO("Add log here");
                    int a = 8;
                    AEAssert(false);
                }
                else
                {
                    int32_t* bi = (int32_t*)&m_Polygons[i].m_Vertexs[j].m_BlendIndices;
                    float* bw = (float*)&m_Polygons[i].m_Vertexs[j].m_BlendWeights;

                    bi[m_Polygons[i].m_Vertexs[j].m_BlendIndexCount] = blendIndex;
                    bw[m_Polygons[i].m_Vertexs[j].m_BlendIndexCount] = blendWeight;

                    m_Polygons[i].m_Vertexs[j].m_BlendIndexCount++;
                }
            }
        }
    }
}

/************************
*   AEImporterHelpers   *
*************************/
namespace AEImporterHelpers
{

    glm::mat4 ConvertAssimpMatrix(const aiMatrix4x4& assimpMatrix)
    {
        glm::mat4 mat = AEMathHelpers::Mat4Identity;

        //Row 1
        mat[0][0] = assimpMatrix.a1;
        mat[0][1] = assimpMatrix.b1;
        mat[0][2] = assimpMatrix.c1;
        mat[0][3] = assimpMatrix.d1;

        //Row 2
        mat[1][0] = assimpMatrix.a2;
        mat[1][1] = assimpMatrix.b2;
        mat[1][2] = assimpMatrix.c2;
        mat[1][3] = assimpMatrix.d2;

        //Row 3
        mat[2][0] = assimpMatrix.a3;
        mat[2][1] = assimpMatrix.b3;
        mat[2][2] = assimpMatrix.c3;
        mat[2][3] = assimpMatrix.d3;

        //Row 4
        mat[3][0] = assimpMatrix.a4;
        mat[3][1] = assimpMatrix.b4;
        mat[3][2] = assimpMatrix.c4;
        mat[3][3] = assimpMatrix.d4;

        return mat;
    }

    AEResult BuildVertexBuffer(GraphicDevice& graphicDevice, IVertexBuffer** vertexBuffer, VertexType vtxType, void* buffer, uint32_t size)    
    {
        AEResult ret = AEResult::Ok;

        switch (vtxType)
        {
            case VertexType::VtxPos:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPosition>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosCol:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionColor>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNor:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormal>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosTex:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionTexture>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNorCol:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormalColor>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNorTex:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormalTexture>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNorTexTex2:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormalTextureTexture2>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNorTexIdxWght:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormalTextureIndicesWeight>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNorTanBinTex:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormalTangentBinormalTexture>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNorTanBinTexTex2:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormalTangentBinormalTextureTexture2>(graphicDevice, vertexBuffer, buffer, size);
                break;
            case VertexType::VtxPosNorTanBinTexIdxWght:
                ret = AEImporterHelpers::CreateVertexBuffer<VertexPositionNormalTangentBinormalTextureIndicesWeight>(graphicDevice, vertexBuffer, buffer, size);
                break;
            default:
                ret = AEResult::Fail;
                break;
        }

        return ret;
    }

    AEResult ConvertToMeshContent(const MeshPartHolder& meshPartHolder, MeshPartContent** meshPartContent, bool generateTangentBinormal)
    {
        if(meshPartContent == nullptr)
        {
            return AEResult::NullParameter;
        }

        MeshPartContent* meshContentTemp = new MeshPartContent();
    
        meshContentTemp->m_MatID = meshPartHolder.m_MaterialID;

        meshContentTemp->m_VertexType = meshPartHolder.m_VertexType;

        AEResult ret = BuildVertexBufferContent(meshContentTemp->m_VertexType, meshPartHolder.m_VtxHolderVec, &meshContentTemp->m_VtxBuff, generateTangentBinormal, meshPartHolder.m_IndexHolder);

        if(ret != AEResult::Ok)
        {
            DeleteMem(meshContentTemp);
            return ret;
        }

        uint32_t idxSize = (uint32_t)meshPartHolder.m_IndexHolder.size();
        meshContentTemp->m_IdxSize = idxSize;
        meshContentTemp->m_IdxBuf16 = new uint16_t[idxSize];
        ZeroMemory(meshContentTemp->m_IdxBuf16, idxSize * sizeof(uint16_t));

        for (uint32_t i = 0; i < idxSize; ++i)
        {
            meshContentTemp->m_IdxBuf16[i] = meshPartHolder.m_IndexHolder[i];
        }

        (*meshPartContent) = meshContentTemp;

        return AEResult::Ok;
    }

    AEResult BuildVertexBufferContent(VertexType vtxType, const VertexHolderVector& tempVtxBuffer, IVertexBufferContent** vtxBufffer, bool generateTangentBinormal, const std::vector<uint16_t>& idxBuffer)
    {
        AEResult ret = AEResult::Ok;

        switch (vtxType)
        {
            case VertexType::VtxPos:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPosition>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosCol:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionColor>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosNor:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormal>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosTex:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionTexture>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosNorCol:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormalColor>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosNorTex:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormalTexture>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosNorTexTex2:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormalTextureTexture2>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosNorTexIdxWght:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormalTextureIndicesWeight>(tempVtxBuffer, vtxBufffer);
                break;
            case VertexType::VtxPosNorTanBinTex:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormalTangentBinormalTexture>(tempVtxBuffer, vtxBufffer, generateTangentBinormal, idxBuffer);
                break;
            case VertexType::VtxPosNorTanBinTexTex2:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormalTangentBinormalTextureTexture2>(tempVtxBuffer, vtxBufffer, generateTangentBinormal, idxBuffer);
                break;
            case VertexType::VtxPosNorTanBinTexIdxWght:
                ret = AEImporterHelpers::CreateVertexBufferContent<VertexPositionNormalTangentBinormalTextureIndicesWeight>(tempVtxBuffer, vtxBufffer, generateTangentBinormal, idxBuffer);
                break;
            default:
                ret = AEResult::Fail;
                break;
        }

        return ret;
    }

    bool IsVertexTypeValid(VertexType vtxType)
    {
        bool ret = false;

        switch (vtxType)
        {
            //We accept all this Vertex Types
            case VertexType::VtxPos:
            case VertexType::VtxPosCol:
            case VertexType::VtxPosNor:
            case VertexType::VtxPosNorCol:
            case VertexType::VtxPosTex:
            case VertexType::VtxPosNorTex:
            case VertexType::VtxPosNorTexIdxWght:
            case VertexType::VtxPosNorTexTex2:
            case VertexType::VtxPosNorTanBinTex:
            case VertexType::VtxPosNorTanBinTexIdxWght:
            case VertexType::VtxPosNorTanBinTexTex2:
                ret = true;
                break;
            default:
                ret = false;
                break;
        }

        return ret;
    }

    AEResult GetVertexArray(VertexType vtxType, const VertexHolderVector& vxtHolderVector, void* vtxArr, uint32_t size)
    {
        AEAssert(vtxArr != nullptr);

        if(vtxArr == nullptr)
        {
            return AEResult::NullParameter;
        }

        switch (vtxType)
        {
            case VertexType::VtxPos:
                {
                    VertexPosition* vtxArrTrans = reinterpret_cast<VertexPosition*>(vtxArr);
                    return GetVertexArrayP(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosCol:
                {
                    VertexPositionColor* vtxArrTrans = reinterpret_cast<VertexPositionColor*>(vtxArr);
                    return GetVertexArrayPC(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNor:
                {
                    VertexPositionNormal* vtxArrTrans = reinterpret_cast<VertexPositionNormal*>(vtxArr);
                    return GetVertexArrayPN(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosTex:
                {
                    VertexPositionTexture* vtxArrTrans = reinterpret_cast<VertexPositionTexture*>(vtxArr);
                    return GetVertexArrayPT(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNorCol:
                {
                    VertexPositionNormalColor* vtxArrTrans = reinterpret_cast<VertexPositionNormalColor*>(vtxArr);
                    return GetVertexArrayPNC(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNorTex:
                {
                    VertexPositionNormalTexture* vtxArrTrans = reinterpret_cast<VertexPositionNormalTexture*>(vtxArr);
                    return GetVertexArrayPNT(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNorTexTex2:
                {
                    VertexPositionNormalTextureTexture2* vtxArrTrans = reinterpret_cast<VertexPositionNormalTextureTexture2*>(vtxArr);
                    return GetVertexArrayPNTT2(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNorTexIdxWght:
                {
                    VertexPositionNormalTextureIndicesWeight* vtxArrTrans = reinterpret_cast<VertexPositionNormalTextureIndicesWeight*>(vtxArr);
                    return GetVertexArrayPNTIW(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNorTanBinTex:
                {
                    VertexPositionNormalTangentBinormalTexture* vtxArrTrans = reinterpret_cast<VertexPositionNormalTangentBinormalTexture*>(vtxArr);
                    return GetVertexArrayPNTBT(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNorTanBinTexTex2:
                {
                    VertexPositionNormalTangentBinormalTextureTexture2* vtxArrTrans = reinterpret_cast<VertexPositionNormalTangentBinormalTextureTexture2*>(vtxArr);
                    return GetVertexArrayPNTBTT2(vxtHolderVector, vtxArrTrans, size);
                }
                break;
            case VertexType::VtxPosNorTanBinTexIdxWght:
                {
                    VertexPositionNormalTangentBinormalTextureIndicesWeight* vtxArrTrans = reinterpret_cast<VertexPositionNormalTangentBinormalTextureIndicesWeight*>(vtxArr);
                    return GetVertexArrayPNTBTIW(vxtHolderVector, vtxArrTrans, size);
                }
                break;
        }
    
        AEAssert(false);

        return AEResult::Fail;
    }

    AEResult GetVertexArrayP(const VertexHolderVector& vxtHolderVector, VertexPosition vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPC(const VertexHolderVector& vxtHolderVector, VertexPositionColor vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Color       = vxtHolderVector[i].m_Color;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPN(const VertexHolderVector& vxtHolderVector, VertexPositionNormal vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {        
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal       = vxtHolderVector[i].m_Normal;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPT(const VertexHolderVector& vxtHolderVector, VertexPositionTexture vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_TexCoord    = vxtHolderVector[i].m_UV;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPNC(const VertexHolderVector& vxtHolderVector, VertexPositionNormalColor vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal      = vxtHolderVector[i].m_Normal;
            vtxArr[i].m_Color       = vxtHolderVector[i].m_Color;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPNTBT(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTexture vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal      = vxtHolderVector[i].m_Normal;
            vtxArr[i].m_TexCoord    = vxtHolderVector[i].m_UV;
            vtxArr[i].m_Tangent     = vxtHolderVector[i].m_Tangent;
            vtxArr[i].m_Binormal    = vxtHolderVector[i].m_Binormal;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPNTBTT2(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTextureTexture2 vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal      = vxtHolderVector[i].m_Normal;
            vtxArr[i].m_TexCoord    = vxtHolderVector[i].m_UV;
            vtxArr[i].m_TexCoord2   = vxtHolderVector[i].m_UV2;
            vtxArr[i].m_Tangent     = vxtHolderVector[i].m_Tangent;
            vtxArr[i].m_Binormal    = vxtHolderVector[i].m_Binormal;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPNTT2(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTextureTexture2 vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal      = vxtHolderVector[i].m_Normal;
            vtxArr[i].m_TexCoord    = vxtHolderVector[i].m_UV;
            vtxArr[i].m_TexCoord2   = vxtHolderVector[i].m_UV2;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPNT(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTexture vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position    = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal      = vxtHolderVector[i].m_Normal;
            vtxArr[i].m_TexCoord    = vxtHolderVector[i].m_UV;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPNTIW(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTextureIndicesWeight vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position        = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal          = vxtHolderVector[i].m_Normal;
            vtxArr[i].m_TexCoord        = vxtHolderVector[i].m_UV;
            vtxArr[i].m_BlendIndices    = vxtHolderVector[i].m_BlendIndices;
            vtxArr[i].m_BlendWeight     = vxtHolderVector[i].m_BlendWeights;
        }

        return AEResult::Ok;
    }

    AEResult GetVertexArrayPNTBTIW(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTextureIndicesWeight vtxArr[], uint32_t size)
    {
        for(uint32_t i = 0; i < size; ++i)
        {
            vtxArr[i].m_Position        = vxtHolderVector[i].m_Position;
            vtxArr[i].m_Normal          = vxtHolderVector[i].m_Normal;
            vtxArr[i].m_TexCoord        = vxtHolderVector[i].m_UV;
            vtxArr[i].m_Tangent         = vxtHolderVector[i].m_Tangent;
            vtxArr[i].m_Binormal        = vxtHolderVector[i].m_Binormal;
            vtxArr[i].m_BlendIndices    = vxtHolderVector[i].m_BlendIndices;
            vtxArr[i].m_BlendWeight     = vxtHolderVector[i].m_BlendWeights;
        }

        return AEResult::Ok;
    }

}
