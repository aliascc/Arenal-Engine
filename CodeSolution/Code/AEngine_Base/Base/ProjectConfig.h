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

#pragma once

#ifndef _PROJECT_CONFIG_H
#define _PROJECT_CONFIG_H

/*
* Autogenerated!
* Do not change the options in this file!
*/

/*****************
*   Pre-checks   *
******************/

//Check that the compiler is not defining NDEBUG and _DEBUG at the same time
#if !defined(NDEBUG) ^ defined(_DEBUG)
#error NDEBUG and _DEBUG cannot be defined at the same time.
#endif

/***********************
*   3rd Party Config   *
************************/

//GLM configuration
#define GLM_FORCE_RADIANS

/*********************
*   Project Config   *
**********************/

//Defines if the build is a final release
//#define AE_FINAL

#if defined(AE_FINAL) && defined(_DEBUG)
#error AE_FINAL and _DEBUG cannot be defined at the same time.
#endif

#if !defined(AE_FINAL)

    //Defines Editor Mode
    #define AE_EDITOR_MODE

    //Defines DirectX Debug Mode
    #define AE_GRAPHIC_DEBUG_DEVICE

    //Defines if extra information is to be printed on when getting a system error (HRESULT)
    #define AE_HRESULT_EXTRA_INFO

    //Defines if assert statements are active
    #define AE_ENABLE_ASSERT

    //Defines if instead of asserting to raise a debug break point
    #define AE_ASSERT_DBG_BRK

    //Memory monitor to check if we have memory leaks
    #define AE_MEM_CHECK

    //If we are debugging and we are inspecting a break point 
    //it is very likely that elapsed time will be more than a sec
    //we do not want our engine to go faster than that, so do not
    //let it pass a sec
    #define AE_TIME_STEP_LOCK

    //Uncomment for verbose deleting and releasing
    //or define in the CPP where delete or release is
    //taking place
    //#define AE_DEBUG_DEL_PRINT_VAR_NAME
    //#define AE_DEBUG_REL_PRINT_VAR_NAME
#endif

#endif