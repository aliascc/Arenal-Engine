/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MEMLEAKS_H
#define _MEMLEAKS_H

#if defined( _DEBUG )

/**********************
*   System Includes   *
***********************/
#include <map>
#include <list>
#include <string>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#define DBG_NEW new (_NORMAL_BLOCK, __FILE__ , __LINE__)
#define new DBG_NEW

/********************
*   Function Defs   *
*********************/
namespace  MemLeaks
{
	extern bool MemoryBegin();
	extern bool MemoryEnd();
}

#endif

#endif