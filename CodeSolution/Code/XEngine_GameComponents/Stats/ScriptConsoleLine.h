/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SCRIPT_CONSOLE_LINE_DEFS_H
#define _SCRIPT_CONSOLE_LINE_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "AngelScript-Add-Ons\scriptarray\scriptarray.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct ScriptConsoleLine sealed : public XEObject
{
	CScriptArray* m_SA_Strings = nullptr;
	CScriptArray* m_SA_Colors = nullptr;

	//Contructor
	ScriptConsoleLine();

	ScriptConsoleLine(const ScriptConsoleLine& other);

	virtual ~ScriptConsoleLine();

	ScriptConsoleLine& operator=(const ScriptConsoleLine& other);

	static void Constructor(ScriptConsoleLine* self);

	static void Destructor(ScriptConsoleLine* self);
};

#endif