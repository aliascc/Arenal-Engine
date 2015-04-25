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
#include "SpriteFontXE.h"
#include "GraphicDevice.h"
#include "SpriteBatchXE.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/************
*   Using   *
*************/

/********************
*   Function Defs   *
*********************/
SpriteFontXE::SpriteFontXE(GraphicDevice* graphicDevice, const std::wstring spriteFontFile)
	: m_GraphicDevice(graphicDevice)
	, m_SpriteFontFile(spriteFontFile)
{
	XEAssert(graphicDevice != nullptr);

	XEAssert(!spriteFontFile.empty());
}

SpriteFontXE::~SpriteFontXE()
{
	DeleteMem(m_SpriteFont);
}

XEResult SpriteFontXE::Initialize()
{
	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	if(m_SpriteFontFile.empty())
	{
		return XEResult::EmptyFilename;
	}

	try
	{
		m_SpriteFont = new DirectX::SpriteFont(m_GraphicDevice->GetDeviceDX(), m_SpriteFontFile.c_str());
	}
	XETODO("Get correct Exception")
	catch(...)
	{
		return XEResult::Fail;
	}

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult SpriteFontXE::DrawString(const SpriteBatchXE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, float scale, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(spriteBatch != nullptr);

	if(spriteBatch == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(!spriteBatch->HasBegun())
	{
		return XEResult::Fail;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);
	
	m_SpriteFont->DrawString(spriteBatch->m_SpriteBatch, text.c_str(), xmPosition, xmColor, rotation, xmOrigin, scale, effects, layerDepth);

	return XEResult::Ok;
}

XEResult SpriteFontXE::DrawString(const SpriteBatchXE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(spriteBatch != nullptr);

	if(spriteBatch == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(!spriteBatch->HasBegun())
	{
		return XEResult::Fail;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);
	DirectX::XMFLOAT2 xmScale(scale.x, scale.y);

	m_SpriteFont->DrawString(spriteBatch->m_SpriteBatch, text.c_str(), xmPosition, xmColor, rotation, xmOrigin, xmScale, effects, layerDepth);

	return XEResult::Ok;
}

glm::vec2 SpriteFontXE::MeasureString(const std::wstring& text) const
{
	if(!m_IsReady)
	{
		return XEMathHelpers::Vec2fZero;
	}

	DirectX::XMVECTOR xmVec = m_SpriteFont->MeasureString(text.c_str());
	DirectX::XMFLOAT2 xmSize;
	DirectX::XMStoreFloat2(&xmSize, xmVec);

	glm::vec2 ret(xmSize.x, xmSize.y);

	return ret;
}