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

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "LoggerDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/****************
*   AELastLog   *
*****************/
AELastLog::AELastLog()
{
}

/************
*   AELog   *
*************/
AELog::AELog()
{
}

std::wstring AELog::ToString() const
{
    std::wstring fileLine = fmt::format(L"{0}-({1}): {2}- \'{3}\'\n", m_LogLine, m_TimeStamp.ToString(), AELogHelpers::LogLevelStr(m_Level), m_Log);

    return fileLine;
}

AELog& AELog::operator=(const AELog &other)
{
    m_Log           = other.m_Log;
    m_Level         = other.m_Level;
    m_LogLine       = other.m_LogLine;
    m_TimeStamp     = other.m_TimeStamp;

    return *this;
}

/*******************
*   AELogHelpers   *
********************/
namespace AELogHelpers
{

    std::wstring AELogHelpers::LogLevelStr(LogLevel lvl)
    {
        switch (lvl)
        {
            case LogLevel::None:
                return L"NONE";

            case LogLevel::Info:
                return L"INFO";

            case LogLevel::Warning:
                return L"WARNING";

            case LogLevel::Error:
                return L"ERROR";

            case LogLevel::Debug:
                return L"DEBUG";
        }

        AEAssert(false);

        return L"";
    }

    LogLevel AELogHelpers::Str2LogLevel(std::wstring lvl)
    {
        if (lvl.compare(L"NONE") == 0)
        {
            return LogLevel::None;
        }
        else if (lvl.compare(L"INFO") == 0)
        {
            return LogLevel::Info;
        }
        else if (lvl.compare(L"WARNING") == 0)
        {
            return LogLevel::Warning;
        }
        else if (lvl.compare(L"ERROR") == 0)
        {
            return LogLevel::Error;
        }
        else if (lvl.compare(L"DEBUG") == 0)
        {
            return LogLevel::Debug;
        }

        AEAssert(false);

        return LogLevel::None;
    }

}
