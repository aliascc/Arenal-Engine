/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 9/15/2014
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _LOCALIZATION_ADD_ON_AS_H
#define _LOCALIZATION_ADD_ON_AS_H

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

class LocalizationAddOnAS sealed : public AngelScriptAddOn
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Methods

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		/// <summary>
		/// Registers Localization Manager Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterLocalizationObject(asIScriptEngine* engine);

		/// <summary>
		/// Registers Localization Manager Object Pointer with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterLocalizationObjectPointer(asIScriptEngine* engine);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// LocalizationAddOnAS Constructor
		/// </summary>
		LocalizationAddOnAS();

		/// <summary>
		/// Default LocalizationAddOnAS Destructor
		/// </summary>
		virtual ~LocalizationAddOnAS();

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