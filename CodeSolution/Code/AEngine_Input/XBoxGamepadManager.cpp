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
#include <stdexcept>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "XBoxGamepad.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "XBoxGamepadManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AETODO("Add mutex");
XBoxGamepadManager::XBoxGamepadManager()
{
	for (uint32_t i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_Gamepads[i] = new XBoxGamepad(i);
	}
}

XBoxGamepadManager::~XBoxGamepadManager()
{
	for (uint32_t i = 0; i < XUSER_MAX_COUNT; i++)
	{
		DeleteMem(m_Gamepads[i]);
	}
}

void XBoxGamepadManager::Update(const TimerParams& timerParams)
{
	for (uint32_t i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_Gamepads[i]->Update(timerParams);
	}
}

XBoxGamepad& XBoxGamepadManager::GetGamepad(uint32_t index) const
{
	AEAssert(index < XUSER_MAX_COUNT);
	if (index >= XUSER_MAX_COUNT)
	{
		throw std::out_of_range("XBox Gamepad Index out of range.");
	}

	return *m_Gamepads[index];
}

XBoxGamepad& XBoxGamepadManager::operator[](uint32_t index) const
{
	return GetGamepad(index);
}