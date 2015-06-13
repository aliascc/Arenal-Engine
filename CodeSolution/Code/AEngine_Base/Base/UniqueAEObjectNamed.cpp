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
#include "UniqueAEObjectNamed.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
UniqueAEObjectNamed::UniqueAEObjectNamed(const std::wstring& name)
	: Named(name)
{
	m_UniqueID = AE_Base::GetNextUniqueID();
}

UniqueAEObjectNamed::~UniqueAEObjectNamed()
{
}