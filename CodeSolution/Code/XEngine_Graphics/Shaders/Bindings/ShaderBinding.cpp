
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
#include "ShaderBinding.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderBinding::ShaderBinding(const std::wstring& name, uint32_t bindIndex)
	: UniqueXEObjectNamed(name)
	, m_BindIndex(bindIndex)
{
}

ShaderBinding::~ShaderBinding()
{
}