
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


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ShaderCustomVariable.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderCustomVariable::ShaderCustomVariable()
{
}

ShaderCustomVariable::ShaderCustomVariable(uint32_t size, uint32_t startOffset, uint32_t offset, uint32_t elementSize, uint32_t rows, uint32_t columns, std::wstring name, void* memory, bool userVariable, bool isArray, uint32_t elements, ShaderVariableClass shaderVariableClass, ShaderVariableType shaderVariableType)
	: m_Size(size)
	, m_StartOffset(startOffset)
	, m_Offset(offset)
	, m_ElementSize(elementSize)
	, m_Rows(rows)
	, m_Columns(columns)
	, m_Name(name)
	, m_Memory(memory)
	, m_UserVariable(userVariable)
	, m_IsArray(isArray)
	, m_Elements(elements)
	, m_ShaderVariableClass(shaderVariableClass)
	, m_ShaderVariableType(shaderVariableType)
{
}

ShaderCustomVariable::ShaderCustomVariable(const ShaderCustomVariable& other)
	: m_Memory(nullptr)
{
	*this = other;
}

ShaderCustomVariable::ShaderCustomVariable(const ShaderCustomVariable* other)
	: m_Memory(nullptr)
{
	if(other != nullptr)
	{
		*this = *other;
	}
	else
	{
		memset(this, 0, sizeof(ShaderCustomVariable));
	}
}

bool ShaderCustomVariable::Compare(const ShaderCustomVariable& other) const
{
	return (*this == other);
}

bool ShaderCustomVariable::Compare(const ShaderCustomVariable* other) const
{
	if(other == nullptr)
	{
		return false;
	}

	return (*this == *other);
}

bool ShaderCustomVariable::operator==(const ShaderCustomVariable& other) const
{
	return(	this->m_Size						== other.m_Size					&&
			this->m_StartOffset					== other.m_StartOffset			&&
			this->m_Offset						== other.m_Offset				&&
			this->m_ElementSize					== other.m_Elements				&&
			this->m_Rows						== other.m_Rows					&&
			this->m_Columns						== other.m_Columns				&&
			this->m_UserVariable				== other.m_UserVariable			&&
			this->m_IsArray						== other.m_IsArray				&&
			this->m_Elements					== other.m_Elements				&&
			this->m_ShaderVariableClass			== other.m_ShaderVariableClass	&&
			this->m_ShaderVariableType			== other.m_ShaderVariableType	&&
			this->m_Name.compare(other.m_Name)	== 0);
}

ShaderCustomVariable& ShaderCustomVariable::operator=(const ShaderCustomVariable& other)
{
	this->m_Size				= other.m_Size;
	this->m_StartOffset			= other.m_StartOffset;
	this->m_Offset				= other.m_Offset;
	this->m_ElementSize			= other.m_Elements;
	this->m_Rows				= other.m_Rows;
	this->m_Columns				= other.m_Columns;
	this->m_UserVariable		= other.m_UserVariable;
	this->m_IsArray				= other.m_IsArray;
	this->m_Elements			= other.m_Elements;
	this->m_ShaderVariableClass	= other.m_ShaderVariableClass;
	this->m_ShaderVariableType	= other.m_ShaderVariableType;
	this->m_Name				= other.m_Name;

	return *this;
}