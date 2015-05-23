/** \file 
 * Contains definitions and methods for basic use of the XEngine 
 */

/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _BASE_FUNCTIONS_H
#define _BASE_FUNCTIONS_H

/**********************
*   System Includes   *
***********************/
#include <time.h>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <Windows.h>
#include <sys/stat.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "boost\filesystem.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\XEObject.h"
#include "Time\XETimeDefs.h"

/**************
*   Defines   *
***************/

//Uncomment to make XE_ASSERT assert and not debug
//#define XE_ASSERT_NO_DEBUG

//FORMAT_MESSAGE_FROM_SYSTEM = use system message tables to retrieve error text
//FORMAT_MESSAGE_ALLOCATE_BUFFER = allocate buffer on local heap for error text
//FORMAT_MESSAGE_IGNORE_INSERTS = Important! will fail otherwise, since we're not (and CANNOT) pass insertion parameters

/// <summary>
/// Displays in the output window an human readable error for HRESULT 
/// <summary>
#if defined(_DEBUG)
#define DisplayError(hr){																										\
							LPWSTR errorText = nullptr;																			\
							std::wstring errorStr = L"";																		\
							std::wstring file(__FILEW__);																		\
							std::wstring line(XEWSTR(__LINE__));																\
							FormatMessage(																						\
								FORMAT_MESSAGE_FROM_SYSTEM 	| FORMAT_MESSAGE_ALLOCATE_BUFFER  | FORMAT_MESSAGE_IGNORE_INSERTS,	\
								nullptr,																						\
								hr,																								\
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),														\
								(LPWSTR)&errorText,																				\
								0,																								\
								nullptr);																						\
							errorStr = file + L"(" + line + L"): " + std::wstring(errorText);									\
							LocalFree(errorText);																				\
							errorText = nullptr;																				\
							OutputDebugString(errorStr.c_str());																\
						}
#else
#define DisplayError(x)
#endif


/// <summary>
/// Checks an HRESULT and if it is an error displays in the output window an human readable error for HRESULT 
/// <summary>
#if defined(_DEBUG)
#define CheckError(hr)	if(FAILED(hr))			\
						{						\
							DisplayError(hr)	\
						}
#else
#define CheckError(x)
#endif


/// <summary>
/// XEmgine Modified Assert.
/// <list type="bullet">
/// <item>If XE_ASSERT_NO_DEBUG is define, XEAssert will work like a normal assert.</item>
/// <item>If XE_ASSERT_NO_DEBUG is not defined instead of an assert it will create a debug breakpoint.</item>
/// </list>
/// </summary>	
#if defined(_DEBUG)
#ifdef XE_ASSERT_NO_DEBUG
#define XEAssert(cond)	assert(cond)
#else
#define XEAssert(cond)	if(!(cond))			\
						{					\
							DebugBreak();	\
						}
#endif
#else
#define XEAssert(x)
#endif

/****************
*   Constants   *
*****************/
namespace XE_Base 
{
	/// <summary>
	/// Defines the mayor Current Version of XEngine
	/// </summary>	
	extern const uint32_t XE_VERSION_MAYOR;

	/// <summary>
	/// Defines the minor Current Version of XEngine
	/// </summary>	
	extern const uint32_t XE_VERSION_MINOR;

	/// <summary>
	/// Defines the Current Version of XEngine
	/// </summary>	
	extern const uint32_t XE_VERSION_REVISION;
	
	/// <summary>
	/// String containing Current Version, Mayor and Minor of XEngine
	/// </summary>	
	extern const std::wstring XE_VERSION;

	/// <summary>
	/// Codename of the Current Version of XEngine
	/// </summary>	
	extern const std::wstring XE_CODENAME;

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/

/***********************
*   Global Functions   *
************************/

	/// <summary>
	/// Gets the Current Time Stamp
	/// </summary>
	/// <param name="timeStamp">TimeStamp object to get store the information</param>
	inline void GetDate(TimeStamp& timeStamp)
	{
		SYSTEMTIME st;

		memset(&timeStamp, 0, sizeof(TimeStamp));

		GetLocalTime(&st);

		timeStamp.m_Day		= st.wDay;
		timeStamp.m_Month	= st.wMonth;
		timeStamp.m_Year	= st.wYear;
		timeStamp.m_Hour	= st.wHour;
		timeStamp.m_Minute	= st.wMinute;
		timeStamp.m_Second	= st.wSecond;
	}

	/// <summary>
	/// Gets the extension from a File Name
	/// </summary>
	/// <remarks>
	/// The extension of the file, is after the last dot, i.e. "test.txt", extension is "txt"
	/// </remarks>
	/// <param name="fullPath">File name from where to get the extension</param>
	/// <returns>Extension of the file name</returns>
	inline std::wstring GetFilenameExtension(const std::wstring& fullPath)
	{
		if(fullPath.empty())
		{
			return L"";
		}

		boost::filesystem::path boostPath(fullPath);

		if(!boostPath.has_extension())
		{
			return L"";
		}

		std::wstring ext = boostPath.extension().c_str();
		ext.erase(ext.begin());

		return ext;
	}

