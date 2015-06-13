/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _NAMED_H
#define _NAMED_H

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
#include "AEObject.h"

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
class Named abstract : public AEObject
{
	protected:

		/***************************
		 *   Protected Variables   *
		 ***************************/
#pragma region Protected Variables

		/// <summary>
		/// Name of AEObject
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
		/// <param name="name">Name of the AEObject</param>
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
		/// Gets the Name of AEObject
		/// </summary>
		/// <returns>Name of AEObject</returns>
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
		/// Sets the Name of the AEObject
		/// </summary>
		/// <param name="name">Name of the AEObject"</param>
		virtual void SetName(const std::wstring name);

#pragma endregion
		
		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods
		
		/// <summary>
		/// Converts the Objects to a String. In case of Named it is the name
		/// </summary>
		/// <returns>Name of AEObject</returns>
		virtual std::wstring ToString() const override;

#pragma endregion

};

#endif