/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _SHADER_SIGNATURES_H
#define _SHADER_SIGNATURES_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Textures\TextureDefs.h"
#include "Shaders\Variables\ShaderCustomVariable.h"

/************
*   Using   *
*************/

/*************
*   Struct   *
**************/
struct ConstantBufferSignature : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    std::list<ShaderCustomVariable> m_ShaderCustomVariableList;

    ConstantBufferSignature()
    {
    }
};

struct SimpleBufferSignature : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    bool m_IsRW = false;

    ShaderVariableClass m_VariableClass = ShaderVariableClass::Scalar;

    ShaderVariableType m_VariableType = ShaderVariableType::Int;

    uint32_t m_ElementCount = 0;

    SimpleBufferSignature()
    {
    }
};

struct StructuredBufferSignature : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    bool m_IsRW = false;

    StructuredBufferSignature()
    {
    }
};

struct TextureInputSignature : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    TextureType m_TextureType = TextureType::Texture2D;

    TextureInputSignature()
    {
    }
};

struct TextureArrayInputSignature : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    TextureType m_TextureType = TextureType::Texture2D;

    TextureArrayInputSignature()
    {
    }
};

struct SamplerSignature : public AEObject
{
    std::string m_Name = "";

    uint32_t m_BindIndex = 0;

    SamplerSignature()
    {
    }
};

/**************
*   Typedef   *
***************/
typedef std::list<ConstantBufferSignature> ConstantBufferSignatureList;

typedef std::list<SimpleBufferSignature> SimpleBufferSignatureList;

typedef std::list<StructuredBufferSignature> StructuredBufferSignatureList;

typedef std::list<TextureInputSignature> TextureInputSignatureList;

typedef std::list<TextureArrayInputSignature> TextureArrayInputSignatureList;

typedef std::list<SamplerSignature> SamplerSignatureList;

/***********************
*   Helper Functions   *
************************/

namespace AEShaderSignatureHelpers
{
    extern ShaderCustomVariable CreateScalarVariable(const std::string& name, uint32_t offset, ShaderVariableType svType);

    extern ShaderCustomVariable CreateVectorVariable(const std::string& name, uint32_t offset, uint32_t numElementsInVector);

    extern ShaderCustomVariable CreateMatrixVariable(const std::string& name, uint32_t offset, bool isArray = false, uint32_t numElementsInArray = 0);

    extern ConstantBufferSignature CreateWorldViewProjCBSig(uint32_t bindIndex);

    extern ConstantBufferSignature CreateViewProjCBSig(uint32_t bindIndex);

    extern ConstantBufferSignature CreateBonesCBSig(uint32_t bindIndex);

    extern ConstantBufferSignature CreateColorCBSig(uint32_t bindIndex);

    extern ConstantBufferSignature CreateHalfPixelCBSig(uint32_t bindIndex);

    extern ConstantBufferSignature CreateFPRLightCullingCBSig(uint32_t bindIndex);

    extern ConstantBufferSignature CreateFPRLightsCBSig(uint32_t bindIndex);

    extern StructuredBufferSignature CreateBufferLightBufferStructureBufferSig(uint32_t bindIndex);

    extern StructuredBufferSignature CreateShadowSpotLightInfoStructureBufferSig(uint32_t bindIndex);

    extern StructuredBufferSignature CreateShadowDirectionalLightInfoStructureBufferSig(uint32_t bindIndex);

    extern SimpleBufferSignature CreateBufferPerTileLightIndexBufferSimpleBufferSig(uint32_t bindIndex);

    extern void CreateForwardRenderingPlusSignatures(ConstantBufferSignatureList& cbSigList, StructuredBufferSignatureList& structuredBufferSigList, SimpleBufferSignatureList& simpleBufferSigList, TextureInputSignatureList& textureInputSigList);
}

#endif
