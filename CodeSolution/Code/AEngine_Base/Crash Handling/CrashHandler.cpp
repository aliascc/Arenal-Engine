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

#ifdef AE_CRASH_HANDILING

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

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
    if (exceptionInfo->ExceptionRecord->ExceptionCode < CRASH_HANDLER_MAX_INFORMATION_EXCEPTION_CODE)
    {
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    std::string coreDumpFileName = "";

    if (!CrashHandlerInst.CreateCoreDump(exceptionInfo, coreDumpFileName))
    {
        return  EXCEPTION_CONTINUE_SEARCH;
    }

    std::string crashMsg = "Crash detected. Core Dump loaded into: " + coreDumpFileName;
    MessageBox(0, crashMsg.c_str(), 0, 0);

    //Exit Process with failure
    exit(EXIT_FAILURE);

    //No return as we exit the program
}

bool CrashHandler::CreateCoreDump(PEXCEPTION_POINTERS exceptionInfo, std::string& coreDumpFileName)
{
    if (!m_IsReady)
    {
        MessageBox(0, "Crash Handling failed to report the crash correctly as it was not initialized.", 0, 0);
        return false;
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
    std::string timeStr = "";
    timeStamp.GetTimeString(timeStr, false);
    coreDumpFileName = fmt::format(AE_CONFIG_ENGINE_CORE_DUMP, timeStr);

    //Open File using Window Handles
    HANDLE hFile = CreateFile(coreDumpFileName.c_str(), GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == nullptr)
    {
        std::string errMsg = "Crash Handling failed to report the crash correctly as it could not open target file to load the core dump into: " + coreDumpFileName;
        MessageBox(0, errMsg.c_str(), 0, 0);
        return false;
    }

    //Create Dump File
    std::string errMsg = "Crash Handling failed to report the crash correctly as it could not open target file to load the core dump into: " + coreDumpFileName;
    BOOL Result = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, (MINIDUMP_TYPE)flags, &expParam, nullptr, nullptr);
    CloseHandle(hFile);

    if (Result == FALSE)
    {
        std::string errMsg = "Crash Handling failed to report the crash correctly as it could not write the core dump into the file: " + coreDumpFileName;
        MessageBox(0, errMsg.c_str(), 0, 0);
        return false;
    }

    return true;
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

#endif //AE_CRASH_HANDILING