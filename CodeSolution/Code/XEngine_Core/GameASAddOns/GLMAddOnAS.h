/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GLM_ADD_ON_AS_H
#define _GLM_ADD_ON_AS_H

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

/*************
*   Define   *
**************/
#define XE_AS_MATH_HELPERS_NAMESPACE	"Math"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class GLMAddOnAS sealed : public AngelScriptAddOn
{
	private:

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		/// <summary>
		/// Registers GLM by a Native Interface
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGLM_Native(asIScriptEngine* engine);

		/// <summary>
		/// Registers GLM Objects by a Native Interface
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGLM_Objects_Native(asIScriptEngine* engine);

		/// <summary>
		/// Registers GLM Vector 3 Functions by a Native Interface
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGLM_Vec3_Functions_Native(asIScriptEngine* engine);

		/// <summary>
		/// Registers GLM Vector 4 Functions by a Native Interface
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGLM_Vec4_Functions_Native(asIScriptEngine* engine);

		/// <summary>
		/// Registers GLM Matrix 4x4 Functions by a Native Interface
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGLM_Mat4_Functions_Native(asIScriptEngine* engine);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GLMAddOnAS Constructor
		/// </summary>
		GLMAddOnAS();

		/// <summary>
		/// Default GLMAddOnAS Destructor
		/// </summary>
		virtual ~GLMAddOnAS();

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