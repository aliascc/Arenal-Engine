/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <new>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ScriptConsoleLine.h"

/********************
*   Function Defs   *
*********************/
ScriptConsoleLine::ScriptConsoleLine()
{
}

ScriptConsoleLine::ScriptConsoleLine(const ScriptConsoleLine& other)
{
	*this = other;
}

ScriptConsoleLine::~ScriptConsoleLine()
{
	ReleaseAngel(m_SA_Strings);
	ReleaseAngel(m_SA_Colors);
}

ScriptConsoleLine& ScriptConsoleLine::operator=(const ScriptConsoleLine& other)
{
	ReleaseAngel(m_SA_Strings);
	ReleaseAngel(m_SA_Colors);

	if (other.m_SA_Strings != nullptr)
	{
		m_SA_Strings = other.m_SA_Strings;
		other.m_SA_Strings->AddRef();
	}

	if (other.m_SA_Colors != nullptr)
	{
		m_SA_Colors = other.m_SA_Colors;
		other.m_SA_Colors->AddRef();
	}

	return *this;
}

void ScriptConsoleLine::Constructor(ScriptConsoleLine* self)
{
	new(self) ScriptConsoleLine();
}

void ScriptConsoleLine::Destructor(ScriptConsoleLine* self)
{
	self->~ScriptConsoleLine();
}
