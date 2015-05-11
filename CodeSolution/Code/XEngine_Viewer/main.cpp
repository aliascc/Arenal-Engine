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

/***************************
*   Game Engine Includes   *
****************************/
#include "ViewerApp.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	std::wstring errorMsg = L"";

	ViewerApp app(hInstance);

	//if(app.InitGameApp(errorMsg) != XEResult::Ok)
	{
		MessageBox(0, errorMsg.c_str(), 0, 0);

		return EXIT_FAILURE;
	}

	app.Run();

	return EXIT_SUCCESS;
}
