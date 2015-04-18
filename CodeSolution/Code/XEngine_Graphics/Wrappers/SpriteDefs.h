/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/29/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
#pragma once

#ifndef _SPRITE_DEFS_H
#define _SPRITE_DEFS_H

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

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class SpriteStateChange : uint32_t
{
	Discard = 0,
	Keep
};

/*************
*   Struct   *
**************/

/**************
*   Typedef   *
***************/

#endif