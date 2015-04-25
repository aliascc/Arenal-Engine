/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"

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
*   XELastLog   *
*****************/
XELastLog::XELastLog()
{
}

/************
*   XELog   *
*************/
XELog::XELog()
{
}

std::wstring XELog::ToString() const
{
	std::wstring fileLine = L"";

	fastformat::fmt(fileLine, L"{0}-({1}): {2}- \'{3}\'\n", m_LogLine, m_TimeStamp.ToString(), XELogHelpers::LogLevelStr(m_Level), m_Log);

	return fileLine;
}

XELog& XELog::operator=(const XELog &other)
{
	m_Log		= other.m_Log;
	m_Level		= other.m_Level;
	m_LogLine	= other.m_LogLine;
	m_TimeStamp	= other.m_TimeStamp;

	return *this;
}

/*******************
*   XELogHelpers   *
********************/
namespace XELogHelpers
{

	std::wstring XELogHelpers::LogLevelStr(LogLevel lvl)
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

		XEAssert(false);

		return L"";
	}

	LogLevel XELogHelpers::Str2LogLevel(std::wstring lvl)
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

		XEAssert(false);

		return LogLevel::None;
	}

}