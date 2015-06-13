/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VIEWPORT_H
#define _VIEWPORT_H

/**********************
*   System Includes   *
***********************/
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class Viewport sealed : public AEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// DirectX Viewport
		/// </summary>
		D3D11_VIEWPORT m_ViewportDX;

		/// <summary>
		/// X Position of the Top of the Viewport
		/// </summary>
		float m_TopLeftX = 0.0f;

		/// <summary>
		/// Y Position of the Top of the Viewport
		/// </summary>
		float m_TopLeftY = 0.0f;

		/// <summary>
		/// Width of the Viewport
		/// </summary>
		float m_Width = 0.0f;

		/// <summary>
		/// Height of the Viewport
		/// </summary>
		float m_Height = 0.0f;

		/// <summary>
		/// Min depth of the viewport. Ranges between 0 and 1
		/// </summary>
		float m_MinDepth = 0.0f;

		/// <summary>
		/// Max depth of the viewport. Ranges between 0 and 1
		/// </summary>
		float m_MaxDepth = 0.0f;

#pragma endregion
		
	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Viewport Constructor
		/// </summary>
		Viewport();

		/// <summary>
		/// Default Viewport Destructor
		/// </summary>
		virtual ~Viewport();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets DX Viewport
		/// </summary>
		/// <returns>Corners of the Frustum</returns>
		inline const D3D11_VIEWPORT& GetViewportDX() const
		{
			return m_ViewportDX;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Initialize the Viewport
		/// </summary>
		/// <param name="topLeftX">X Position of the Top of the Viewport</param>
		/// <param name="topLeftY">Y Position of the Top of the Viewport</param>
		/// <param name="width">Width of the Viewport</param>
		/// <param name="height">Height of the Viewport</param>
		/// <param name="minDepth">Min depth of the viewport</param>
		/// <param name="maxDepth">Max depth of the viewport</param>
		/// <returns>Returns AEResult::Ok if successful</returns>
		AEResult Initialize(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth);

#pragma endregion

};

#endif