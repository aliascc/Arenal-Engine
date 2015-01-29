/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/14/2012
*
* Desc:
*
*
* Copyright 2012
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

	ViewerApp app(hInstance, XE_CONFIG_ENGINE_FILE_PATH);

	//if(app.InitGameApp(errorMsg) != XEResult::Ok)
	{
		MessageBox(0, errorMsg.c_str(), 0, 0);

		return EXIT_FAILURE;
	}

	app.Run();

	return EXIT_SUCCESS;
}
