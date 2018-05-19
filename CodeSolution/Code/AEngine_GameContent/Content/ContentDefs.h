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

#ifndef _CONTENT_DEFS_H
#define _CONTENT_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"
#include "Color\AEColorDefs.h"
#include "Vertex\VertexDefs.h"
#include "Shaders\ShaderDefs.h"
#include "Textures\TextureDefs.h"
#include "Vertex\Types\VertexTypes.h"
#include "Models\Skinning\SkinningDefs.h"
#include "Shaders\Variables\ShaderCustomVariable.h"

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/********************
*   Forward Decls   *
*********************/
class Bone;
class GraphicDevice;
struct VertexHolder;
class IVertexBuffer;
struct PolygonHolder;
struct MeshPartContent;
struct IVertexBufferContent;

/***************
*   Typedefs   *
****************/
typedef std::vector<VertexHolder> VertexHolderVector;
typedef std::vector<PolygonHolder> PolygonHolderVector;
typedef std::map<int32_t, std::vector<PolygonHolder>> PolygonHolderVecMap;

/*************
*   Struct   *
**************/

struct SamplerShaderHolder sealed : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    SamplerShaderHolder()
    {
    }
};

struct TextureArrayShaderVariableHolder sealed : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    TextureType m_TextureType = TextureType::Texture2D;

    TextureArrayShaderVariableHolder()
    {
    }
};

struct SimpleBufferShaderHolder sealed : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    bool m_IsRW = false;

    ShaderVariableClass m_VariableClass = ShaderVariableClass::Scalar;

    ShaderVariableType m_VariableType = ShaderVariableType::Int;

    uint32_t m_ElementCount = 0;

    SimpleBufferShaderHolder()
    {
    }
};

struct StructuredBufferShaderHolder sealed : public AEObject
{
    std::string m_Name = "";
    uint32_t m_BindIndex = 0;
    bool m_IsRW = false;

    StructuredBufferShaderHolder()
    {
    }
};

struct ConstantBufferShaderHolder sealed : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    std::list<ShaderCustomVariable> m_ShaderCustomVariableList;

    ConstantBufferShaderHolder()
    {
    }
};

struct TextureShaderVariableHolder sealed : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    TextureType m_TextureType = TextureType::Texture2D;

    TextureShaderVariableHolder()
    {
    }
};

/// <summary>
/// Struct to temporally hold Vertex information 
/// </summary>
struct VertexHolder sealed : public AEObject
{
    glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

    glm::vec2 m_UV = AEMathHelpers::Vec2fZero;

    glm::vec2 m_UV2 = AEMathHelpers::Vec2fZero;

    glm::vec3 m_Normal = AEMathHelpers::Vec3fZero;

    glm::vec4 m_Tangent = AEMathHelpers::Vec4fZero;

    glm::vec4 m_Binormal = AEMathHelpers::Vec4fZero;

    Color m_Color = AEColors::White;

    glm::ivec4 m_BlendIndices = AEMathHelpers::Vec4iZero;

    glm::vec4 m_BlendWeights = AEMathHelpers::Vec4fZero;;

    std::map<std::string, uint32_t> m_BoneToIndex;

    std::map<uint32_t, std::string> m_IndexToBone;

    uint32_t m_BlendIndexCount = 0;

    int32_t m_VertexID = -1;

    int32_t m_ControlPointIndex = -1;

    VertexHolder()
    {
    }

    std::string ToString() const override;
};

struct PolygonHolder final : public AEObject
{
    int32_t m_PolygonIndex = -1;
    int32_t m_MaterialIndex = -1;
    uint64_t m_MaterialID = 0;

    VertexHolderVector m_Vertexs;

    PolygonHolder()
    {
    }
};

struct PolygonHolderContainer final : public AEObject
{
    PolygonHolderVector m_Polygons;

    PolygonHolderContainer();

    PolygonHolderVecMap GetPolygonsSeparate();

    AEResult SetMaterialIndexToPolygons(int32_t polyIdx, int32_t matIdx);

    void SetMaterialIndexToAllPolygons(int32_t matIdx);

    void SetMaterialIDToPolygons(uint64_t matID, int32_t matIdx);

    void SetBlendIndicesWeightToVertex(int32_t controlPointIndex, int32_t blendIndex, float blendWeight);

    inline void Clear()
    {
        m_Polygons.clear();
    }

    inline void Add(const PolygonHolder& poly)
    {
        m_Polygons.push_back(poly);
    }
};

struct MaterialHolder sealed : public AEObject
{
    int32_t m_MaterialIndex = -1;

    uint64_t m_MaterialID = 0;

    std::string m_Name = "";

    std::string m_DiffuseTextureString = "";

    std::string m_NormalTextureString = "";

    std::string m_SpecularTextureString = "";

    std::string m_LightMapTextureString = "";

    std::string m_DisplacementTextureString = "";

    Color m_DiffuseColor = AEColors::White;

    MaterialHolder()
    {
    }
};

struct MeshPartHolder sealed : public AEObject
{
    VertexType m_VertexType = VertexType::VtxNone;

    uint32_t m_MaterialID = 0;

    VertexHolderVector m_VtxHolderVec;

    std::vector<uint16_t> m_IndexHolder;

    MeshPartHolder()
    {
    }
};

struct MeshHolder sealed : public AEObject
{
    std::string m_Name = "";

