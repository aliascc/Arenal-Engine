/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

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