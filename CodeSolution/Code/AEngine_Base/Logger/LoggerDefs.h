/** \file 
 * Contains definitions for use of the Log System
 */

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

#ifndef _LOGGER_DEFS_H
#define _LOGGER_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Time\AETimeDefs.h"
#include "Localization/LocalizationManagerDefs.h"

/********************
*   Forward Decls   *
*********************/
class Logger;

/**************
*   Defines   *
***************/

/// <summary>
/// Define for Logger Singleton Instance Get Call 
/// </summary>
#define AELOGGER                    Logger::GetInstance()

/// <summary>
/// Default Maximum Log Capacity
/// </summary>
#define AE_LOG_CAPACITY             500

/// <summary>
/// Define how many seconds a log with the same
/// message will be counted as a duplicate
/// </summary>
#define AE_LOG_DUPLICATE_TIME       1.0

/// <summary>
/// Define how many seconds the timeout to count
/// logs as duplicates
/// </summary>
#define AE_LOG_DUPLICATE_TIMEOUT    5.0

/// <summary>
/// Sleep Time for Timeout monitor thread in seconds
/// </summary>
#define AE_LOG_TIMEOUT_THREAD_SLEEP 5

/************
*   Enums   *
*************/

/// <summary>
/// Enum for Log Levels to be able to identify the severity of a log or 
/// for the Log System to know which level of logs to accept.
/// </summary>
enum class LogLevel : uint32_t
{
    /// <summary>
    /// No Log is to be accepted.
    /// </summary>
    /// <remarks>
    /// This is only for the Log System to use, not for an specific log.
    /// </remarks>
    None = 0,

    /// <summary>
    /// Informational log only. Will only print logs that inform of certain unimportant events.
    /// </summary>
    Info,

    /// <summary>
    /// Warning log, if something happens but the Game can continue working in a reliable way, this log is use
    /// </summary>
    Warning,
    
    /// <summary>
    /// Error log, if an unrecoverable error happens or if the path taken is not expected , this log is use. Can cause the Game App to crash or behave in an unexpected way.
    /// </summary>
    Error,
    
    /// <summary>
    /// Debug log, use to show more detailed information of what the Game App is doing.
    /// </summary>
    Debug

    /******************************
    * When Adding new Log
    * do not forget to add it
    * to Script Enum Init
    ******************************/
};

enum class LogSystem : uint32_t
{
    None            = 0x00000000,
    Localization    = 0x00000001,
    Graphics        = 0x00000002,
    Core            = 0x00000004,
    GameResources   = 0x00000008,
    XML             = 0x00000010,
    GameComponents  = 0x00000020,
    GraphicsExt     = 0x00000040,
    GameAssets      = 0x00000080,
    GameObjects     = 0x00000100,
    GameImporters   = 0x00000200,
    Physics         = 0x00000400,
    Scripting       = 0x00000800,

    ////////////
    All             = 0xffffffff
};

/*************
*   Struct   *
**************/

/// <summary>
/// Struct to hold the information of the Log 
/// </summary>
struct AELastLog sealed : public AEObject
{
    /************************
     *   Public Variables   *
     ************************/
#pragma region Public Variables

    /// <summary>
    /// String that contains the Log
    /// </summary>
    std::string m_Log = "";
    
    /// <summary>
    /// Log Level for this Log
    /// </summary>
    LogLevel m_Level = LogLevel::None;

    /// <summary>
    /// Counter for number of log has repeated
    /// </summary>
    uint32_t m_NumSameLogs = 0;

    /// <summary>
    /// Time since the same log message appeared
    /// </summary>
    double m_ElapsedTimeSameLogPrint = 0.0;

#pragma endregion

    /***************************
     *   Constructor Methods   *
     ***************************/
#pragma region Constructor & Destructor Methods

    /// <summary>
    /// Default Constructor of AELog
    /// </summary>
    AELastLog();

#pragma endregion
};

/// <summary>
/// Struct to hold the Log information
/// </summary>
struct AELog sealed : public AEObject
{

    /***********************
     *   Public Variables   *
     ************************/
#pragma region Public Variables

    /// <summary>
    /// String that contains the Log
    /// </summary>
    std::string m_Log = "";

    /// <summary>
    /// Log Level for this Log
    /// </summary>
    LogLevel m_Level = LogLevel::None;

    /// <summary>
    /// Log position in order that it came in.
    /// </summary>
    uint32_t m_LogLine = 0;

