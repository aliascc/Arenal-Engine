/** \file
* Contains definitions for handling Game Objects Scripts
*/

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

XEResult GameObjectScriptProperty::SaveCurrentValue()
{
	XEAssert(m_PropertyAddress != nullptr);
	if (m_PropertyAddress == nullptr)
	{
		return XEResult::NullObj;
	}

	DeleteMemArr(m_SavedMem);

	uint32_t size = XEAngelScriptHelpers::GetSizeOfType(m_PropertyType);
	if (size == 0)
	{
		return XEResult::ZeroSize;
	}

	m_SavedMem = new uint8_t[size];

	memcpy(m_SavedMem, m_PropertyAddress, size);

	return XEResult::Ok;
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

XEResult GameObjectScriptProperties::SaveCurrentValues()
{
	XEResult ret = XEResult::Ok;

	for (auto scriptProperty : m_PropertiesMap)
	{
		ret = scriptProperty.second->SaveCurrentValue();
		if (ret != XEResult::Ok)
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

XEResult GameObjectScriptProperties::CopySavedValuesToProperties(GameObjectScriptProperties& target)
{
	for (auto scriptProperty : target.m_PropertiesMap)
	{
		const std::string& propName = scriptProperty.first;

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

		uint32_t size = XEAngelScriptHelpers::GetSizeOfType(gosProp->m_PropertyType);
		if (size == 0)
		{
			continue;
		}
		
		memcpy(gosProp->m_PropertyAddress, m_PropertiesMap[propName]->m_SavedMem, size);
	}

	return XEResult::Ok;
}