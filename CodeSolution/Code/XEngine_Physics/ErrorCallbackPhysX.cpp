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
#include "Logger\Logger.h"
#include "ErrorCallbackPhysX.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

ErrorCallbackPhysX::ErrorCallbackPhysX()
{
}

ErrorCallbackPhysX::~ErrorCallbackPhysX()
{
}

void ErrorCallbackPhysX::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	XETODO("Add log error");
}