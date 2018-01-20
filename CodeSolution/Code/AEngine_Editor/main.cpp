/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include "AEngine_editor.h"
#include "Crash Handling\CrashHandler.h"
#include "Crash Handling\CrashHandlerDefs.h"

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

    if (CrashHandlerInst->InitCrashHandling() != AEResult::Ok)
    {
        AETODO("Add error window");
        return EXIT_FAILURE;
    }

	QApplication a(argc, argv);
	AEngine_Editor w;
	
	w.show();

    int retCode = 0;

    if (w.IsInitialized())
    {
        retCode = a.exec();

        CrashHandlerInst->DeinitCrashHandling();
    }
    else
    {
        CrashHandlerInst->DeinitCrashHandling();

        a.exit();
    }

    return EXIT_SUCCESS;
}
