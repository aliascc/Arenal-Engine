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
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Logger\Logger.h"
#include "XML\XEXMLWriter.h"
#include "XML\XEXMLParser.h"
#include "Logger\LoggerDefs.h"
#include "Base\BaseLocations.h"
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

XEResult LocalizationManager::Initialize(const std::wstring& file, const std::wstring& enginePath)
{
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	XEAssert(!file.empty());
	if(file.empty())
	{
		return XEResult::EmptyFilename;
	}

	m_FilenameEngine = file;

	return ReloadEngineWithoutLock(enginePath);
}

void LocalizationManager::ClearAllMaps()
{
	m_DefaultLanguage = L"";
	m_LanguagesMap.clear();
	m_CurrentLiteralMap = nullptr;

	CleanProjectInfo();
}

void LocalizationManager::CleanProjectInfo()
{
	for (auto literalSetIt : m_ExtendedLiteralsMap)
	{
		LiteralsSet& literalSet = literalSetIt.second;

		auto languageMapIt = m_LanguagesMap.find(literalSetIt.first);
		if (languageMapIt != m_LanguagesMap.end())
		{
			LiteralMap& literalMap = languageMapIt->second;
			for (auto literalName : literalSet)
			{
				literalMap.erase(literalName);
			}

			if (literalMap.size() == 0)
			{
				m_LanguagesMap.erase(literalSetIt.first);
			}
		}
	}

	m_ExtendedLiteralsMap.clear();
}

XEResult LocalizationManager::LoadProjectFile(const std::wstring& filename, const std::wstring& projectPath)
{
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	XEAssert(!filename.empty());
	if (filename.empty())
	{
		return XEResult::EmptyFilename;
	}

	m_FilenameProject = filename;

	return ReloadProjectWithoutLock(projectPath);
}

XEResult LocalizationManager::ReloadAll(const std::wstring& enginePath, const std::wstring& projectPath)
{
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	return ReloadAllWithoutLock(enginePath, projectPath);
}

XEResult LocalizationManager::ReloadAllWithoutLock(const std::wstring& enginePath, const std::wstring& projectPath)
{
	XEResult ret = XEResult::Ok;

	ret = ReloadEngineWithoutLock(enginePath);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	ret = ReloadProjectWithoutLock(projectPath);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	return XEResult::Ok;
}

