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

#pragma once

#ifndef _GAME_OBJECT_SCRIPT_DEFS_H
#define _GAME_OBJECT_SCRIPT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <stdint.h>
#include <unordered_map>

/*************************
*   3rd Party Includes   *
**************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "AngelScript\AngelScriptManager.h"

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

/// <summary>
/// Name of the Game Object Script Interface.
/// </summary>
#define AE_GOS_INTERFACE_NAME			"IGameObjectScript"

/// <summary>
/// Initialize Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define AE_GOS_INIT_FUNC				"void Initialize()"

/// <summary>
/// Constant Update Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define AE_GOS_CONSTANT_UPDATE_FUNC		"void ConstantUpdate(const TimerParams&in)"

/// <summary>
/// Update Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define AE_GOS_UPDATE_FUNC				"void Update(const TimerParams&in)"

/// <summary>
/// Post Update Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define AE_GOS_POST_UPDATE_FUNC			"void PostUpdate(const TimerParams&in)"

/**************
*   Typedef   *
***************/

/************
*   Enums   *
*************/

/******************
*   Struct Decl   *
*******************/

/// <summary>
/// Structure that holds Game Object Script Instance Object and name.
/// </summary>
struct GameObjectScriptInstance sealed : public AEObject
{
	/// <summary>
	/// Name of the instance of the Game Object Script
	/// </summary>
	std::wstring m_Name = L"";

	/// <summary>
	/// Angel Script Object
	/// </summary>
	asIScriptObject* m_ASObj = nullptr;

	/// <summary>
	/// Default Constructor
	/// </summary>
	GameObjectScriptInstance();

	/// <summary>
	/// Default Destructor
	/// </summary>
	~GameObjectScriptInstance();
};

/// <summary>
/// Structure that holds Game Object Script Functions.
/// </summary>
struct GameObjectScriptFunctions sealed : public AEObject
{

	/// <summary>
	/// Constructor of the Script Object
	/// </summary>
	asIScriptFunction* m_Constructor = nullptr;

	/// <summary>
	/// Initialize Function of the Script Object
	/// </summary>
	asIScriptFunction* m_Initialize = nullptr;

	/// <summary>
	/// Constant Update Function of the Script Object
	/// </summary>
	asIScriptFunction* m_ConstantUpdate = nullptr;

	/// <summary>
	/// Update Function of the Script Object
	/// </summary>
	asIScriptFunction* m_Update = nullptr;

	/// <summary>
	/// Post Update Function of the Script Object
	/// </summary>
	asIScriptFunction* m_PostUpdate = nullptr;

	/// <summary>
	/// Default Constructor
	/// </summary>
	GameObjectScriptFunctions();

	/// <summary>
	/// Default Destructor
	/// </summary>
	~GameObjectScriptFunctions();
};

/// <summary>
/// Structure that holds Game Object Script Property Info.
/// </summary>
struct GameObjectScriptProperty sealed : public AEObject
{
	/// <summary>
	/// Name of the Property
	/// </summary>
	std::wstring m_PropertyName = L"";

	/// <summary>
	/// Index use by the Property
	/// </summary>
	uint32_t m_PropertyIndex = 0;

	/// <summary>
	/// Type of the Property
	/// </summary>
	asETypeIdFlags m_PropertyType = asTYPEID_VOID;

	/// <summary>
	/// Pointer to Property Address
	/// </summary>
	void* m_PropertyAddress = nullptr;

	/// <summary>
	/// Copy of Contents of the Property, use to save them when reloading scripts
	/// </summary>
	uint8_t* m_SavedMem = nullptr;

	/// <summary>
	/// Default Constructor
	/// </summary>
	GameObjectScriptProperty();

	/// <summary>
	/// Default Destructor
	/// </summary>
	~GameObjectScriptProperty();

	/// <summary>
	/// Equal operator overload.
	/// </summary>
	bool operator==(const GameObjectScriptProperty& other) const;

	/// <summary>
	/// Not equal operator overload.
	/// </summary>
	bool operator!=(const GameObjectScriptProperty& other) const;

	/// <summary>
	/// Copies the current value of the property to memory
	/// </summary>
	/// <returns>Return Ok if value was copied successfully</returns>
	AEResult SaveCurrentValue();
};

/// <summary>
/// Structure that holds Game Object Script Properties Information.
/// </summary>
struct GameObjectScriptProperties sealed : public AEObject
{
	/// <summary>
	/// Map Containing the Script Properties
	/// </summary>
	std::unordered_map<std::wstring, GameObjectScriptProperty*> m_PropertiesMap;

	/// <summary>
	/// Default Constructor
	/// </summary>
	GameObjectScriptProperties();

	/// <summary>
	/// Default Destructor
	/// </summary>
	~GameObjectScriptProperties();

	/// <summary>
	/// Copies the current value of the properties to memory
	/// </summary>
	/// <returns>Return Ok if values were copied successfully</returns>
	AEResult SaveCurrentValues();

	void ClearPropertiesAddress();

	inline bool GameObjectScriptPropertyExists(const std::wstring& name) const
	{
		return (m_PropertiesMap.find(name) != m_PropertiesMap.end());
	}

	AEResult CopySavedValuesToProperties(GameObjectScriptProperties& target);

	GameObjectScriptProperty* GetGameObjectScriptProperty(const std::wstring& name);
};

#endif