    /// <summary>
    /// Time when this log happened.
    /// </summary>
    TimeStamp m_TimeStamp;

#pragma endregion

    /****************************************
     *   Constructor & Destructor Methods   *
     ****************************************/
#pragma region Constructor & Destructor Methods

    /// <summary>
    /// Default Constructor of AELog
    /// </summary>
    AELog();

#pragma endregion

    /***********************
     *   Public Methods   *
     ************************/
#pragma region Public Methods

    /// <summary>
    /// Override method ToString, use to convert a AELog to a string
    /// </summary>
    std::string ToString() const override;

    /// <summary>
    /// Override equal operator for AELog
    /// </summary>
    AELog& operator=(const AELog &other);

#pragma endregion
};

/***********************
*   Global Functions   *
************************/

inline LogSystem operator&(LogSystem left, LogSystem right)
{
    LogSystem res = (LogSystem)( (uint32_t)left & (uint32_t)right );
    return res;
}

inline LogSystem& operator&=(LogSystem& in, LogSystem right)
{
    in = (LogSystem)((uint32_t)in & (uint32_t)right);
    return in;
}

inline LogSystem operator|(LogSystem left, LogSystem right)
{
    LogSystem res = (LogSystem)((uint32_t)left | (uint32_t)right);
    return res;
}

inline LogSystem& operator|=(LogSystem& in, LogSystem right)
{
    in = (LogSystem)((uint32_t)in | (uint32_t)right);
    return in;
}

inline LogSystem operator^(LogSystem left, LogSystem right)
{
    LogSystem res = (LogSystem)((uint32_t)left ^ (uint32_t)right);
    return res;
}

inline LogSystem& operator^=(LogSystem& in, LogSystem right)
{
    in = (LogSystem)((uint32_t)in ^ (uint32_t)right);
    return in;
}

namespace AELogHelpers
{
    /// <summary>
    /// Converts an enum LogLevel to a string, using the same name as its variable
    /// </summary>
    /// <param name="lvl">LogLevel to convert to string</param>
    /// <returns>String name of the enum</returns>
    extern std::string LogLevelStr(LogLevel lvl);

    /// <summary>
    /// Converts a string to a enum LogLevel
    /// </summary>
    /// <param name="lvl">LogLevel to convert to string, string has to be the same name as the variable to convert to, if not it return LogLevel::None</param>
    /// <returns>enum LogLevel</returns>
    extern LogLevel Str2LogLevel(std::string lvl);

#if !defined(AE_FINAL)
    template<class...LogArgs>
    inline void LogDebug(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
        Log(LogLevel::Debug, logSystem, literalID, logArgs...);
    }

    template<class...LogArgs>
    inline void LogError(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
        Log(LogLevel::Error, logSystem, literalID, logArgs...);
    }

    template<class...LogArgs>
    inline void LogWarning(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
        Log(LogLevel::Warning, logSystem, literalID, logArgs...);
    }

    template<class...LogArgs>
    inline void LogInfo(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
        Log(LogLevel::Info, logSystem, literalID, logArgs...);
    }
#else
    template<class...LogArgs>
    inline void LogDebug(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
    }

    template<class...LogArgs>
    inline void LogError(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
    }

    template<class...LogArgs>
    inline void LogWarning(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
    }

    template<class...LogArgs>
    inline void LogInfo(LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
    }
#endif

    template<class...LogArgs>
    inline void Log(LogLevel logLevel, LogSystem logSystem, const std::string& literalID, LogArgs... logArgs)
    {
        if ( (AELOGGER.GetLogSystem() & logSystem) == LogSystem::None || AELOGGER.GetLogLevel() < logLevel)
        {
            return;
        }

        std::string msg = fmt::format(AELOCMAN.GetLiteral(literalID), logArgs...);

        AELOGGER.AddNewLog(logLevel, logSystem, msg);
    }

    template<class...LogArgs>
    inline void LogExplicit(LogLevel logLevel, LogSystem logSystem, const std::string& logMsg, LogArgs... logArgs)
    {
        if ((AELOGGER.GetLogSystem() & logSystem) == LogSystem::None || AELOGGER.GetLogLevel() < logLevel)
        {
            return;
        }

        std::string msg = fmt::format(logMsg, logArgs...);

        AELOGGER.AddNewLog(logLevel, logSystem, msg);
    }
};

/***************
*   Typedefs   *
****************/

/// <summary>
/// Typedef to Simplify the variable type of LogList
/// </summary>
typedef std::list<AELog> LogList;

#endif
