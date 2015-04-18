/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/8/2012
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

/***************************
*   Game Engine Includes   *
****************************/
#include "BloomPostProcessDefs.h"

/********************
*   Function Defs   *
*********************/
BloomPostProcessSettings::BloomPostProcessSettings()
	: m_Name("Default")
	, m_BloomThreshold(0.25f)
	, m_BlurAmount(4.0f)
	, m_BloomIntensity(1.25f)
	, m_BaseIntensity(1.0f)
	, m_BloomSaturation(1.0f)
	, m_BaseSaturation(1.0f)
{
}
		
BloomPostProcessSettings::BloomPostProcessSettings(const std::string& name, float bloomThreshold, float blurAmount,
													float bloomIntensity, float baseIntensity,
													float bloomSaturation, float baseSaturation)
	: m_Name(name)
	, m_BloomThreshold(bloomThreshold)
	, m_BlurAmount(blurAmount)
	, m_BloomIntensity(bloomIntensity)
	, m_BaseIntensity(baseIntensity)
	, m_BloomSaturation(bloomSaturation)
	, m_BaseSaturation(baseSaturation)
{
}
