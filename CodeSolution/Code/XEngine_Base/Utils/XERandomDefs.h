/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _XE_RANDOM_DEFS_H
#define _XE_RANDOM_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/***********************
*   Global Functions   *
************************/
namespace XERandomHelpers
{
	extern float GetFloat(float min, float max);

	extern int32_t GetInt(int32_t min, int32_t max);

	extern uint32_t GetUInt(uint32_t min, uint32_t max);
};

#endif