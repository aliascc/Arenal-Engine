/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/22/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _CONSOLE_DEFS_H
#define _CONSOLE_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "AngelScript-Add-Ons\scriptarray\scriptarray.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\Color.h"
#include "Base\XEObject.h"

/**************
*   Defines   *
***************/

/// <summary>
/// Console Max Lines to preserve in history
/// </summary>
#define XE_CONSOLE_MAX_PRINT_HISTORY		50

/// <summary>
/// Console Max Command History to preserve
/// </summary>
#define XE_CONSOLE_MAX_CMD_HISTORY			10

/// <summary>
/// Console Script Module Name
/// </summary>
#define XE_CONSOLE_SCRIPT_MOD_NAME			L"XE_Console_Module"

/// <summary>
/// Max Console Line Chars
/// </summary>
#define XE_CONSOLE_DEFAULT_TIME_UNDERSCORE	0.5f

/// <summary>
/// Max Console Line Chars
/// </summary>
#define XE_CONSOLE_MAX_LINE_CHARS			128

/// <summary>
/// Console Max Console Line
/// </summary>
#define XE_CONSOLE_LINE_MEM_SIZE			sizeof(wchar_t) * XE_CONSOLE_MAX_LINE_CHARS

/// <summary>
/// Default Console Present Time
/// </summary>
#define XE_CONSOLE_DEFAULT_PRESENT_TIME		0.5f

/// <summary>
/// Default Console Height
/// </summary>
#define XE_CONSOLE_DEFAULT_HEIGHT			150

/// <summary>
/// Delay between each char erase
/// </summary>
#define XE_CONSOLE_BACKSPACE_TIME			0.075f

/// <summary>
/// Delay between each first char erase and the rest
/// </summary>
#define XE_CONSOLE_BACKSPACE_FIRST_TIME		-0.4f

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/****************
*   Constants   *
*****************/
extern const Color ConsoleBackgroundDefaultColor;

/******************
*   Struct Decl   *
*******************/
struct ConsoleShowLine sealed : public XEObject
{
	std::vector<std::wstring>	m_ShowCols;
	std::vector<Color>			m_Colors;

	ConsoleShowLine();
};

#endif