    glm::mat4 m_WorldTransform = AEMathHelpers::Mat4Identity;

    std::vector<MeshPartHolder> m_MeshPartHolderVec;

    PolygonHolderContainer m_PolygonHolderContainer;

    MeshHolder()
    {
    }
};

struct TransformsHolder sealed : public AEObject
{
    glm::mat4 m_LocalTransform = AEMathHelpers::Mat4Identity;

    glm::mat4 m_WorldTransform = AEMathHelpers::Mat4Identity;

    TransformsHolder()
    {
    }
};

struct KeyFrameHolder sealed : public AEObject
{
    std::map<float, BonePose> m_BonePoseMap;

    std::string m_BoneName = "";

    KeyFrameHolder()
    {
    }
};

struct AnimationHolder sealed : public AEObject
{
    std::vector<KeyFrameHolder> m_KeyFramesVec;

    float m_Duration = 0.0f;

    std::string m_Name = "";

    AnimationHolder()
    {
    }
};

struct TreeBone sealed : public AEObject
{
    Bone* m_Bone = nullptr;

    std::list<TreeBone*> m_ChildBones;

    TreeBone();
    virtual ~TreeBone();
};

AETODO("Should rename of move out of here");

namespace AEImporterHelpers
{
    extern glm::mat4 ConvertAssimpMatrix(const aiMatrix4x4& assimpMatrix);

    extern AEResult BuildVertexBuffer(GraphicDevice* graphicDevice, IVertexBuffer** vertexBuffer, VertexType vtxType, void* buffer, uint32_t size);

    extern AEResult ConvertToMeshContent(const MeshPartHolder& meshPartHolder, MeshPartContent** meshPartContent, bool generateTangentBinormal = false);

    extern AEResult BuildVertexBufferContent(VertexType vtxType, const VertexHolderVector& tempVtxBuffer, IVertexBufferContent** vtxBufffer, bool generateTangentBinormal = false, const std::vector<uint16_t>& idxsData = std::vector<uint16_t>(0));

    extern bool IsVertexTypeValid(VertexType vtxType);

    extern AEResult GetVertexArray(VertexType vtxType, const VertexHolderVector& vxtHolderVector, void* vtxArr, uint32_t size);
    extern AEResult GetVertexArrayP(const VertexHolderVector& vxtHolderVector, VertexPosition vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPC(const VertexHolderVector& vxtHolderVector, VertexPositionColor vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPN(const VertexHolderVector& vxtHolderVector, VertexPositionNormal vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPT(const VertexHolderVector& vxtHolderVector, VertexPositionTexture vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPNC(const VertexHolderVector& vxtHolderVector, VertexPositionNormalColor vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPNT(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTexture vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPNTT2(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTextureTexture2 vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPNTIW(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTextureIndicesWeight vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPNTBT(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTexture vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPNTBTT2(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTextureTexture2 vtxArr[], uint32_t size);
    extern AEResult GetVertexArrayPNTBTIW(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTextureIndicesWeight vtxArr[], uint32_t size);

    template<class T>
    AEResult CreateVertexBuffer(GraphicDevice* graphicDevice, IVertexBuffer** vertexBuffer, void* buffer, uint32_t size)
    {
        AEAssert(graphicDevice != nullptr);
        AEAssert(vertexBuffer != nullptr);
        AEAssert(buffer != nullptr);

        if (graphicDevice == nullptr || vertexBuffer == nullptr || buffer == nullptr)
        {
            return AEResult::NullParameter;
        }

        VertexBuffer<T>* vtxBufferTemplate = new VertexBuffer<T>(graphicDevice);

        T* arrayBuffer = reinterpret_cast<T*>(buffer);

        vtxBufferTemplate->CopyVertexBuffer(arrayBuffer, size);
        vtxBufferTemplate->BuildVertexBuffer();

        (*vertexBuffer) = vtxBufferTemplate;

        return AEResult::Ok;
    }

    template<class T>
    AEResult CreateVertexBufferContent(const VertexHolderVector& vxtHolderVector, IVertexBufferContent** vtxBuff, bool generateTangentBinormal = false, const std::vector<uint16_t>& idxsData = std::vector<uint16_t>(0), uint32_t geometryStride = 0, uint32_t normalStride = 12, uint32_t tangentStride = 24, uint32_t binormalStride = 40, uint32_t textureCoordsStride = 56)
    {
        AEAssert(vtxBuff != nullptr);

        if (vtxBuff == nullptr)
        {
            return AEResult::NullParameter;
        }

        uint32_t size = (uint32_t)vxtHolderVector.size();

        T* tmpBuff = new T[size];
        ZeroMemory(tmpBuff, sizeof(T)* size);

        AEImporterHelpers::GetVertexArray(T::GetVertexType(), vxtHolderVector, tmpBuff, size);

        if (generateTangentBinormal == true)
        {
            AEMathHelpers::CalculateTangentsAndBinormal(tmpBuff, idxsData, size, sizeof(T), geometryStride, normalStride, tangentStride, binormalStride, textureCoordsStride);
        }

        VertexBufferContent<T>* cntBuff = new VertexBufferContent<T>();
        cntBuff->m_Buffer = tmpBuff;
        cntBuff->m_Size = size;

        (*vtxBuff) = cntBuff;

        return AEResult::Ok;
    }
}

#endif
