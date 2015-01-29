
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include "MemLeaks.h"

/*********************
*   Function Decls   *
**********************/
#if defined( _DEBUG )

bool MemLeaks::MemoryBegin()
{
	_CrtSetDbgFlag (  _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(563);
	//_CrtSetBreakAlloc(95726);
	//_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//atexit(FileStringDestroy);
	//atexit(DumpLeaks);

	return true;
}

bool MemLeaks::MemoryEnd()
{
	return true;
}

#endif
