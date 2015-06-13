/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _BASE_ADD_ON_AS_H
#define _BASE_ADD_ON_AS_H

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

class BaseAddOnAS sealed : public AngelScriptAddOn
{
	private:

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		/// <summary>
		/// Registers Basic AEngine Constants with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		AEResult RegisterBasicEngineConst(asIScriptEngine* engine);

		/// <summary>
		/// Registers the AEResult Enum with the AS Engine.
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		AEResult RegisterAEResultEnum(asIScriptEngine* engine);

		/// <summary>
		/// Registers Basic AEngine Functions with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		AEResult RegisterBasicEngineFuncs(asIScriptEngine* engine);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// BaseAddOnAS Constructor
		/// </summary>
		BaseAddOnAS();

		/// <summary>
		/// Default BaseAddOnAS Destructor
		/// </summary>
		virtual ~BaseAddOnAS();

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
		AEResult Register(asIScriptEngine* engine) override;

#pragma endregion

};

#endif