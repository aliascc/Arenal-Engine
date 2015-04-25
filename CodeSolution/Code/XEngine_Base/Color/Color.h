/** \file 
 * Contains definitions for use of Colors in XEngine 
 */

/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _COLOR_H
#define _COLOR_H

/**********************
*   System Includes   *
***********************/
#include <array>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"

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
/// Structure that encapsulates Color information
/// </summary>
struct Color
{

	/***********************
	*   Public Variables   *
	************************/
#pragma region Public Variables

	union
	{
		struct
		{
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		};
		uint32_t color;
	};

#pragma endregion

	/***************************************
	*   Constructor & Destructor Methods   *
	****************************************/
#pragma region Constructor & Destructor Methods

	/// <summary>
	/// Default constructor for Color
	/// </summary>
	Color();

	/// <summary>
	/// Constructor for Color
	/// </summary>
	/// <param name="otherColor">Color to copy</param>
	Color(const Color& otherColor);

	/// <summary>
	/// Constructor for Color
	/// </summary>
	/// <param name="r">Amount of red color, ranging from 0 to 255</param>
	/// <param name="g">Amount of green color, ranging from 0 to 255</param>
	/// <param name="b">Amount of blue color, ranging from 0 to 255</param>
	/// <param name="a">Amount of alpha color, ranging from 0 to 255</param>
	Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a);

	/// <summary>
	/// Constructor for Color
	/// </summary>
	/// <param name="r">Amount of red color, ranging from 0 to 1</param>
	/// <param name="g">Amount of green color, ranging from 0 to 1</param>
	/// <param name="b">Amount of blue color, ranging from 0 to 1</param>
	/// <param name="a">Amount of alpha color, ranging from 0 to 1</param>
	Color(float r, float g, float b, float a);

	/// <summary>
	/// Constructor for Color
	/// </summary>
	/// <param name="rgba">Amount of red/green/blue/alpha color, ranging from 0 to 1</param>
	Color(const glm::vec4& rgba);

	/// <summary>
	/// Constructor for Color
	/// </summary>
	/// <param name="otherColor">Full color, it divides the uint32_t into 4 uint8_t, order: blue, green, red, alpha</param>
	Color(uint32_t otherColor);

#pragma endregion
		
	/*************************
	 *   Framework Methods   *
	 *************************/
#pragma region Framework Methods

	/// <summary>
	/// Fills a float array
	/// </summary>
	/// <remarks>
	/// Must be of size 4 or can cause memory corruption
	/// </remarks>
	void FillFloatArray(float* arr) const;

	/// <summary>
	/// Returns a Color uint32_t as RGBA format
	/// </summary>
	/// <returns>Returns uint32_t as RGBA format</returns>
	uint32_t GetColorRGBA() const;

	/// <summary>
	/// Allows to cast Color to a glm::vec4
	/// </summary>
	/// <returns>Returns glm::vec4 vector with the color information</returns>
	operator glm::vec4() const;

	/// <summary>
	/// Allows to cast Color to a glm::ivec4
	/// </summary>
	/// <returns>Returns glm::ivec4 vector with the color information</returns>
	operator glm::ivec4() const;
	
	/// <summary>
	/// Allows to cast Color to a uint32_t
	/// </summary>
	/// <returns>Returns uint32_t with the color information</returns>
	operator uint32_t() const;

	/// <summary>
	/// Allows the assignment operator to accept a glm::vec4 to convert it to a Color
	/// </summary>
	/// <returns>The new color information</returns>
	Color& operator=(const glm::vec4& vec);

	/// <summary>
	/// Allows the assignment operator to accept a glm::ivec4 to convert it to a Color
	/// </summary>
	/// <returns>The new color information</returns>
	Color& operator=(const glm::ivec4& vec);

	/// <summary>
	/// Allows the assignment operator to accept a uint32_t to convert it to a Color
	/// </summary>
	/// <returns>The new color information</returns>
	Color& operator=(uint32_t otherColor);

	/// <summary>
	/// Compares a glm::vec4 with this color
	/// </summary>
	/// <returns>True if they are equal</returns>
	bool operator==(const glm::vec4& vec) const;

	/// <summary>
	/// Compares a glm::vec4 with this color
	/// </summary>
	/// <returns>True if they are not equal</returns>
	bool operator!=(const glm::vec4& vec) const;

	/// <summary>
	/// Compares a uint32_t with this color
	/// </summary>
	/// <returns>True if they are equal</returns>
	bool operator==(uint32_t otherColor) const;

	/// <summary>
	/// Compares a uint32_t with this color
	/// </summary>
	/// <returns>True if they are not equal</returns>
	bool operator!=(uint32_t otherColor) const;

	/// <summary>
	/// Compares a Colors
	/// </summary>
	/// <returns>True if they are equal</returns>
	bool operator==(const Color& otherColor) const;

	/// <summary>
	/// Compares a Colors
	/// </summary>
	/// <returns>True if they are not equal</returns>
	bool operator!=(const Color& otherColor) const;

#pragma endregion

};

#endif