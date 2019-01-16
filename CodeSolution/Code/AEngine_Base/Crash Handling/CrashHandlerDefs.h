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

#ifndef _CRASH_HANDLER_DEFS_H
#define _CRASH_HANDLER_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/**************
*   Defines   *
***************/

// From PhysX this exception is used when 'setting the thread name in Microsoft debuggers'
// We omit the crash handler when we this this type of exception
// NS_MS_VC_EXCEPTION 0x406D1388

// All exception codes that would cause a crash start after
// https://docs.microsoft.com/en-us/windows/desktop/Debug/getexceptioncode
// https://docs.microsoft.com/en-us/windows/desktop/LearnWin32/error-codes-in-com
// https://stackoverflow.com/a/12300563/1967659
#define CRASH_HANDLER_MAX_INFORMATION_EXCEPTION_CODE 0x80000000L


/// <summary>
/// Define for Crash Handler Singleton Instance Get Call 
/// </summary>
#define CrashHandlerInst          CrashHandler::GetInstance()

#endif