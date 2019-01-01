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
#include "Textures\Texture.h"
#include "ShaderTextureBinding.h"
#include "Shaders\Buffers\ConstantBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderTextureBinding::ShaderTextureBinding(const std::string& name, uint32_t bindIndex, TextureType textureType, Texture* texture)
    : ShaderBinding(name, bindIndex)
    , m_Texture(texture)
    , m_TextureType(textureType)
{
    AEAssert(!name.empty());
}

ShaderTextureBinding::~ShaderTextureBinding()
{
    AERelease(m_Texture);
}

AEResult ShaderTextureBinding::SetTexture(Texture* texture) 
{ 
    if (texture != nullptr && texture->GetTextureType() != m_TextureType)
    {
        return AEResult::InvalidTextureType;
    }

    AERelease(m_Texture);

    m_Texture = texture;

    return AEResult::Ok;
}

std::string ShaderTextureBinding::GetTextureName() const
{
    if (m_Texture == nullptr)
    {
        return "";
    }

    return m_Texture->GetName();
}

bool ShaderTextureBinding::Compare(const ShaderTextureBinding& other) const
{
    return (*this == other);
}

bool ShaderTextureBinding::Compare(const ShaderTextureBinding* other) const
{
    if(other == nullptr)
    {
        return false;
    }

    return (*this == *other);
}

bool ShaderTextureBinding::operator==(const ShaderTextureBinding& other) const
{
    return  (
                this->m_BindIndex   == other.m_BindIndex    &&
                this->m_TextureType == other.m_TextureType  &&
                this->m_Name.compare(other.m_Name) == 0
            );
}
