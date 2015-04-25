/********************************************************
*
* Author: Carlos Chacón N.
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
#include "boost\random\mersenne_twister.hpp"

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
	extern double GetNextDouble();

	extern float GetNextFloat();

	extern float GetFloat(float max);

	extern float GetFloat(float min, float max);

	extern int32_t GetInt(int32_t max);
};

#endif