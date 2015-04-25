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
#include "TextureArray.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
TextureArray::TextureArray(GraphicDevice* graphicDevice, const std::wstring& name, TextureType textureType, uint32_t bindIndex)
	: ShaderBinding(name, bindIndex)
	, m_GraphicDevice(graphicDevice)
	, m_TextureType(textureType)
{
	XEAssert(m_GraphicDevice != nullptr);
}

TextureArray::~TextureArray()
{
	ReleaseCOM(m_TextureArraySRV);
}