	/// <summary>
	/// Gets the File Name from a full path
	/// </summary>
	/// <remarks>
	/// The file name of the full path, is after the last slash, i.e. "C:\my test\test.txt", file name is "test.txt"
	/// </remarks>
	/// <param name="fullPath">Full path from where to get the file name</param>
	/// <returns>File name</returns>
	inline std::wstring GetFilename(const std::wstring& fullPath)
	{
		if(fullPath.empty())
		{
			return L"";
		}

		boost::filesystem::path boostPath(fullPath);

		std::wstring fileName = boostPath.filename().c_str();
		
		return fileName;
	}

	/// <summary>
	/// Gets the File Name from a full path
	/// </summary>
	/// <remarks>
	/// The file name of the full path, is after the last slash, i.e. "C:\my test\test.txt", file name is "test.txt"
	/// </remarks>
	/// <param name="fullPath">Full path from where to get the file name</param>
	/// <returns>File name</returns>
	inline std::wstring GetFilenameOnly(const std::wstring& fullPath)
	{
		if(fullPath.empty())
		{
			return L"";
		}

		boost::filesystem::path boostPath(fullPath);

		std::wstring fileName = boostPath.filename().replace_extension(L"").c_str();

		return fileName;
	}

	/// <summary>
	/// Converts a normal string (or char*) to a wide string (Unicode)
	/// </summary>
	/// <param name="s">String to convert to wide string (Unicode)</param>
	/// <returns>Wide string</returns>
	inline std::wstring String2WideStr(const std::string& s)
	{
		if(s.empty())
		{
			return L"";
		}

		int32_t len = 0;
		int32_t slength = (int32_t)s.length() + 1;
		
		len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, 0, 0); 

		wchar_t* buf = new wchar_t[len];
		ZeroMemory(buf, sizeof(wchar_t) * len);

		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, buf, len);
		
		std::wstring ws(buf);

		DeleteMemArr(buf);

		return ws;
	}

	/// <summary>
	/// Converts a wide string (Unicode) to a normal string (char*)
	/// </summary>
	/// <param name="ws">Wide String to convert to string (char*)</param>
	/// <returns>Normal string</returns>
	inline std::string WideStr2String(const std::wstring& ws)
	{
		if (ws.empty())
		{
			return "";
		}

		int32_t len = 0;
		int32_t slength = (int32_t)ws.length() + 1;

		len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), slength, nullptr, 0, nullptr, nullptr);

		char* buf = new char[len];
		ZeroMemory(buf, sizeof(char)* len);

		WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), slength, buf, len, nullptr, nullptr);

		std::string s(buf);

		DeleteMemArr(buf);

		return s;
	}

	/// <summary>
	/// Creates a Relative path to the current directory
	/// </summary>
	/// <param name="filepath">File path to create relative path</param>
	/// <returns>File path Relative to current working directory</returns>
	std::wstring GetRelativePath(const std::wstring& filepath);

	/// <summary>
	/// Splits the Strings with a delimiter
	/// </summary>
	/// <param name="s">String to split</param>
	/// <param name="delim">Delimiter character</param>
	/// <returns>Vector of strings that are split</returns>
	std::vector<std::string> SplitString(const std::string& s, char delim);

	/// <summary>
	/// Splits the Strings with a delimiter
	/// </summary>
	/// <param name="s">String to split</param>
	/// <param name="delim">Delimiter character</param>
	/// <returns>Vector of strings that are split</returns>
	std::vector<std::wstring> SplitString(const std::wstring& s, wchar_t delim);

	/// <summary>
	/// Splits a String into substrings by delimiters set
	/// </summary>
	/// <param name="str">String to Split</param>
	/// <param name="tokens">Vector of Strings where to insert sub strings</param>
	/// <param name="delimiters">Delimiters to use to split the string</param>
	/// <param name="trimEmpty">If true, it will not insert empty substrings to the vector</param>
	void SplitString(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& delimiters = L" ", bool trimEmpty = false);

	/// <summary>
	/// Gets the Last Time Stamp when the File was modified
	/// </summary>
	/// <param name="fileName">File name with path</param>
	/// <param name="timeStamp">TimeStamp object to get store the information</param>
	/// <returns>XEResult::OK if successful</returns>
	XEResult GetFileModifiedTime(const std::wstring& fileName, TimeStamp& timeStamp);

	/// <summary>
	/// Print a message to the console window
	/// </summary>
	/// <param name="msg"Message to print</param>
	void ConsolePrint(const std::wstring& msg);

	/// <summary>
	/// Print a message to the console window with a break line at the end
	/// </summary>
	/// <param name="msg"Message to print</param>
	void ConsolePrintLine(const std::wstring& msg);
	
	/// <summary>
	/// Gets a unique ID
	/// </summary>
	/// <remarks>
	/// This ID is unique and will not repeat throughout the life time of the Game Application
	/// it is non-persistent, so when the Game restarts it will start back from 1
	/// </remarks>
	/// <returns>Unique ID</returns>
	uint64_t GetNextUniqueID();

	/// <summary>
	/// Gets XEResult Code in a Readable String
	/// </summary>
	/// <param name="fileName">XEResult to Translate</param>
	/// <returns>XEResult in String Format</returns>
	std::wstring GetXEResultString(XEResult result);
}

#endif