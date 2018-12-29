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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_editor.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

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
    AEngine_Editor w;
    
    w.show();

    int retCode = 0;

    if (w.IsInitialized())
    {
        retCode = a.exec();
    }
    else
    {
        a.exit();
    }

    return EXIT_SUCCESS;
}
