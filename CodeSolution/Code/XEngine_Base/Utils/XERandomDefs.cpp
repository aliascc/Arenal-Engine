/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/13/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "boost\random\uniform_int_distribution.hpp"
#include "boost\random\uniform_real_distribution.hpp"

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
	static boost::random::mt11213b m_RGen = boost::random::mt11213b();

	/********************
	*   Function Defs   *
	*********************/
	float XERandomHelpers::GetNextFloat()
	{
		boost::random::uniform_real_distribution<float> disRand;

		return disRand(m_RGen);
	}

	double XERandomHelpers::GetNextDouble()
	{
		boost::random::uniform_real_distribution<double> disRand;

		return disRand(m_RGen);
	}

	float XERandomHelpers::GetFloat(float max)
	{
		boost::random::uniform_real_distribution<float> disRand(0, max);

		return disRand(m_RGen);
	}

	float XERandomHelpers::GetFloat(float min, float max)
	{
		boost::random::uniform_real_distribution<float> disRand(min, max);

		return disRand(m_RGen);
	}

	int32_t XERandomHelpers::GetInt(int32_t max)
	{
		boost::random::uniform_int_distribution<int32_t> disRand(0, max);

		return disRand(m_RGen);
	}

}