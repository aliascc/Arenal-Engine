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
#include "ShaderBinding.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderBinding::ShaderBinding(const std::wstring& name, uint32_t bindIndex)
	: UniqueAEObjectNamed(name)
	, m_BindIndex(bindIndex)
{
}

ShaderBinding::~ShaderBinding()
{
}