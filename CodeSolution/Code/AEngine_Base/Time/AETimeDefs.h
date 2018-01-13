/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#ifndef _AE_TIME_DEFS_H
#define _AE_TIME_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct TimerParams sealed : public AEObject
{
	/***********************
	*   Public Variables   *
	************************/
#pragma region Private Variables

	/// <summary>
	/// Elapsed Time since Last Frame
	/// </summary>
	double m_ElapsedTime = 0.0;

	/// <summary>
	/// Elapsed Time since the beginning of the frame
	/// </summary>
	double m_PreviousCallTime = 0.0;

	/// <summary>
	/// Total running time since the start of the Game App
	/// </summary>
	double m_TotalElapsedTime = 0.0;

#pragma endregion

	/***************************************
	*   Constructor & Destructor Methods   *
	****************************************/
#pragma region Constructor & Destructor Methods

	/// <summary>
	/// Default TimerParams Constructor
	/// </summary>
	TimerParams();

#pragma endregion

};

struct TimeStamp sealed : public AEObject
{
	/***********************
	*   Public Variables   *
	************************/
#pragma region Private Variables

	/// <summary>
	/// Record the day number of the time stamp
	/// </summary>
	uint32_t m_Day = 0;

	/// <summary>
	/// Record the month number of the time stamp
	/// </summary>
	uint32_t m_Month = 0;

	/// <summary>
	/// Record the Year of the time stamp
	/// </summary>
	uint32_t m_Year = 0;

	/// <summary>
	/// Record the hour of the time stamp
	/// </summary>
	uint32_t m_Hour = 0;

	/// <summary>
	/// Record the minute of the time stamp
	/// </summary>
	uint32_t m_Minute = 0;

	/// <summary>
	/// Record the second of the time stamp
	/// </summary>
	uint32_t m_Second = 0;

#pragma endregion

	/***************************************
	*   Constructor & Destructor Methods   *
	****************************************/
#pragma region Constructor & Destructor Methods

	/// <summary>
	/// Default TimeStamp Constructor
	/// </summary>
	TimeStamp();

	/// <summary>
	/// TimeStamp Constructor
	/// </summary>
	TimeStamp(const std::wstring& str);

#pragma endregion

	/************************
	*   Framework Methods   *
	*************************/
#pragma region Framework Methods

	/// <summary>
	/// Function to convert from String to TimeStamp
	/// </summary>
	/// <remarks>
	/// Format has to be like this: 'Year-Month-Day Hour:Minute:Second'
	/// </remarks>
	/// <param name="str">String to parse and convert to TimeStamp</param>
	/// <returns>AEResult::Ok if parse was successful</returns>
	AEResult GetFromString(const std::wstring& str);

#pragma endregion

	/*********************************
	*   Virtual Framework Methods   *
	**********************************/
#pragma region Virtual Framework Methods

	/// <summary>
	/// Overload operator to convert from String to TimeStamp
	/// </summary>
	/// <remarks>
	///	Uses GetFromString function for the conversion. If parse is unsuccessful it will not change the value of the TimeStampt
	/// </remarks>
	/// <param name="str">String to parse and convert to TimeStamp</param>
	/// <returns>Returns Time Stamp</returns>
	TimeStamp& operator=(const std::wstring& str);

	/// <summary>
	/// Equal operator overload
	/// </summary>
	bool operator==(const TimeStamp& other);

	/// <summary>
	/// Less than operator overload
	/// </summary>
	bool operator<(const TimeStamp& other);

	/// <summary>
	/// Greater than operator overload
	/// </summary>
	bool operator>(const TimeStamp& other);

	/// <summary>
	/// Less than or equal operator overload
	/// </summary>
	bool operator<=(const TimeStamp& other);

	/// <summary>
	/// Greater than or equal operator overload
	/// </summary>
	bool operator>=(const TimeStamp& other);

	/// <summary>
	/// To String method override, Gets Time Stamp as a string
	/// </summary>
	/// <returns>Returns Time Stamp as a string</returns>
	std::wstring ToString() const override;

#pragma endregion

};

#endif