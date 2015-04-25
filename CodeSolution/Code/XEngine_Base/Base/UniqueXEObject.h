/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _UNIQUE_XE_OBJECT_H
#define _UNIQUE_XE_OBJECT_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "XEObject.h"

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
/// Class that contains a Unique ID
/// </summary>
class UniqueXEObject abstract : public XEObject
{
	private:

		/*************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

		/// <summary>
		/// Unique ID of the Object
		/// </summary>
		uint64_t m_UniqueID = 0;

#pragma endregion
		
	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default UniqueXEObject Constructor
		/// </summary>
		UniqueXEObject();

		/// <summary>
		/// Default UniqueXEObject Destructor
		/// </summary>
		virtual ~UniqueXEObject();

#pragma endregion

		/*********************
		 *     Get Methods   *
		 *********************/
#pragma region Get Methods

		/// <summary>
		/// Gets Unique ID
		/// </summary>
		/// <returns>Gets the Unique ID of the Object</returns>
		inline uint64_t GetUniqueID() const
		{
			return m_UniqueID;
		}

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

#pragma endregion

};

#endif