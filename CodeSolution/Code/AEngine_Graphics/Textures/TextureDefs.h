/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _TEXTURE_DEFS_H
#define _TEXTURE_DEFS_H

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
enum class TextureType : uint32_t
{
	Texture2D = 0,
	TextureCube
};

enum class TextureBindOption : uint32_t
{
	Default = 0,
	RenderTarget,
	DepthStencil
};

/******************
*   Struct Decl   *
*******************/

#endif