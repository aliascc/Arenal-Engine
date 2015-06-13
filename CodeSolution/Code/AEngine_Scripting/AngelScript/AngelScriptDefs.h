/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ANGEL_SCRIPT_DEFS_H
#define _ANGEL_SCRIPT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "angelscript.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct AELog;
struct TimeStamp;
struct TimerParams;

/*************
*   Define   *
**************/

/// <summary>
/// Defines the Base Add on ID for Users Add-on
/// </summary>
#define AE_AS_ADD_ON_ID_USER_START		10000

/***********************
*   Global Functions   *
************************/

namespace AEAngelScriptHelpers
{
	extern uint32_t GetSizeOfType(asETypeIdFlags typeID);

	extern bool TypeIsBasic(asETypeIdFlags typeID);
}

#endif