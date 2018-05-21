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
#include "Texture.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Texture::Texture(GraphicDevice* graphicDevice, TextureType textureType, const std::string& textureName)
    : GameResource(textureName, GameResourceType::Unknown)
    , m_GraphicDevice(graphicDevice)
    , m_TextureType(textureType)
{
    AEAssert(m_GraphicDevice != nullptr);

    switch (m_TextureType)
    {
        case TextureType::Texture2D:
            m_ResourceType = GameResourceType::Texture2D;
            break;
        case TextureType::TextureCube:
            m_ResourceType = GameResourceType::TextureCube;
            break;
        default:
            AEAssert(false);
            break;
    }
}

Texture::~Texture()
{
    ReleaseCOM(m_ShaderResourceView);
}
