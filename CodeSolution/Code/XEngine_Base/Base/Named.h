/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/18/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _XE_NAMED_H
#define _XE_NAMED_H

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
/// Class that Defines a name for the Object with its Gets and Sets
/// </summary>
class Named abstract : public XEObject
{
	protected:

		/***************************
		 *   Protected Variables   *
		 ***************************/
#pragma region Protected Variables

		/// <summary>
		/// Name of XEObject
		/// </summary>
		std::wstring m_Name = L"";

#pragma endregion
		
	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Named Constructor
		/// </summary>
		/// <param name="name">Name of the XEObject</param>
		Named(const std::wstring& name);
		
		/// <summary>
		/// Default Named Destructor
		/// </summary>
		virtual ~Named();

#pragma endregion
		
		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods
		
		/// <summary>
		/// Gets the Name of XEObject
		/// </summary>
		/// <returns>Name of XEObject</returns>
		inline const std::wstring& GetName() const
		{
			return m_Name;
		}

#pragma endregion
		
		/*******************
		 *   Set Methods   *
		 *******************/
#pragma region Set Methods
		
		/// <summary>
		/// Sets the Name of the XEObject
		/// </summary>
		/// <param name="name">Name of the XEObject"</param>
		virtual void SetName(const std::wstring name);

#pragma endregion
		
		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods
		
		/// <summary>
		/// Converts the Objects to a String. In case of Named it is the name
		/// </summary>
		/// <returns>Name of XEObject</returns>
		virtual std::wstring ToString() const override;

#pragma endregion

};

#endif