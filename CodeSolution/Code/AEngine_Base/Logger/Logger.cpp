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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_base.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "angelscript.h"
#include "fmt\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Logger.h"
#include "LoggerDefs.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Logger::Logger()
{
    m_RepeatLogLvlThread = std::thread(&Logger::MonitorRepeatedLogTimeOut, this);
}

Logger::~Logger()
{
    m_IsRunning = false;

    m_LogConditionalVar.notify_all();

    m_RepeatLogLvlThread.join();

    std::lock_guard<std::mutex> lock(m_LogMutex);

    if(m_FileStream.is_open())
    {
        m_FileStream.flush();
        m_FileStream.close();
    }
}

AEResult Logger::ActivateLogToFile()
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    if(m_LogToFile)
    {
        return AEResult::Ok;
    }

    AEAssert(!m_LogFilename.empty());
    if (m_LogFilename.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_FileStream = std::ofstream(m_LogFilename, std::ios::out | std::ios::app);

    if(!m_FileStream.is_open())
    {
        return AEResult::OpenFileFail;
    }

    AETODO("Remove this from here and replace");
    const std::string beginFile = "----------------------------------------------------\n";
    m_FileStream.write(beginFile.c_str(), beginFile.size());

    m_LogToFile = true;

    return AEResult::Ok;
}

AEResult Logger::DeactivateLogToFile()
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    if(!m_LogToFile)
    {
        return AEResult::Ok;
    }

    m_FileStream.close();

    m_LogToFile = false;

    return AEResult::Ok;
}

AEResult Logger::SetLogFilename(const std::string& path)
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    AEAssert(!path.empty());
    if(path.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_LogFilename = path;

    return AEResult::Ok;
}

void Logger::SetCapacity(uint32_t capacity)
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    m_Capacity = capacity; 
}

void Logger::SetLogLevel(LogLevel logLevel)
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    m_LogLevel = logLevel; 
}

void Logger::SetScriptAELogArrayType(asITypeInfo* scriptAELogArrayType)
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    m_ScriptAELogArrayType = scriptAELogArrayType;
}

bool Logger::ErrorsExist () const
{ 
    return m_Errors;
}

bool Logger::WarningsExist () const
{ 
    return m_Warnings;
}

void Logger::ResetErrors()
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    m_Errors = false; 
}

void Logger::ResetWarning()
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    m_Warnings = false; 
}

void Logger::AddNewLog(LogLevel logLevel, LogSystem logSystem, const std::string& msg)
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    if ( (m_LogSystem & logSystem) == LogSystem::None || m_LogLevel < logLevel)
    {
        return;
    }

    bool printSameLog = false;
    bool isTheSame = false;

    m_Timer.Update();

    if(msg.compare(m_LastLog.m_Log) == 0)
    {
        double tempElapsedTime = m_Timer.GetTimerParams().m_ElapsedTimePression;

        isTheSame = true;

        m_LastLog.m_ElapsedTimeSameLogPrint += tempElapsedTime;

        if(m_LastLog.m_ElapsedTimeSameLogPrint >= AE_LOG_DUPLICATE_TIMEOUT || tempElapsedTime >= AE_LOG_DUPLICATE_TIME)
        {
            printSameLog = true;
        }
        else
        {
            m_LastLog.m_NumSameLogs++;
        }
    }
    else if(m_LastLog.m_NumSameLogs >= 2)
    {
        printSameLog = true;
    }

    if(printSameLog)
    {
        std::string sameLogMsg = "";

        if (logSystem != LogSystem::Localization)
        {
            sameLogMsg = fmt::format(AELOCMAN.GetLiteral("LOGGER_SAME_LOG_MSG"), m_LastLog.m_NumSameLogs);
        }
        else
        {
            //This should never happen
            AEAssert(false);
            sameLogMsg = fmt::format("Localization Manager null or is calling Logger. Number of Messages repeated: {0}", m_LastLog.m_NumSameLogs);
        }

        InsertLogNoLock(m_LastLog.m_Level, sameLogMsg, false);

        //Set Last Log to empty again
        m_LastLog = AELastLog();
    }

    if(!isTheSame)
    {
        InsertLogNoLock(logLevel, msg, true);
    }
}

