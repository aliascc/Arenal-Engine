/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SHADER_CUSTOM_VARIABLE_H
#define _SHADER_CUSTOM_VARIABLE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\Named.h"
#include "Base\AEObject.h"
#include "Shaders\ShaderDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct ShaderCustomVariable sealed : public AEObject
{
	uint32_t m_Size = 0;

	uint32_t m_StartOffset = 0;

	uint32_t m_Offset = 0;

	uint32_t m_ElementSize = 0;

	uint32_t m_Rows = 0;

	uint32_t m_Columns = 0;

	std::wstring m_Name = L"";

	void* m_Memory = nullptr;

	bool m_UserVariable = false;

	bool m_IsArray = false;

	uint32_t m_Elements = 0;

	ShaderVariableClass m_ShaderVariableClass = ShaderVariableClass::Scalar;

	ShaderVariableType m_ShaderVariableType = ShaderVariableType::Int;

	ShaderCustomVariable();

	ShaderCustomVariable(uint32_t size, uint32_t startOffset, uint32_t offset, uint32_t elementSize, uint32_t rows, uint32_t columns, std::wstring name, void* memory, bool userVariable, bool isArray, uint32_t elements, ShaderVariableClass shaderVariableClass, ShaderVariableType shaderVariableType);

	ShaderCustomVariable(const ShaderCustomVariable& other);

	ShaderCustomVariable(const ShaderCustomVariable* other);

	inline uint32_t CalculateOffsetForNextVariable()
	{
		return (m_StartOffset + m_Offset + m_Size);
	}

	bool Compare(const ShaderCustomVariable& other) const;

	bool Compare(const ShaderCustomVariable* other) const;

	bool operator==(const ShaderCustomVariable& other) const;

	ShaderCustomVariable& operator=(const ShaderCustomVariable& other);
};

#endif