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