XEResult LocalizationManager::ReloadEngineWithoutLock(const std::wstring& enginePath)
{
	if (m_FilenameEngine.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEAssert(!enginePath.empty());
	if (enginePath.empty())
	{
		return XEResult::EmptyString;
	}

	m_IsReady = false;

	XEXMLParser newFile;
	if (newFile.LoadFile(m_FilenameEngine) != XEResult::Ok)
	{
		/*Hard coded string Loc Manager not loaded*/
		std::wstring msg_error = fmt::format(L"{0}: Could not read file: {1}", __FUNCTIONW__, m_FilenameEngine);

		m_CallByLocManager = true;
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		m_CallByLocManager = false;

		return XEResult::OpenFileFail;
	}

	bool ret = false;

	//Clear all maps for new load
	ClearAllMaps();

	XEXMLParser localizationXML = newFile[XE_LOC_LOCALIZATION_NODE_NAME];
	if (localizationXML.IsReady())
	{
		uint16_t l_Count = localizationXML.GetNumChildren();

		for (uint16_t i = 0; i < l_Count; ++i)
		{
			XEXMLParser child = localizationXML(i);

			std::wstring l_Type = child.GetName();

			if (l_Type.compare(XE_LOC_DEFAULT_LANG_NODE_NAME) == 0)
			{
				m_DefaultLanguage = child.GetString(XE_LOC_LANGUAGE_PROP_NAME);
			}
			else if (l_Type.compare(XE_LOC_LANGUAGE_NODE_NAME) == 0)
			{
				std::wstring language = child.GetString(XE_LOC_LANG_NAME_PROP_NAME);

				std::wstring langFile = enginePath + child.GetString(XE_LOC_FILE_PROP_NAME);

				XEResult loadLangRet = LoadLanguageLiterals(language, langFile);
				if (loadLangRet != XEResult::Ok)
				{
					/*Hard coded string Loc Manager not loaded*/
					std::wstring msg_error = fmt::format(L"{0}: Failed to read Language Literals: {1}, from file: {2}", __FUNCTIONW__, language, langFile);

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

	if (!ret || sdl != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult LocalizationManager::ReloadProjectWithoutLock(const std::wstring& projectPath)
{
	//////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_FilenameProject.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEAssert(!projectPath.empty());
	if (projectPath.empty())
	{
		return XEResult::EmptyString;
	}

	XEXMLParser newFile;
	if (newFile.LoadFile(m_FilenameProject) != XEResult::Ok)
	{
		XETODO("Add literal");

		std::wstring msg_error = fmt::format(L"{0}: Could not read file: {1}", __FUNCTIONW__, m_FilenameProject);

		m_CallByLocManager = true;
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		m_CallByLocManager = false;

		return XEResult::OpenFileFail;
	}

	bool ret = true;

	//Clear all maps for new load
	CleanProjectInfo();

	XEXMLParser localizationXML = newFile[XE_LOC_LOCALIZATION_NODE_NAME];
	if (localizationXML.IsReady())
	{
		uint16_t l_Count = localizationXML.GetNumChildren();

		for (uint16_t i = 0; i < l_Count; ++i)
		{
			XEXMLParser child = localizationXML(i);

			std::wstring l_Type = child.GetName();

			if (l_Type.compare(XE_LOC_LANGUAGE_NODE_NAME) == 0)
			{
				std::wstring language = child.GetString(XE_LOC_LANG_NAME_PROP_NAME);

				std::wstring langFile = projectPath + child.GetString(XE_LOC_FILE_PROP_NAME);

				XEResult loadLangRet = LoadExtendedLanguageLiterals(language, langFile);
				if (loadLangRet != XEResult::Ok)
				{
					XETODO("Add Literal");

					std::wstring msg_error = fmt::format(L"{0}: Failed to read Language Literals: {1}, from file: {2}", __FUNCTIONW__, language, langFile);

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

	if (!ret)
	{
		return XEResult::Fail;
	}

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
		std::wstring msg_error = fmt::format(L"{0}: Could not read file: {1}", __FUNCTIONW__, file);

		m_CallByLocManager = true;
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		m_CallByLocManager = false;

		return XEResult::OpenFileFail;
	}

	XEXMLParser languageXML = newFile[XE_LOC_LANGUAGE_NODE_NAME];

	if ( languageXML.IsReady() )
	{
		if (m_LanguagesMap.find(name) == m_LanguagesMap.end())
		{
			//Create Literal Map and add it to Map
			m_LanguagesMap[name] = LiteralMap();
		}

		//Get Reference to add Literals to the Map
		LiteralMap& litMap = m_LanguagesMap[name];

		uint16_t l_Count = languageXML.GetNumChildren();
		for( uint16_t i = 0; i < l_Count; ++i )
		{
			XEXMLParser child = languageXML(i);

			std::wstring l_Type = child.GetName();

			if (l_Type.compare(XE_LOC_LITERAL_NODE_NAME) == 0)
			{
				std::wstring literalName = child.GetString(XE_LOC_LITERAL_NAME_PROP_NAME);
				std::wstring literalMsg = child.GetString(XE_LOC_LITERAL_MSG_PROP_NAME);

				if(!name.empty())
				{
					if(litMap.find(literalName) == litMap.end()) //If Literal Name is not present
					{						
						litMap[literalName] = LocalizationLiteral(literalName, literalMsg);
					}
					else //If Literal Name is present
					{
						/*Hard coded string Loc Manager not loaded*/
						std::wstring msg_error = fmt::format(L"{0}: Literal: {1}, already exists in the language collection", __FUNCTIONW__, literalName);

						m_CallByLocManager = true;
						XELOGGER->AddNewLog(LogLevel::Warning, msg_error);
						m_CallByLocManager = false;
					}
				}
				else
				{
					/*Hard coded string Loc Manager not loaded*/
					std::wstring msg_error = fmt::format(L"{0}: Msg: {1} does not have a name", __FUNCTIONW__, literalMsg);

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

XEResult LocalizationManager::LoadExtendedLanguageLiterals(const std::wstring& name, const std::wstring& file)
{
	if (file.empty() || name.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLParser newFile;
	if (newFile.LoadFile(file) != XEResult::Ok)
	{
		XETODO("Add literal");


		std::wstring msg_error = fmt::format(L"{0}: Could not read file: {1}", __FUNCTIONW__, file);

		m_CallByLocManager = true;
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		m_CallByLocManager = false;

		return XEResult::OpenFileFail;
	}

	XEXMLParser languageXML = newFile[XE_LOC_LANGUAGE_NODE_NAME];

	if (languageXML.IsReady())
	{
		if (m_LanguagesMap.find(name) == m_LanguagesMap.end())
		{
			//Create Literal Map and add it to Map
			m_LanguagesMap[name] = LiteralMap();
		}

		if (m_ExtendedLiteralsMap.find(name) != m_ExtendedLiteralsMap.end())
		{
			m_ExtendedLiteralsMap[name] = LiteralsSet();
		}

		//Get Reference to add Literals to the Map
		LiteralMap& litMap = m_LanguagesMap[name];
		LiteralsSet& litSet = m_ExtendedLiteralsMap[name];

		uint16_t l_Count = languageXML.GetNumChildren();
		for (uint16_t i = 0; i < l_Count; ++i)
		{
			XEXMLParser child = languageXML(i);

			std::wstring l_Type = child.GetName();

			if (l_Type.compare(XE_LOC_LITERAL_NODE_NAME) == 0)
			{
				std::wstring literalName = child.GetString(XE_LOC_LITERAL_NAME_PROP_NAME);
				std::wstring literalMsg = child.GetString(XE_LOC_LITERAL_MSG_PROP_NAME);

				if (!name.empty())
				{
					if (litMap.find(literalName) == litMap.end())
					{
						litSet.insert(literalName);

						litMap[literalName] = LocalizationLiteral(literalName, literalMsg);
					}
					else
					{
						XETODO("Add literal");


						std::wstring msg_error = fmt::format(L"{0}: Literal: {1}, already exists in the language collection", __FUNCTIONW__, literalName);

						m_CallByLocManager = true;
						XELOGGER->AddNewLog(LogLevel::Warning, msg_error);
						m_CallByLocManager = false;
					}
				}
				else
				{
					XETODO("Add literal");


					std::wstring msg_error = fmt::format(L"{0}: Msg: {1} does not have a name", __FUNCTIONW__, literalMsg);

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

	auto it = m_CurrentLiteralMap->find(literalName);

	if(it != m_CurrentLiteralMap->end())
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
		m_CurrentLiteralMap = &it->second;

		return XEResult::Ok;
	}

	return XEResult::NotFound;
}

XEResult LocalizationManager::AddExtendedLanguage(const std::wstring& languageName)
{
	//////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	//////////////////////////////////
	//Lock
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	//////////////////////////////////
	//Extended Language Does not exist
	if (m_ExtendedLiteralsMap.find(languageName) != m_ExtendedLiteralsMap.end())
	{
		return XEResult::ObjExists;
	}

	//////////////////////////////////
	//Create Extended Language
	m_ExtendedLiteralsMap[languageName] = LiteralsSet();

	//////////////////////////////////
	//Create Language if none found in Language Map
	if (m_LanguagesMap.find(languageName) == m_LanguagesMap.end())
	{
		m_LanguagesMap[languageName] = LiteralMap();
	}

	return XEResult::Ok;
}

XEResult LocalizationManager::AddExtendedLiteral(const std::wstring& languageName, const std::wstring& literalName, const std::wstring& literalMessage)
{
	//////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	//////////////////////////////////
	//Lock
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	//////////////////////////////////
	//Extended Language Exists
	if (m_ExtendedLiteralsMap.find(languageName) == m_ExtendedLiteralsMap.end())
	{
		return XEResult::NotFound;
	}

	//////////////////////////////////
	//Add or Modify Message
	LiteralsSet& literalSet = m_ExtendedLiteralsMap[languageName];
	LiteralMap& literalMap = m_LanguagesMap[languageName];

	auto literalSetIt = literalSet.find(literalName);

	if (literalSetIt != literalSet.end())
	{
		//////////////////////////////////
		//Modify Message
		literalMap[literalName].m_Msg = literalMessage;
	}
	else if (literalMap.find(literalName) == literalMap.end()) //If Literal Set 
	{
		//////////////////////////////////
		//Add to Set
		literalSet.insert(literalName);

		//////////////////////////////////
		//Add Message to Literal Language Map
		literalMap[literalName] = LocalizationLiteral(literalName, literalMessage);
	}
	else
	{
		//////////////////////////////////
		//Literal exists in default Engine config, invalid option

		XETODO("Add log");

		XETODO("Better return type");
		return XEResult::Fail;
	}

	//////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult LocalizationManager::RemoveExtendedLanguage(const std::wstring& languageName)
{
	//////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	//////////////////////////////////
	//Lock
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	//////////////////////////////////
	//Extended Language Exists
	if (m_ExtendedLiteralsMap.find(languageName) == m_ExtendedLiteralsMap.end())
	{
		return XEResult::NotFound;
	}

	//////////////////////////////////
	//Remove Literals from Language Map
	LiteralsSet& literalSet = m_ExtendedLiteralsMap[languageName];
	LiteralMap& literalMap = m_LanguagesMap[languageName];

	for (auto literalName : literalSet)
	{
		literalMap.erase(literalName);
	}

	//////////////////////////////////
	//Remove Language Map if size is 0
	if (literalMap.size() == 0)
	{
		m_LanguagesMap.erase(languageName);
	}

	//////////////////////////////////
	//Remove Extended Literal Map
	m_ExtendedLiteralsMap.erase(languageName);

	//////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult LocalizationManager::RemoveExtendedLiteral(const std::wstring& languageName, const std::wstring& literalName)
{
	//////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	//////////////////////////////////
	//Lock
	std::lock_guard<std::mutex> lock(m_LocalizationMutex);

	//////////////////////////////////
	//Extended Language Exists
	if (m_ExtendedLiteralsMap.find(languageName) == m_ExtendedLiteralsMap.end())
	{
		return XEResult::NotFound;
	}

	//////////////////////////////////
	//Find in Literal Set and Remove
	LiteralsSet& literalSet = m_ExtendedLiteralsMap[languageName];

	if (literalSet.find(literalName) == literalSet.end())
	{
		return XEResult::NotFound;
	}

	literalSet.erase(literalName);

	//////////////////////////////////
	//Remove from Language Map
	LiteralMap& literalMap = m_LanguagesMap[languageName];

	literalMap.erase(literalName);

	//////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult LocalizationManager::SaveToXML(const std::wstring& filename, const std::wstring& projectDir) const
{
	//////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (filename.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLWriter xmlWriter;
	XEResult ret = XEResult::Ok;

	ret = xmlWriter.CreateXML(filename, true);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.StartNode(XE_LOC_LOCALIZATION_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (m_ExtendedLiteralsMap.size() != 0)
	{
		ret = xmlWriter.StartNode(XE_LOC_LANGUAGE_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		for (auto extendedLiteralIt : m_ExtendedLiteralsMap)
		{
			xmlWriter.WriteString(XE_LOC_LANG_NAME_PROP_NAME, extendedLiteralIt.first);

			std::wstring filename = XE_PROJ_LOCALIZATION_DIR_LOC;
			filename += L"/" + extendedLiteralIt.first + L".xml";
			xmlWriter.WriteString(XE_LOC_FILE_PROP_NAME, filename);

			filename = projectDir + L"/" + filename;
			ret = SaveToXMLExtentedLiteral(filename, extendedLiteralIt.second, extendedLiteralIt.first);
			if (ret != XEResult::Ok)
			{
				XETODO("Add error");

				XETODO("Better return code");
				return XEResult::Fail;
			}
		}

		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.FinalizeXML();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	//////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult LocalizationManager::SaveToXMLExtentedLiteral(const std::wstring& filename, const LiteralsSet& literalSet, const std::wstring& languageName) const
{
	if (filename.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLWriter xmlWriter;
	XEResult ret = XEResult::Ok;

	ret = xmlWriter.CreateXML(filename, true);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.StartNode(XE_LOC_LANGUAGE_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	auto langMapIt = m_LanguagesMap.find(languageName);
	if (langMapIt != m_LanguagesMap.end())
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	const LiteralMap& literalMap = langMapIt->second;
	for (auto literalSetName : literalSet)
	{
		ret = xmlWriter.StartNode(XE_LOC_LITERAL_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		xmlWriter.WriteString(XE_LOC_LITERAL_NAME_PROP_NAME, literalSetName);

		xmlWriter.WriteString(XE_LOC_LITERAL_MSG_PROP_NAME, literalMap.find(literalSetName)->second.m_Msg);

		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.FinalizeXML();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	//////////////////////////////////
	//Finish
	return XEResult::Ok;
}
