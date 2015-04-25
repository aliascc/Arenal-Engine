/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LOG_ADD_ON_AS_H
#define _LOG_ADD_ON_AS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "AngelScript\Add-ons\AngelScriptAddOn.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class LogAddOnAS sealed : public AngelScriptAddOn
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		/// <summary>
		/// Registers Log Level Enum with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterLogLevelEnum(asIScriptEngine* engine);

		/// <summary>
		/// Registers XELog Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterXELogObject(asIScriptEngine* engine);

		/// <summary>
		/// Registers Logger Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterLoggerObject(asIScriptEngine* engine);

		/// <summary>
		/// Registers Logger Object Pointer with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterLoggerObjectPointer(asIScriptEngine* engine);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// LogAddOnAS Constructor
		/// </summary>
		LogAddOnAS();

		/// <summary>
		/// Default LogAddOnAS Destructor
		/// </summary>
		virtual ~LogAddOnAS();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Registers the Add-on with the AS Engine.
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		XEResult Register(asIScriptEngine* engine) override;

#pragma endregion

};

#endif