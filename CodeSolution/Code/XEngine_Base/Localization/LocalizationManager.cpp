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
#include <assert.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Logger\Logger.h"
#include "XML\XEXMLParser.h"
#include "Logger\LoggerDefs.h"
#include "LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
LocalizationManager::LocalizationManager()
{
}

LocalizationManager::~LocalizationManager()
{
}

XEResult LocalizationManager::Initialize(const std::wstring& file, const std::wstring& projectDir)
{
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	XEAssert(!file.empty());
	if(file.empty())
	{
		return XEResult::EmptyFilename;
	}

	m_ProjectDirectory = projectDir;
	if (m_ProjectDirectory.back() != L'\\' && m_ProjectDirectory.back() != L'/')
	{
		m_ProjectDirectory.push_back(L'/');
	}

	m_Filename = file;
	
	return ReloadWithoutLock();
}

void LocalizationManager::ClearAllMaps()
{
	m_DefaultLanguage = L"";
	m_CurrentLiteralMap.clear();
	m_LanguagesMap.clear();
}

XEResult LocalizationManager::Reload()
{
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	return ReloadWithoutLock();
}

XEResult LocalizationManager::ReloadWithoutLock()
{
	if(m_Filename.empty())
	{
		return XEResult::EmptyFilename;
	}

	m_IsReady = false;

	XEXMLParser newFile;
	if (newFile.LoadFile(m_Filename) != XEResult::Ok)
	{
		/*Hard coded string Loc Manager not loaded*/
		std::wstring msg_error = L"";
		fastformat::fmt(msg_error, L"{0}: Could not read file: {1}", __FUNCTIONW__, m_Filename);

		m_CallByLocManager = true;
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		m_CallByLocManager = false;

		return XEResult::OpenFileFail;
	}

	bool ret = false;

	//Clear all maps for new load
	ClearAllMaps();

	XEXMLParser localizationXML = newFile[L"Localization"];
	if ( localizationXML.IsReady() )
	{
		uint16_t l_Count = localizationXML.GetNumChildren();

		for( uint16_t i = 0; i < l_Count; ++i )
		{
			XEXMLParser child = localizationXML(i);

			std::wstring l_Type = child.GetName();

			if( l_Type.compare(L"Default") == 0 )
			{
				m_DefaultLanguage = child.GetString(L"lang");
			}
			else if ( l_Type.compare(L"Language") == 0 )
			{
				std::wstring language = child.GetString(L"name");
				uint32_t locFind = child.GetUInt(L"loc", 0, false);

				std::wstring langFile = L"";

				if (locFind == 1)
				{
					langFile = XE_PREFIX_ENGINE_FILE_PATH + child.GetString(L"file");
				}
				else
				{
					langFile = m_ProjectDirectory + child.GetString(L"file");
				}

				XEResult loadLangRet = LoadLanguageLiterals(language, langFile);
				if(loadLangRet != XEResult::Ok)
				{
					/*Hard coded string Loc Manager not loaded*/
					std::wstring msg_error = L"";
					fastformat::fmt(msg_error, L"{0}: Failed to read Language Literals: {1}, from file: {2}", __FUNCTIONW__, language, langFile);

					m_CallByLocManager = true;
					XELOGGER->AddNewLog(LogLevel::Error, msg_error);
					m_CallByLocManager = false;

					ret |= false;
				}
				else
				{
					//If at least 1 was loaded correctly than we can return true that language literals where loaded
					ret |= true;
				}
			}
		}
	}

	//Both ret and Set Default Language has to be true in order to return that loc manager was loaded
	XEResult sdl = SetDefaultLanguageWithoutLock(m_DefaultLanguage);

	if(!ret || sdl != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult LocalizationManager::LoadLanguageLiterals(const std::wstring& name, const std::wstring& file)
{
	if(file.empty() || name.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLParser newFile;
	if (newFile.LoadFile(file) != XEResult::Ok)
	{
		/*Hard coded string Loc Manager not loaded*/
		std::wstring msg_error = L"";
		fastformat::fmt(msg_error, L"{0}: Could not read file: {1}", __FUNCTIONW__, file);

		m_CallByLocManager = true;
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		m_CallByLocManager = false;

		return XEResult::OpenFileFail;
	}

	XEXMLParser languageXML = newFile[L"Language"];

	if ( languageXML.IsReady() )
	{
		//Create Literal Map and add it to Map
		m_LanguagesMap[name] = LiteralMap();

		//Get Reference to add Literals to the Map
		LiteralMap& litMap = m_LanguagesMap[name];

		uint16_t l_Count = languageXML.GetNumChildren();

		for( uint16_t i = 0; i < l_Count; ++i )
		{
			XEXMLParser child = languageXML(i);

			std::wstring l_Type = child.GetName();

			if ( l_Type.compare(L"Literal") == 0 )
			{
				std::wstring literalName = child.GetString(L"name");
				std::wstring literalMsg = child.GetString(L"msg");

				if(!name.empty())
				{
					if(litMap.find(literalName) == litMap.end()) //If Literal Name is not present
					{						
						litMap[literalName] = LocalizationLiteral(literalName, literalMsg);
					}
					else //If Literal Name is present
					{
						/*Hard coded string Loc Manager not loaded*/
						std::wstring msg_error = L"";
						fastformat::fmt(msg_error, L"{0}: Literal: {1}, already exists in the language collection", __FUNCTIONW__, literalName);

						m_CallByLocManager = true;
						XELOGGER->AddNewLog(LogLevel::Warning, msg_error);
						m_CallByLocManager = false;
					}
				}
				else
				{
					/*Hard coded string Loc Manager not loaded*/
					std::wstring msg_error = L"";
					fastformat::fmt(msg_error, L"{0}: Msg: {1} does not have a name", __FUNCTIONW__, literalMsg);

					m_CallByLocManager = true;
					XELOGGER->AddNewLog(LogLevel::Warning, msg_error);
					m_CallByLocManager = false;
				}
			}
		}

		return XEResult::Ok;
	}

	return XEResult::Fail;
}

const std::wstring& LocalizationManager::GetLiteral(const std::wstring& literalName)
{
	if(!m_IsReady)
	{
		return XE_LOC_LANG_DEFAULT_LITERAL;
	}

	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	auto it = m_CurrentLiteralMap.find(literalName);

	if(it != m_CurrentLiteralMap.end())
	{
		return it->second.m_Msg;
	}

	return XE_LOC_LANG_DEFAULT_LITERAL;
}

const std::wstring& LocalizationManager::GetDefaultLanguage() const
{
	if(!m_IsReady)
	{
		return XE_LOC_LANG_NO_LANGUAGE;
	}

	if(m_DefaultLanguage.empty())
	{
		return XE_LOC_LANG_NO_LANGUAGE;
	}

	return m_DefaultLanguage;
}

XEResult LocalizationManager::SetDefaultLanguage(const std::wstring& languageName)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	return SetDefaultLanguageWithoutLock(languageName);
}

XEResult LocalizationManager::SetDefaultLanguageWithoutLock(const std::wstring& languageName)
{
	auto it = m_LanguagesMap.find(languageName);

	if(it != m_LanguagesMap.end())
	{
		m_DefaultLanguage = languageName;
		m_CurrentLiteralMap = it->second;
		return XEResult::Ok;
	}

	return XEResult::NotFound;
}