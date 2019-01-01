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
#include "precomp_base.h"

#ifdef AE_MEM_CHECK

/***************************
*   Game Engine Includes   *
****************************/
#include "MemLeaks.h"

/*********************
*   Function Decls   *
**********************/

bool MemLeaks::MemoryBegin()
{
    _CrtSetDbgFlag (  _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    //_CrtSetBreakAlloc(201);
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

#endif //AE_MEM_CHECK
