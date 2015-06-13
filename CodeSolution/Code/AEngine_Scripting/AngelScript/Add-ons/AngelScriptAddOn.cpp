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
#include "AngelScriptAddOn.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AngelScriptAddOn::AngelScriptAddOn(uint32_t addOnID)
	: m_AddOnID(addOnID)
{
}

AngelScriptAddOn::~AngelScriptAddOn()
{
}