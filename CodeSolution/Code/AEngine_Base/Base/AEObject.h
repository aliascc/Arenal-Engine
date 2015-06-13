/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AE_OBJECT_H
#define _AE_OBJECT_H

/**********************
*   System Includes   *
***********************/
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Base class for most of all AEngine Classes or structs
/// </summary>
class AEObject abstract
{
	protected:

		/***************************
		 *   Protected Variables   *
		 ***************************/
#pragma region Protected Variables

#pragma endregion
		
	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// AEObject Constructor
		/// </summary>
		AEObject();
		
		/// <summary>
		/// Default AEObject Destructor
		/// </summary>
		virtual ~AEObject();

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

#pragma endregion

		/*******************
		 *   Set Methods   *
		 *******************/
#pragma region Set Methods

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Converts the Objects to a String. 
		/// </summary>
		/// <returns>Object in String format</returns>
		virtual std::wstring ToString() const;

#pragma endregion

};

#endif