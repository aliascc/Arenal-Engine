/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _FPR_DEFS_H
#define _FPR_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

#define AE_TILE_RES												16

#define AE_FORWARD_PLUS_DEPTH_STENCIL_NAME						L"Forward Plus Z DS"

/****************
*   Constants   *
*****************/

/********************
*   Forward Decls   *
*********************/

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct ForwardPlusHelper sealed : public AEObject
{
	private:
		//So no one will create an instance of this struct
		ForwardPlusHelper();

	public:

		/// <summary>
		/// Calculate the number of tiles for the Screen Size
		/// </summary>
		/// <param name="screenSize">Screen Size for X and Y</param>
		/// <returns>Number of Tiles in the screen size for X and Y</returns>
		static glm::uvec2 GetNumTiles(const glm::uvec2& screenSize);

		/// <summary>
		/// Calculate the number of tiles for the Screen Size
		/// </summary>
		/// <param name="width">Width of the Screen</param>
		/// <param name="height">Height of the Screen</param>
		/// <returns>Number of Tiles in the screen size for X and Y</returns>
		static glm::uvec2 GetNumTiles(uint32_t width, uint32_t height);
};

#endif