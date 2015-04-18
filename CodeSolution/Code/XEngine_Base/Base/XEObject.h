/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 5/18/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _XE_OBJECT_H
#define _XE_OBJECT_H

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
/// Base class for most of all XEngine Classes or structs
/// </summary>
class XEObject abstract
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
		/// XEObject Constructor
		/// </summary>
		XEObject();
		
		/// <summary>
		/// Default XEObject Destructor
		/// </summary>
		virtual ~XEObject();

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