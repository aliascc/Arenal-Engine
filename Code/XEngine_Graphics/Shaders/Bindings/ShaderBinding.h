
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/7/2103
* 
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _SHADER_BINDING_H
#define _SHADER_BINDING_H

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
#include "Base\Base.h"
#include "Base\UniqueXEObjectNamed.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class ShaderBinding abstract : public UniqueXEObjectNamed
{
	protected:

		/****************************
		 *   Protected Variables    *
		 ****************************/
#pragma region Protected Variables
				
		/// <summary>
		/// Index to where to bind
		/// </summary>
		uint32_t m_BindIndex = 0;

#pragma endregion

	public:
		/*****************************************
		 *   Constructor & Destructor Methods   *
		 *****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// ShaderBinding Constructor
		/// </summary>
		/// <param name="name">Name of the Binding</param>
		/// <param name="bindIndex">Index where to bind</param>
		ShaderBinding(const std::wstring& name, uint32_t bindIndex);

		/// <summary>
		/// Default ShaderBinding Destructor
		/// </summary>
		virtual ~ShaderBinding();

#pragma endregion

		/********************
		 *   Get Methods    *
		 ********************/
#pragma region Get Methods

		/// <summary>
		/// Gets Bind Index
		/// </summary>
		/// <returns>Bind Index</returns>
		inline uint32_t GetBindIndex() const 
		{ 
			return m_BindIndex; 
		}

#pragma endregion

		/********************
		 *   Set Methods    *
		 ********************/
#pragma region Set Methods

		/// <summary>
		/// Sets the Bind Index to where to bind to the shader
		/// </summary>
		/// <param name="bindIndex">New Bind Index</param>
		inline void SetBindIndex(uint32_t bindIndex) 
		{ 
			m_BindIndex = bindIndex; 
		}

#pragma endregion

};

#endif