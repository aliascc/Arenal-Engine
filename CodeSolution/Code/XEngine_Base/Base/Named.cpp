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
