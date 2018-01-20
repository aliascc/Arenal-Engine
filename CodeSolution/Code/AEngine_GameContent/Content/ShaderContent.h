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

#ifndef _SHADER_CONTENT_H
#define _SHADER_CONTENT_H

/**********************
*   System Includes   *
***********************/
#include <list>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameContent.h"
#include "Shaders\ShaderDefs.h"
#include "Content\ContentDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct ShaderContent sealed : public GameContent
{
    std::wstring m_Name = L"";

    ShaderType m_ShaderType = ShaderType::VertexShader;

    void* m_ShaderByteCode = nullptr;

    uint32_t m_ByteCodeSize = 0;

    std::list<ConstantBufferShaderHolder> m_ConstantBufferShaderHolderList;

    std::list<TextureShaderVariableHolder> m_TextureShaderVariableHolderList;
    
    std::list<StructuredBufferShaderHolder> m_StructuredBufferShaderHolderList;
    
    std::list<SimpleBufferShaderHolder> m_SimpleBufferShaderHolderList;

    std::list<TextureArrayShaderVariableHolder> m_TextureArrayShaderVariableHolderList;

    std::list<SamplerShaderHolder> m_SamplerShaderHolderList;

    ShaderContent();
    virtual ~ShaderContent();
};

#endif
