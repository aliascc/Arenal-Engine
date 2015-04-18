
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/7/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

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