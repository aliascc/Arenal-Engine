/** \file
* Contains definitions for handling Game Objects Scripts
*/

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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "GameObjectScriptDefs.h"
#include "AngelScript\AngelScriptDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/******************
*   Struct Defs   *
*******************/

/**************************************
*   struct GameObjectScriptInstance   *
***************************************/
GameObjectScriptInstance::GameObjectScriptInstance()
{
}

GameObjectScriptInstance::~GameObjectScriptInstance()
{
	ReleaseAngel(m_ASObj);
}

/***************************************
*   struct GameObjectScriptFunctions   *
****************************************/
GameObjectScriptFunctions::GameObjectScriptFunctions()
{
}

GameObjectScriptFunctions::~GameObjectScriptFunctions()
{
	ReleaseAngel(m_Constructor);
	ReleaseAngel(m_Initialize);
	ReleaseAngel(m_ConstantUpdate);
	ReleaseAngel(m_Update);
	ReleaseAngel(m_PostUpdate);
}

/**************************************
*   struct GameObjectScriptProperty   *
***************************************/
GameObjectScriptProperty::GameObjectScriptProperty()
{
}

GameObjectScriptProperty::~GameObjectScriptProperty()
{
	DeleteMemArr(m_SavedMem);
}

bool GameObjectScriptProperty::operator==(const GameObjectScriptProperty& other) const
{
	return (this->m_PropertyName.compare(other.m_PropertyName) == 0 &&
		this->m_PropertyType == other.m_PropertyType);
}

bool GameObjectScriptProperty::operator!=(const GameObjectScriptProperty& other) const
{
	return !(*this == other);
}

AEResult GameObjectScriptProperty::SaveCurrentValue()
{
	AEAssert(m_PropertyAddress != nullptr);
	if (m_PropertyAddress == nullptr)
	{
		return AEResult::NullObj;
	}

	DeleteMemArr(m_SavedMem);

	uint32_t size = AEAngelScriptHelpers::GetSizeOfType(m_PropertyType);
	if (size == 0)
	{
		return AEResult::ZeroSize;
	}

	m_SavedMem = new uint8_t[size];

	memcpy(m_SavedMem, m_PropertyAddress, size);

	return AEResult::Ok;
}

/****************************************
*   struct GameObjectScriptProperties   *
*****************************************/
GameObjectScriptProperties::GameObjectScriptProperties()
{
}

GameObjectScriptProperties::~GameObjectScriptProperties()
{
	for (auto scriptProperty : m_PropertiesMap)
	{
		DeleteMem(scriptProperty.second);
	}
	m_PropertiesMap.clear();
}

AEResult GameObjectScriptProperties::SaveCurrentValues()
{
	AEResult ret = AEResult::Ok;

	for (auto scriptProperty : m_PropertiesMap)
	{
		ret = scriptProperty.second->SaveCurrentValue();
		if (ret != AEResult::Ok)
		{
			break;
		}
	}

	return ret;
}

void GameObjectScriptProperties::ClearPropertiesAddress()
{
	for (auto scriptProperty : m_PropertiesMap)
	{
		scriptProperty.second->m_PropertyAddress = nullptr;
	}
}

AEResult GameObjectScriptProperties::CopySavedValuesToProperties(GameObjectScriptProperties& target)
{
	for (auto scriptProperty : target.m_PropertiesMap)
	{
		const std::wstring& propName = scriptProperty.first;

		GameObjectScriptProperty* gosProp = scriptProperty.second;
		if (gosProp->m_PropertyAddress == nullptr)
		{
			continue;
		}
		
		if (m_PropertiesMap.find(scriptProperty.first) == m_PropertiesMap.end())
		{
			continue;
		}

		if (m_PropertiesMap[propName]->m_PropertyType != gosProp->m_PropertyType)
		{
			continue;
		}

		if (m_PropertiesMap[propName]->m_SavedMem == nullptr)
		{
			continue;
		}

		uint32_t size = AEAngelScriptHelpers::GetSizeOfType(gosProp->m_PropertyType);
		if (size == 0)
		{
			continue;
		}
		
		memcpy(gosProp->m_PropertyAddress, m_PropertiesMap[propName]->m_SavedMem, size);
	}

	return AEResult::Ok;
}

GameObjectScriptProperty* GameObjectScriptProperties::GetGameObjectScriptProperty(const std::wstring& name)
{
	if (!GameObjectScriptPropertyExists(name))
	{
		return nullptr;
	}

	return m_PropertiesMap[name];
}
