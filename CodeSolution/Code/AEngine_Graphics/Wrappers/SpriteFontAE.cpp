/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

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
SpriteFontAE::SpriteFontAE(GraphicDevice* graphicDevice, const std::wstring spriteFontFile)
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
		m_SpriteFont = new DirectX::SpriteFont(m_GraphicDevice->GetDeviceDX(), m_SpriteFontFile.c_str());
	}
	AETODO("Get correct Exception")
	catch(...)
	{
		return AEResult::Fail;
	}

	m_IsReady = true;

	return AEResult::Ok;
}

AEResult SpriteFontAE::DrawString(const SpriteBatchAE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, float scale, DirectX::SpriteEffects effects, float layerDepth)
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
	
	m_SpriteFont->DrawString(spriteBatch->m_SpriteBatch, text.c_str(), xmPosition, xmColor, rotation, xmOrigin, scale, effects, layerDepth);

	return AEResult::Ok;
}

AEResult SpriteFontAE::DrawString(const SpriteBatchAE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects, float layerDepth)
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

	m_SpriteFont->DrawString(spriteBatch->m_SpriteBatch, text.c_str(), xmPosition, xmColor, rotation, xmOrigin, xmScale, effects, layerDepth);

	return AEResult::Ok;
}

glm::vec2 SpriteFontAE::MeasureString(const std::wstring& text) const
{
	if(!m_IsReady)
	{
		return AEMathHelpers::Vec2fZero;
	}

	DirectX::XMVECTOR xmVec = m_SpriteFont->MeasureString(text.c_str());
	DirectX::XMFLOAT2 xmSize;
	DirectX::XMStoreFloat2(&xmSize, xmVec);

	glm::vec2 ret(xmSize.x, xmSize.y);

	return ret;
}