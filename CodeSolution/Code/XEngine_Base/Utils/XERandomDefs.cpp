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
#include <random>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "XERandomDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

namespace XERandomHelpers
{
	/*********************
	*   Variables Defs   *
	**********************/
	std::default_random_engine mRandomGenerator((std::random_device())());;

	/********************
	*   Function Defs   *
	*********************/
	float XERandomHelpers::GetFloat(float min, float max)
	{
		std::uniform_real_distribution<float> disRand(min, max);

		return disRand(mRandomGenerator);
	}

	int32_t XERandomHelpers::GetInt(int32_t max)
	{
		std::uniform_int_distribution<int32_t> disRand(0, max);

		return disRand(mRandomGenerator);
	}

}
