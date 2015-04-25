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
#include "FPRDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/************************
*   ForwardPlusHelper   *
*************************/
ForwardPlusHelper::ForwardPlusHelper()
{
}

glm::uvec2 ForwardPlusHelper::GetNumTiles(const glm::uvec2& screenSize)
{
	return GetNumTiles(screenSize.x, screenSize.y);
}

glm::uvec2 ForwardPlusHelper::GetNumTiles(uint32_t width, uint32_t height)
{
	glm::uvec2 numTiles;

	float numTilesX = ( (width + XE_TILE_RES - 1 ) / (float)XE_TILE_RES );
	float numTilesY = ( (height + XE_TILE_RES - 1 ) / (float)XE_TILE_RES );

	numTiles.x = static_cast<uint32_t>(numTilesX);
	numTiles.y = static_cast<uint32_t>(numTilesY);

	return numTiles;
}
