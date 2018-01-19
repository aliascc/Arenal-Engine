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

#pragma once

#ifndef _CRASH_HANDLER_H
#define _CRASH_HANDLER_H

/**********************
*   System Includes   *
***********************/
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Utils\Singleton.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/****************
*   Class Def   *
*****************/

class CrashHandler : public Singleton<CrashHandler>
{
    friend class Singleton<CrashHandler>;

    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        bool m_IsReady = false;

        PVOID m_ExpceptionHandler = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        static LONG CALLBACK ExceptionVectorHandler(_In_ PEXCEPTION_POINTERS exceptionInfo);

        void CreateCoreDump(PEXCEPTION_POINTERS exceptionInfo);

#pragma endregion

        /***********************************************
        *   Private Constructor & Destructor Methods   *
        ************************************************/
#pragma region Private Constructor & Destructor Methods

        /// <summary>
        /// Protected constructor
        /// </summary>
        CrashHandler();

        /// <summary>
        /// Default CrashHandler Destructor
        /// </summary>
        virtual ~CrashHandler();

#pragma endregion

    public:

        /*************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult InitCrashHandling();

        void DeinitCrashHandling();

#pragma endregion

};

#endif