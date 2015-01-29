/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 5/18/2012
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

/***************************
*   Game Engine Includes   *
****************************/
#include "XEObject.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XEObject::XEObject()
{
}

XEObject::~XEObject()
{
}

std::wstring XEObject::ToString() const
{
	return L"";
}
