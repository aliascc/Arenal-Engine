/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ANGEL_SCRIPT_MANAGER_H
#define _ANGEL_SCRIPT_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <set>
#include <vector>

/*************************
*   3rd Party Includes   *
**************************/
#include "angelscript.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\XEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class AngelScriptAddOn;

/*****************
*   Class Decl   *
******************/
class AngelScriptManager sealed : public XEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Angel Script Main Engine
		/// </summary>
		asIScriptEngine* m_ASEngine = nullptr;

		/// <summary>
		/// Lets know the Manager it is ready to start using the AS Engine
		/// </summary>
		bool m_IsReady = false;

		/// <summary>
		/// Keeps track of all the Add Ons IDs that have been registered.
		/// </summary>
		std::set<uint32_t> m_AddOnsRegisterIDs;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		/// <summary>
		/// Callback used by the Angel Script Engine to give human readable error messages when something isn't working as it should
		/// </summary>
		/// <param name='msg'>Angel Script Struct with information</param>
		void ASMessageCallback(const asSMessageInfo& msg);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// AngelScriptManager Constructor
		/// </summary>
		AngelScriptManager();

		/// <summary>
		/// Default AngelScriptManager Destructor
		/// </summary>
		virtual ~AngelScriptManager();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Get the Angel Script Engine Handler
		/// </summary>
		/// <returns>Angel Script Engine Handler</returns>
		inline asIScriptEngine* GetASEngine() const
		{ 
			return m_ASEngine; 
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Initialize the Angel Script Manager
		/// </summary>
		/// <returns>Return Ok if initialization was successful</returns>
		XEResult Initialize();

		/// <summary>
		/// Registers an Angel Script Add-on
		/// </summary>
		/// <param name='addOn'>Add-on to register.</param>
		/// <returns>Return Ok if registration was successful</returns>
		XEResult RegisterAddOn(AngelScriptAddOn& addOn);

		/// <summary>
		/// Loads a Script to Memory
		/// </summary>
		/// <param name='scriptName'>Script File Name</param>
		/// <param name='module'>Module on which script will be loaded, module can not exist</param>
		/// <param name='asModule'>If Module exist Angel Script Module will be assigned to this parameter if non null</param>
		/// <returns>Return Ok if script was loaded was successfully</returns>
		XEResult LoadScript(const std::wstring& scriptName, const std::wstring& module, asIScriptModule** asModule = nullptr);

		/// <summary>
		/// Loads Scripts to Memory
		/// </summary>
		/// <param name='scriptNames'>Vector containing the Scripts File Names</param>
		/// <param name='module'>Module on which the scripts will be loaded, module can not exist</param>
		/// <param name='asModule'>If Module exist or is created, Angel Script Module will be assigned to this parameter if non null</param>
		/// <returns>Return Ok if the scripts were loaded was successfully</returns>
		XEResult LoadScript(const std::vector<std::wstring>& scriptNames, const std::wstring& module, asIScriptModule** asModule = nullptr);

		/// <summary>
		/// Removes a Module from Memory
		/// </summary>
		/// <param name='module'>Module Name to remove</param>
		/// <returns>Return Ok if module was removed was successfully</returns>
		XEResult RemoveModule(const std::wstring& module);

#pragma endregion

};

#endif