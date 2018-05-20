/** \file 
 * Contains definitions and methods for basic use of the AEngine 
 */

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

#ifndef _BASE_FUNCTIONS_H
#define _BASE_FUNCTIONS_H

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

/**************
*   Defines   *
***************/

/// <summary>
/// Displays in the output window an human readable error for HRESULT 
/// <summary>
#if defined(AE_HRESULT_EXTRA_INFO)

//FORMAT_MESSAGE_FROM_SYSTEM = use system message tables to retrieve error text
//FORMAT_MESSAGE_ALLOCATE_BUFFER = allocate buffer on local heap for error text
//FORMAT_MESSAGE_IGNORE_INSERTS = Important! will fail otherwise, since we're not (and CANNOT) pass insertion parameters

/// <summary>
/// Displays in the output window an human readable error for HRESULT 
/// <summary>
#define DisplayError(hr){                                                                                                           \
                            LPSTR errorText = nullptr;                                                                              \
                            std::string errorStr = "";                                                                              \
                            std::string file(__FILE__);                                                                             \
                            std::string line(AESTR(__LINE__));                                                                      \
                            FormatMessage(                                                                                          \
                                FORMAT_MESSAGE_FROM_SYSTEM     | FORMAT_MESSAGE_ALLOCATE_BUFFER  | FORMAT_MESSAGE_IGNORE_INSERTS,   \
                                nullptr,                                                                                            \
                                hr,                                                                                                 \
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                                                          \
                                (LPSTR)&errorText,                                                                                  \
                                0,                                                                                                  \
                                nullptr);                                                                                           \
                            errorStr = file + "(" + line + "): " + std::string(errorText);                                          \
                            LocalFree(errorText);                                                                                   \
                            errorText = nullptr;                                                                                    \
                            OutputDebugString(errorStr.c_str());                                                                    \
                        }
#else
#define DisplayError(x)
#endif

#if defined(AE_HRESULT_EXTRA_INFO)
/// <summary>
/// Checks an HRESULT and if it is an error displays in the output window an human readable error for HRESULT 
/// <summary>
#define CheckError(hr)  if(FAILED(hr))          \
                        {                       \
                            DisplayError(hr);   \
                        }
#else
#define CheckError(x)
#endif


#if defined(AE_ENABLE_ASSERT)
#ifdef AE_ASSERT_DBG_BRK
#define AEAssert(cond)  if(!(cond))         \
                        {                   \
                            DebugBreak();   \
                        }
#else
#define AEAssert(cond)    assert(cond)
#endif
#else
#define AEAssert(x)
#endif

/****************
*   Constants   *
*****************/
namespace AE_Base 
{
    /// <summary>
    /// Defines the mayor Current Version of AEngine
    /// </summary>    
    extern const uint32_t AE_VERSION_MAYOR;

    /// <summary>
    /// Defines the minor Current Version of AEngine
    /// </summary>    
    extern const uint32_t AE_VERSION_MINOR;

    /// <summary>
    /// Defines the Current Version of AEngine
    /// </summary>    
    extern const uint32_t AE_VERSION_REVISION;
    
    /// <summary>
    /// String containing Current Version, Mayor and Minor of AEngine
    /// </summary>    
    extern const std::string AE_VERSION;

    /// <summary>
    /// Codename of the Current Version of AEngine
    /// </summary>    
    extern const std::string AE_CODENAME;

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

        timeStamp.m_Day     = st.wDay;
        timeStamp.m_Month   = st.wMonth;
        timeStamp.m_Year    = st.wYear;
        timeStamp.m_Hour    = st.wHour;
        timeStamp.m_Minute  = st.wMinute;
        timeStamp.m_Second  = st.wSecond;
    }

    /// <summary>
    /// Gets the extension from a File Name
    /// </summary>
    /// <remarks>
    /// The extension of the file, is after the last dot, i.e. "test.txt", extension is "txt"
    /// </remarks>
    /// <param name="fullPath">File name from where to get the extension</param>
    /// <returns>Extension of the file name</returns>
    inline std::string GetFilenameExtension(const std::string& fullPath)
    {
        if(fullPath.empty())
        {
            return "";
        }

        boost::filesystem::path boostPath(fullPath);

        if(!boostPath.has_extension())
        {
            return "";
        }

        std::string ext = boostPath.extension().string();
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
    inline std::string GetFilename(const std::string& fullPath)
    {
        if(fullPath.empty())
        {
            return "";
        }

        boost::filesystem::path boostPath(fullPath);

        std::string fileName = boostPath.filename().string();
        
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
    inline std::string GetFilenameOnly(const std::string& fullPath)
    {
        if(fullPath.empty())
        {
            return "";
        }

        boost::filesystem::path boostPath(fullPath);

        std::string fileName = boostPath.filename().replace_extension("").string();

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
    std::string GetRelativePath(const std::string& filepath);

    /// <summary>
    /// Splits a String into substrings by delimiters set
    /// </summary>
    /// <param name="str">String to Split</param>
    /// <param name="tokens">Vector of Strings where to insert sub strings</param>
    /// <param name="regex">Regular Expression to separate the string</param>
    /// <param name="trimEmpty">If true, it will not insert empty substrings to the vector</param>
    void SplitString(const std::string& str, std::vector<std::string>& tokens, const std::string& regex, bool trimEmpty = false);

    /// <summary>
    /// Splits a String into substrings by delimiters set
    /// </summary>
    /// <param name="str">String to Split</param>
    /// <param name="tokens">Vector of Strings where to insert sub strings</param>
    /// <param name="regex">Regular Expression to separate the string</param>
    /// <param name="trimEmpty">If true, it will not insert empty substrings to the vector</param>
    void SplitWString(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& regex, bool trimEmpty = false);

    /// <summary>
    /// Gets the Last Time Stamp when the File was modified
    /// </summary>
    /// <param name="fileName">File name with path</param>
    /// <param name="timeStamp">TimeStamp object to get store the information</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult GetFileModifiedTime(const std::string& fileName, TimeStamp& timeStamp);

    /// <summary>
    /// Print a message to the console window
    /// </summary>
    /// <param name="msg"Message to print</param>
    void ConsolePrint(const std::string& msg);

    /// <summary>
    /// Print a message to the console window with a break line at the end
    /// </summary>
    /// <param name="msg"Message to print</param>
    void ConsolePrintLine(const std::string& msg);
    
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
    /// Gets AEResult Code in a Readable String
    /// </summary>
    /// <param name="fileName">AEResult to Translate</param>
    /// <returns>AEResult in String Format</returns>
    std::string GetAEResultString(AEResult result);
}

#endif
