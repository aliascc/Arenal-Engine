
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/
#pragma once

#ifndef _ERROR_CALLBACK_PHYSX_H
#define _ERROR_CALLBACK_PHYSX_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "PxPhysicsAPI.h"

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class ErrorCallbackPhysX sealed : public physx::PxErrorCallback
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default ErrorCallbackPhysX Constructor
		/// </summary>
		ErrorCallbackPhysX();

		/// <summary>
		/// Default ErrorCallbackPhysX Destructor
		/// </summary>
		virtual ~ErrorCallbackPhysX();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);

#pragma endregion

};

#endif