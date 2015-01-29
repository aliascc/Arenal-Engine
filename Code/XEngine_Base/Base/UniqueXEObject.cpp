/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/21/2013
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
#include "UniqueXEObject.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
UniqueXEObject::UniqueXEObject()
{
	m_UniqueID = XE_Base::GetNextUniqueID();
}

UniqueXEObject::~UniqueXEObject()
{
}