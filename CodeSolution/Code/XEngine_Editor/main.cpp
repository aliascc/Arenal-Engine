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
#include <Windows.h>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets/QApplication"

/***************************
*   Game Engine Includes   *
****************************/
#include "xengine_editor.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
int main(int argc, char *argv[])
{
	#if defined(_DEBUG)
		MemLeaks::MemoryBegin();
	#endif

	QApplication a(argc, argv);
	XEngine_Editor w;
	
	w.show();

	return a.exec();
}