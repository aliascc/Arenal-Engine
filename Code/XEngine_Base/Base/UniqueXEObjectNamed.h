/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/21/2013
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _UNIQUE_XE_OBJECT_NAMED_H
#define _UNIQUE_XE_OBJECT_NAMED_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Named.h"

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
/// Class that contains a Unique ID also name for the Object with its Gets and Sets
/// </summary>
class UniqueXEObjectNamed abstract : public Named
{
	private:

		/**************************
		 *   Private Variables   *
		 **************************/
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
		/// Default UniqueXEObjectNamed Constructor
		/// </summary>
		/// <param name="name">Name of the XEObject</param>
		UniqueXEObjectNamed(const std::wstring& name = L"");

		/// <summary>
		/// Default UniqueXEObjectNamed Destructor
		/// </summary>
		virtual ~UniqueXEObjectNamed();

#pragma endregion

		/**********************
		 *     Get Methods    *
		 **********************/
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