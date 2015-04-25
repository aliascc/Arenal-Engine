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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Texture.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Texture::Texture(GraphicDevice* graphicDevice, TextureType textureType, const std::wstring& textureName)
	: GameResource(textureName, GameResourceType::Unknown)
	, m_GraphicDevice(graphicDevice)
	, m_TextureType(textureType)
{
	XEAssert(m_GraphicDevice != nullptr);

	switch (m_TextureType)
	{
		case TextureType::Texture2D:
			m_ResourceType = GameResourceType::Texture2D;
			break;
		case TextureType::TextureCube:
			m_ResourceType = GameResourceType::TextureCube;
			break;
		default:
			XEAssert(false);
			break;
	}
}

Texture::~Texture()
{
	ReleaseCOM(m_ShaderResourceView);
}
