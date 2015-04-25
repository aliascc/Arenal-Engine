/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LOGGER_H
#define _LOGGER_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <fstream>
#include <stdint.h>
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/
#include "AngelScript-Add-Ons\scriptarray\scriptarray.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "LoggerDefs.h"
#include "Time\XETimer.h"
#include "Utils\Singleton.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Logger class that controls all the log messages of the Game
/// </summary>
class Logger : public Singleton<Logger>
{
	friend class Singleton<Logger>;

	private:

		/*************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

		/// <summary>
		/// This thread is in charge of monitoring if a message has been repeated and 
		/// no different message has been logged after the time out for repeated messages
		/// </summary>
		std::thread m_RepeatLogLvlThread;

		/// <summary>
		/// True will Logger is running and has not been deleted
		/// </summary>
		bool m_IsRunning = true;

		/// <summary>
		/// List of Logs that have been added
		/// </summary>
		LogList m_Logs;

		/// <summary>
		/// Line count of Logs
		/// </summary>
		uint32_t m_LinesCount = 1;

		/// <summary>
		/// Max Capacity for Logs in the list
		/// </summary>
		uint32_t m_Capacity = XE_LOG_CAPACITY;

		/// <summary>
		/// Max Level of Log to be logged
		/// </summary>
		LogLevel m_LogLevel = LogLevel::Info;

		/// <summary>
		/// Variable to time logs, so if same message appears continuously
		/// in a certain time frame it will only to print how many logs of the 
		/// same message has been repeated.
		/// </summary>
		XETimer m_Timer;

		/// <summary>
		/// Last Message saved in log
		/// </summary>
		XELastLog m_LastLog;

		/// <summary>
		/// Variable to tell if errors have been generated.
		/// </summary>
		bool m_Errors = false;

		/// <summary>
		/// Variable to tell if warnings have been generated.
		/// </summary>
		bool m_Warnings = false;

		/// <summary>
		/// Filename of the log file.
		/// </summary>
		std::wstring m_LogFilename = XE_LOG_DEFAULT_PATH;

		/// <summary>
		/// Determines if the Logs need to be printed also in a file
		/// </summary>
		bool m_LogToFile = false;

		/// <summary>
		/// File Stream of Log File when it is open
		/// </summary>
		std::wofstream m_FileStream;

		/// <summary>
		/// Mutex for Logger so it will be thread safe
		/// </summary>
		std::mutex m_LogMutex;

		/// <summary>
		/// Script "XELog Array" Object Type
		/// </summary>
		asIObjectType* m_ScriptXELogArrayType = nullptr;

#pragma endregion

		/***********************
		 *   Private Methods   *
		 ***********************/
#pragma region Private Methods

		/// <summary>
		/// Inserts of Log into the List. This should be called only from thread safe methods as this one
		/// is not.
		/// </summary>
		/// <param name="logLevel">Log Level of log</param>
		/// <param name="msg">Log message</param>
		/// <param name="countEqual">If this log is to be monitored for repeated ones</param>
		void InsertLogNoLock(LogLevel logLevel, const std::wstring& msg, bool countEqual);

		/// <summary>
		/// Function to monitor if any duplicate logs have to be flush to log after a timeout
		/// </summary>
		void MonitorRepeatedLogTimeOut();

#pragma endregion

		/***********************************************
		*   Private Constructor & Destructor Methods   *
		************************************************/
#pragma region Private Constructor & Destructor Methods

		/// <summary>
		/// Default Logger Constructor
		/// </summary>
		Logger();

		/// <summary>
		/// Default Logger Destructor
		/// </summary>
		virtual ~Logger();

#pragma endregion

	public:

		/*******************
		 *   Set Methods   *
		 *******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the Max Capacity for the Logs
		/// </summary>
		/// <param name="capacity">Max Capacity for the Logs</param>
		void SetCapacity(uint32_t capacity);

		/// <summary>
		/// Sets the Max Log Level to be logged
		/// </summary>
		/// <param name="logLevel">Max Log Level to log</param>
		void SetLogLevel(LogLevel logLevel);

		/// <summary>
		/// Sets the File Path where Logs will be flushed if activated
		/// </summary>
		/// <param name="path">File path</param>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SetLogFilename(const std::wstring& path);

		/// <summary>
		/// Sets the Script "XELog Array" type
		/// </summary>
		/// <param name="scriptXELogArrayType">Script "XELog Array" Type</param>
		void SetScriptXELogArrayType(asIObjectType* scriptXELogArrayType);

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

		/// <summary>
		/// Gets Capacity for Logs
		/// </summary>
		/// <returns>Max Capacity for the Logs</returns>
		inline uint32_t GetCapacity () const 
		{
			return m_Capacity; 
		}

		/// <summary>
		/// Gets Max Log Level to be logged
		/// </summary>
		/// <returns>Max Log Level</returns>
		inline LogLevel GetLogLevel() const
		{
			return m_LogLevel; 
		}

		/// <summary>
		/// Log File where Logs will be flush also if activated
		/// </summary>
		/// <returns>Name of Log File</returns>
		inline const std::wstring& GetLogFilename() const
		{
			return m_LogFilename;
		}

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Activates Log to File function
		/// </summary>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult ActivateLogToFile();

		/// <summary>
		/// Deactivates Log to File function
		/// </summary>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult DeactivateLogToFile();

		/// <summary>
		/// Returns if any errors have been logged
		/// </summary>
		/// <returns>True if errors have been logged</returns>
		bool ErrorsExist() const;

		/// <summary>
		/// Returns if any warnings have been logged
		/// </summary>
		/// <returns>True if warnings have been logged</returns>
		bool WarningsExist() const;

		/// <summary>
		/// Reset Error Found Flag
		/// </summary>
		void ResetErrors();

		/// <summary>
		/// Reset Warning Found Flag
		/// </summary>
		void ResetWarning();

		/// <summary>
		/// If Log to File function is not active, it will dump the current list of logs to a file
		/// </summary>
		/// <returns>XEResult::Ok if successful</returns>
		XEResult SaveLogsInFile();

		/// <summary>
		/// Add new Log to file
		/// </summary>
		/// <param name="logLevel">Log Level of the Log</param>
		/// <param name="msg">Log message</param>
		void AddNewLog(LogLevel logLevel, const std::wstring& msg);

		/// <summary>
		/// Returns a Script Array of the current Logs
		/// </summary>
		/// <returns>Script Array of the Logs</returns>
		CScriptArray* GetLogsScript();

#pragma endregion
};

#endif

