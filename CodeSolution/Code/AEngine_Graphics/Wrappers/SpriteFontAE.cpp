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
#include "SpriteFontAE.h"
#include "GraphicDevice.h"
#include "SpriteBatchAE.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/************
*   Using   *
*************/

/********************
*   Function Defs   *
*********************/
SpriteFontAE::SpriteFontAE(GraphicDevice* graphicDevice, const std::string spriteFontFile)
    : m_GraphicDevice(graphicDevice)
    , m_SpriteFontFile(spriteFontFile)
{
    AEAssert(graphicDevice != nullptr);

    AEAssert(!spriteFontFile.empty());
}

SpriteFontAE::~SpriteFontAE()
{
    DeleteMem(m_SpriteFont);
}

AEResult SpriteFontAE::Initialize()
{
    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    if(m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    if(m_SpriteFontFile.empty())
    {
        return AEResult::EmptyFilename;
    }

    try
    {
        std::wstring spriteFontFileW = AE_Base::String2WideStr(m_SpriteFontFile);
        m_SpriteFont = new DirectX::SpriteFont(m_GraphicDevice->GetDeviceDX(), spriteFontFileW.c_str());
    }
    catch(...)
    {
        AETODO("Get correct Exception")
        return AEResult::Fail;
    }

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult SpriteFontAE::DrawString(const SpriteBatchAE* spriteBatch, const std::string& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, float scale, DirectX::SpriteEffects effects, float layerDepth)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(spriteBatch != nullptr);

    if(spriteBatch == nullptr)
    {
        return AEResult::NullParameter;
    }

    if(!spriteBatch->HasBegun())
    {
        return AEResult::Fail;
    }

    DirectX::XMFLOAT2 xmPosition(position.x, position.y);
    DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
    DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);

    std::wstring textW = AE_Base::String2WideStr(text);

    m_SpriteFont->DrawString(spriteBatch->m_SpriteBatch, textW.c_str(), xmPosition, xmColor, rotation, xmOrigin, scale, effects, layerDepth);

    return AEResult::Ok;
}

AEResult SpriteFontAE::DrawString(const SpriteBatchAE* spriteBatch, const std::string& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects, float layerDepth)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(spriteBatch != nullptr);

    if(spriteBatch == nullptr)
    {
        return AEResult::NullParameter;
    }

    if(!spriteBatch->HasBegun())
    {
        return AEResult::Fail;
    }

    DirectX::XMFLOAT2 xmPosition(position.x, position.y);
    DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
    DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);
    DirectX::XMFLOAT2 xmScale(scale.x, scale.y);

    std::wstring textW = AE_Base::String2WideStr(text);

    m_SpriteFont->DrawString(spriteBatch->m_SpriteBatch, textW.c_str(), xmPosition, xmColor, rotation, xmOrigin, xmScale, effects, layerDepth);

    return AEResult::Ok;
}

glm::vec2 SpriteFontAE::MeasureString(const std::string& text) const
{
    if(!m_IsReady)
    {
        return AEMathHelpers::Vec2fZero;
    }

    std::wstring textW = AE_Base::String2WideStr(text);

    DirectX::XMVECTOR xmVec = m_SpriteFont->MeasureString(textW.c_str());
    DirectX::XMFLOAT2 xmSize;
    DirectX::XMStoreFloat2(&xmSize, xmVec);

    glm::vec2 ret(xmSize.x, xmSize.y);

    return ret;
}
