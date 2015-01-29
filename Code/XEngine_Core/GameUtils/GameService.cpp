/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/23/2013
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
#include "GameService.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"
/********************
*   Function Defs   *
*********************/
XETODO("add mutex");
GameService::GameService(const std::wstring& name)
	: UniqueXEObjectNamed(name)
{
}

GameService::~GameService()
{
}
