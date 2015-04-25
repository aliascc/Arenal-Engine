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
#include "Textures\Texture.h"
#include "Base\BaseFunctions.h"
#include "ShaderTextureBinding.h"
#include "Shaders\Buffers\ConstantBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderTextureBinding::ShaderTextureBinding(const std::wstring& name, uint32_t bindIndex, TextureType textureType, Texture* texture)
	: ShaderBinding(name, bindIndex)
	, m_Texture(texture)
	, m_TextureType(textureType)
{
	XEAssert(!name.empty());
}

ShaderTextureBinding::~ShaderTextureBinding()
{
	XERelease(m_Texture);
}

XEResult ShaderTextureBinding::SetTexture(Texture* texture) 
{ 
	if (texture != nullptr && texture->GetTextureType() != m_TextureType)
	{
		return XEResult::InvalidTextureType;
	}

	XERelease(m_Texture);

	m_Texture = texture;

	return XEResult::Ok;
}

std::wstring ShaderTextureBinding::GetTextureName() const
{
	if (m_Texture == nullptr)
	{
		return L"";
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
	return	(
				this->m_BindIndex	== other.m_BindIndex	&&
				this->m_TextureType == other.m_TextureType	&&
				this->m_Name.compare(other.m_Name) == 0
			);
}