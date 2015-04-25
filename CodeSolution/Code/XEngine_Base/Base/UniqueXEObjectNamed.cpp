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
#include "UniqueXEObjectNamed.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
UniqueXEObjectNamed::UniqueXEObjectNamed(const std::wstring& name)
	: Named(name)
{
	m_UniqueID = XE_Base::GetNextUniqueID();
}

UniqueXEObjectNamed::~UniqueXEObjectNamed()
{
}