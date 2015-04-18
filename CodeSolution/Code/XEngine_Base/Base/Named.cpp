/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/18/2012
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
#include "Named.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Named::Named(const std::wstring& name)
	: m_Name(name)
{
}

Named::~Named()
{
}

void Named::SetName(const std::wstring name)
{
	m_Name = name;
}

std::wstring Named::ToString() const
{
	return m_Name;
}