void Logger::InsertLogNoLock(LogLevel logLevel, const std::string& msg, bool countEqual)
{
    AELog newLog;

    AE_Base::GetDate(newLog.m_TimeStamp);

    newLog.m_Log        = msg;
    newLog.m_Level        = logLevel;

    if(logLevel == LogLevel::Warning)
    {
        m_Warnings = true;
    }

    if(logLevel == LogLevel::Error)
    {
        m_Errors = true;
    }

    newLog.m_LogLine = m_LinesCount;

    //Increase light count
    m_LinesCount++;

    if(m_Logs.size() == m_Capacity)
    {
        m_Logs.pop_front();
    }

    m_Logs.push_back(newLog);

    if(m_LogToFile)
    {
        std::string logStr = newLog.ToString();

        m_FileStream.write(logStr.c_str(), logStr.size());

        m_FileStream.flush();
    }

    if(countEqual)
    {
        m_LastLog.m_Level                       = logLevel;
        m_LastLog.m_Log                         = msg;
        m_LastLog.m_NumSameLogs                 = 1;
        m_LastLog.m_ElapsedTimeSameLogPrint     = 0.0;
    }
}

AEResult Logger::SaveLogsInFile()
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    if(m_LogToFile)
    {
        return AEResult::Log2FileActive;
    }

    AEAssert(!m_LogFilename.empty());
    if (m_LogFilename.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_FileStream = std::ofstream(m_LogFilename, std::ios::out | std::ios::ate);
    if(!m_FileStream.is_open())
    {
        return AEResult::OpenFileFail;
    }

    AETODO("Remove this from here and replace");
    const std::string beginFile = "----------------------------------------------------\n";
    m_FileStream.write(beginFile.c_str(), beginFile.size());

    auto it = m_Logs.cbegin();
    auto itEnd = m_Logs.cend();

    for(; it != itEnd; ++it)
    {
        std::string logStr = it->ToString();

        m_FileStream.write(logStr.c_str(), logStr.size());
    }

    m_FileStream.flush();
    m_FileStream.close();

    return AEResult::Ok;
}

CScriptArray* Logger::GetLogsScript()
{
    std::lock_guard<std::mutex> lock(m_LogMutex);

    AEAssert(m_ScriptAELogArrayType != nullptr);
    if (m_ScriptAELogArrayType == nullptr)
    {
        return nullptr;
    }

    CScriptArray* asAELogArr = CScriptArray::Create(m_ScriptAELogArrayType);

    asAELogArr->Resize((int32_t)m_Logs.size());

    auto it = m_Logs.cbegin();
    auto itEnd = m_Logs.cend();
    for (uint32_t i = 0; it != itEnd; ++it, ++i)
    {
        AELog* log = reinterpret_cast<AELog*>(asAELogArr->At(i));

        AEAssert(log != nullptr);
        *log = *it;
    }

    return asAELogArr;
}

void Logger::MonitorRepeatedLogTimeOut()
{
    while(m_IsRunning)
    {
        {
            std::unique_lock<std::mutex> cvLock(m_LogMutex);
            m_LogConditionalVar.wait_for(cvLock,
                                         std::chrono::seconds(AE_LOG_TIMEOUT_THREAD_SLEEP),
                                        [this](){return m_LastLog.m_NumSameLogs >= 2 || !m_IsRunning;});
        }

        std::lock_guard<std::mutex> lock(m_LogMutex);
        if (m_LastLog.m_NumSameLogs < 2)
        {
            continue;
        }

        m_Timer.Update();

        m_LastLog.m_ElapsedTimeSameLogPrint += m_Timer.GetTimerParams().m_ElapsedTimePression;

        if(m_LastLog.m_ElapsedTimeSameLogPrint >= AE_LOG_DUPLICATE_TIMEOUT)
        {
            std::string sameLogMsg = "";
            sameLogMsg = fmt::format(sameLogMsg, AELOCMAN.GetLiteral("LOGGER_SAME_LOG_MSG"), m_LastLog.m_NumSameLogs);
            InsertLogNoLock(m_LastLog.m_Level, sameLogMsg, false);

            //Set Last Log to empty again
            m_LastLog = AELastLog();
        }
    }
}
