/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/22/2012
*
* Desc:
*
*
* Copyright 2012
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
#include "ITexture2D.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ITexture2D::ITexture2D(GraphicDevice* graphicDevice, const std::wstring& textureName, bool isCube)
	: Texture(graphicDevice, (isCube ? TextureType::TextureCube : TextureType::Texture2D), textureName)
{
}

ITexture2D::~ITexture2D()
{
	ReleaseCOM(m_TextureDX);
}