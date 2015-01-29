/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 8/10/2014
*
* Desc:
*
*
* Copyright 2014
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