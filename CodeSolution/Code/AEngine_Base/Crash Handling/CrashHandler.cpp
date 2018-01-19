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

/**********************
*   System Includes   *
***********************/
#include <Windows.h>
#include <DbgHelp.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"
#include "boost\filesystem.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "CrashHandler.h"
#include "Time\AETimeDefs.h"
#include "CrashHandlerDefs.h"
#include "Base\BaseLocations.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/*****************
*   Class Decl   *
******************/

CrashHandler::CrashHandler()
{
}

CrashHandler::~CrashHandler()
{
}

LONG CrashHandler::ExceptionVectorHandler(_In_ PEXCEPTION_POINTERS exceptionInfo)
{
    //Check to see if the exception was handled in the code
    if (exceptionInfo->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE)
    {
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    //Create Core Dump
    CrashHandlerInst->CreateCoreDump(exceptionInfo);

    //Exit Process with failure
    exit(EXIT_FAILURE);

    //No return as we exit the program
}

void CrashHandler::CreateCoreDump(PEXCEPTION_POINTERS exceptionInfo)
{
    if (!m_IsReady)
    {
        AETODO("Open Message Window with error");
        return;
    }

    //Core Dump Flags
    const uint32_t flags = MiniDumpNormal | MiniDumpWithDataSegs | MiniDumpWithIndirectlyReferencedMemory;
    
    //Core Dump Exception Information
    MINIDUMP_EXCEPTION_INFORMATION expParam = { 0 };
    expParam.ThreadId = GetCurrentThreadId();
    expParam.ExceptionPointers = exceptionInfo;
    expParam.ClientPointers = FALSE;

    //Get Date
    TimeStamp timeStamp;
    AE_Base::GetDate(timeStamp);    

    //Get Core Dump File
    std::wstring coreDumpFileName = fmt::format(AE_CONFIG_ENGINE_CORE_DUMP, timeStamp.ToString());

    //Open File using Window Handles
    HANDLE hFile = CreateFile(coreDumpFileName.c_str(), GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == nullptr)
    {
        AETODO("Open Message Window with error");
        return;
    }

    //Create Dump File
    BOOL Result = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, (MINIDUMP_TYPE)flags, &expParam, nullptr, nullptr);
    if (Result == FALSE)
    {
        AETODO("Open Message Window with error");
    }

    //Close File
    CloseHandle(hFile);
}

AEResult CrashHandler::InitCrashHandling()
{
    if (m_IsReady)
    {
        return AEResult::Ok;
    }

    //0 = Call Last
    m_ExpceptionHandler = AddVectoredExceptionHandler(0, ExceptionVectorHandler);

    if (m_ExpceptionHandler == nullptr)
    {
        return AEResult::ExceptionHandledFailed;
    }

    m_IsReady = true;

    return AEResult::Ok;
}

void CrashHandler::DeinitCrashHandling()
{
    if (!m_IsReady)
    {
        return;
    }

    RemoveVectoredExceptionHandler(m_ExpceptionHandler);

    m_ExpceptionHandler = nullptr;

    m_IsReady = false;
}