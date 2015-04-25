/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ANGEL_SCRIPT_ADDON_H
#define _ANGEL_SCRIPT_ADDON_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

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

/*****************
*   Class Decl   *
******************/

class AngelScriptAddOn abstract : public XEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Add-on ID 
		/// </summary>
		uint32_t m_AddOnID = 0;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// AngelScriptAddOn Constructor
		/// </summary>
		/// <param name="addOnID">ID of the Add-on to be identified by the engine.</param>
		AngelScriptAddOn(uint32_t addOnID);

		/// <summary>
		/// Default AngelScriptAddOn Destructor
		/// </summary>
		virtual ~AngelScriptAddOn();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline uint32_t GetAddOnID() const
		{
			return m_AddOnID;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Registers the Add-on with the AS Engine.
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		virtual XEResult Register(asIScriptEngine* engine) = 0;

#pragma endregion

};


#endif