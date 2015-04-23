/** \file
* Contains definitions for handling Game Objects Scripts
*/

/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/19/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

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
#include "Base\XEObject.h"
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
#define XE_GOS_INTERFACE_NAME			"IGameObjectScript"

/// <summary>
/// Initialize Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define XE_GOS_INIT_FUNC				"void Initialize()"

/// <summary>
/// Constant Update Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define XE_GOS_CONSTANT_UPDATE_FUNC		"void ConstantUpdate(const TimerParams&in)"

/// <summary>
/// Update Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define XE_GOS_UPDATE_FUNC				"void Update(const TimerParams&in)"

/// <summary>
/// Post Update Function Declaration 
/// for the Game Object Script Interface.
/// </summary>
#define XE_GOS_POST_UPDATE_FUNC			"void PostUpdate(const TimerParams&in)"

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
struct GameObjectScriptInstance sealed : public XEObject
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
struct GameObjectScriptFunctions sealed : public XEObject
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
struct GameObjectScriptProperty sealed : public XEObject
{
	/// <summary>
	/// Name of the Property
	/// </summary>
	std::string m_PropertyName = "";

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
	XEResult SaveCurrentValue();
};

/// <summary>
/// Structure that holds Game Object Script Properties Information.
/// </summary>
struct GameObjectScriptProperties sealed : public XEObject
{
	/// <summary>
	/// Map Containing the Script Properties
	/// </summary>
	std::unordered_map<std::string, GameObjectScriptProperty*> m_PropertiesMap;

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
	XEResult SaveCurrentValues();

	void ClearPropertiesAddress();

	XEResult CopySavedValuesToProperties(GameObjectScriptProperties& target);
};

#endif