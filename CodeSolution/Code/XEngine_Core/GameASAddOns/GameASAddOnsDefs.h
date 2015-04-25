/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_AS_ADD_ONS_DEFS_H
#define _GAME_AS_ADD_ONS_DEFS_H

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

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/************
*   Enums   *
*************/
enum class XEGameASAddOnsID : uint32_t
{
	Base = 0,
	GLM,
	Log,
	Localization,
	Time,
	Color,
	GameCore,
	Input,
	Audio,
	Assets
};

#endif