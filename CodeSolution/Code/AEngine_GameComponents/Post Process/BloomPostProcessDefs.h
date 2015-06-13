/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _BLOOM_POST_PROCESS_DEFS_H
#define _BLOOM_POST_PROCESS_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/********************************
*		Using					*
*********************************/

/**************
*   Defines   *
***************/
#define AE_BLOOM_BLUR_SAMPLE_COUNT 15

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct BloomPostProcessSettings sealed : public AEObject
{
        // Name of a preset bloom setting, for display to the user.
        std::string		m_Name;

        // Controls how bright a pixel needs to be before it will bloom.
        // Zero makes everything bloom equally, while higher values select
        // only brighter colors. Somewhere between 0.25 and 0.5 is good.
        float			m_BloomThreshold;
		
		// Controls how much blurring is applied to the bloom image.
        // The typical range is from 1 up to 10 or so.
        float			m_BlurAmount;

        // Controls the amount of the bloom and base images that
        // will be mixed into the final scene. Range 0 to 1.
        float			m_BloomIntensity;
        float			m_BaseIntensity;

        // Independently control the color saturation of the bloom and
        // base images. Zero is totally desaturated, 1.0 leaves saturation
        // unchanged, while higher values increase the saturation level.
        float			m_BloomSaturation;
        float			m_BaseSaturation;

		//Constructor
		BloomPostProcessSettings();
		BloomPostProcessSettings(const std::string& name, float bloomThreshold, float blurAmount,
								 float bloomIntensity, float baseIntensity,
								 float bloomSaturation, float baseSaturation);
};

#endif