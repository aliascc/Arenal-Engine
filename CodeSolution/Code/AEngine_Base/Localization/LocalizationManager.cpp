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

/***************************
*   Game Engine Includes   *
****************************/
#include "Logger\Logger.h"
#include "XML\AEXMLWriter.h"
#include "XML\AEXMLParser.h"
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

AEResult LocalizationManager::Initialize(const std::string& file, const std::string& enginePath)
{
    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    AEAssert(!file.empty());
    if(file.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_FilenameEngine = file;

    return ReloadEngineWithoutLock(enginePath);
}

void LocalizationManager::ClearAllMaps()
{
    m_DefaultLanguage = "";
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

AEResult LocalizationManager::LoadProjectFile(const std::string& filename, const std::string& projectPath)
{
    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    AEAssert(!filename.empty());
    if (filename.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_FilenameProject = filename;

    return ReloadProjectWithoutLock(projectPath);
}

AEResult LocalizationManager::ReloadAll(const std::string& enginePath, const std::string& projectPath)
{
    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    return ReloadAllWithoutLock(enginePath, projectPath);
}

AEResult LocalizationManager::ReloadAllWithoutLock(const std::string& enginePath, const std::string& projectPath)
{
    AEResult ret = AEResult::Ok;

    ret = ReloadEngineWithoutLock(enginePath);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    ret = ReloadProjectWithoutLock(projectPath);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    return AEResult::Ok;
}

AEResult LocalizationManager::ReloadEngineWithoutLock(const std::string& enginePath)
{
    if (m_FilenameEngine.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEAssert(!enginePath.empty());
    if (enginePath.empty())
    {
        return AEResult::EmptyString;
    }

    m_IsReady = false;

    AEXMLParser newFile;
    if (newFile.LoadFile(m_FilenameEngine) != AEResult::Ok)
    {
        /*Hard coded string Loc Manager not loaded*/
        std::string msg_error = fmt::format("{0}: Could not read file: {1}", __FUNCTION__, m_FilenameEngine);

        m_CallByLocManager = true;
        AELOGGER->AddNewLog(LogLevel::Error, msg_error);
        m_CallByLocManager = false;

        return AEResult::OpenFileFail;
    }

    bool ret = false;

    //Clear all maps for new load
    ClearAllMaps();

    AEXMLParser localizationXML = newFile[AE_LOC_LOCALIZATION_NODE_NAME];
    if (localizationXML.IsReady())
    {
        uint16_t l_Count = localizationXML.GetNumChildren();

        for (uint16_t i = 0; i < l_Count; ++i)
        {
            AEXMLParser child = localizationXML(i);

            std::string l_Type = child.GetName();

            if (l_Type.compare(AE_LOC_DEFAULT_LANG_NODE_NAME) == 0)
            {
                m_DefaultLanguage = child.GetString(AE_LOC_LANGUAGE_PROP_NAME);
            }
            else if (l_Type.compare(AE_LOC_LANGUAGE_NODE_NAME) == 0)
            {
                std::string language = child.GetString(AE_LOC_LANG_NAME_PROP_NAME);

                std::string langFile = enginePath + child.GetString(AE_LOC_FILE_PROP_NAME);

                AEResult loadLangRet = LoadLanguageLiterals(language, langFile);
                if (loadLangRet != AEResult::Ok)
                {
                    /*Hard coded string Loc Manager not loaded*/
                    std::string msg_error = fmt::format("{0}: Failed to read Language Literals: {1}, from file: {2}", __FUNCTION__, language, langFile);

                    m_CallByLocManager = true;
                    AELOGGER->AddNewLog(LogLevel::Error, msg_error);
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
    AEResult sdl = SetDefaultLanguageWithoutLock(m_DefaultLanguage);

    if (!ret || sdl != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult LocalizationManager::ReloadProjectWithoutLock(const std::string& projectPath)
{
    //////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_FilenameProject.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEAssert(!projectPath.empty());
    if (projectPath.empty())
    {
        return AEResult::EmptyString;
    }

    AEXMLParser newFile;
    if (newFile.LoadFile(m_FilenameProject) != AEResult::Ok)
    {
        AETODO("Add literal");

        std::string msg_error = fmt::format("{0}: Could not read file: {1}", __FUNCTION__, m_FilenameProject);

        m_CallByLocManager = true;
        AELOGGER->AddNewLog(LogLevel::Error, msg_error);
        m_CallByLocManager = false;

        return AEResult::OpenFileFail;
    }

    bool ret = true;

    //Clear all maps for new load
    CleanProjectInfo();

    AEXMLParser localizationXML = newFile[AE_LOC_LOCALIZATION_NODE_NAME];
    if (localizationXML.IsReady())
    {
        uint16_t l_Count = localizationXML.GetNumChildren();

        for (uint16_t i = 0; i < l_Count; ++i)
        {
            AEXMLParser child = localizationXML(i);

            std::string l_Type = child.GetName();

            if (l_Type.compare(AE_LOC_LANGUAGE_NODE_NAME) == 0)
            {
                std::string language = child.GetString(AE_LOC_LANG_NAME_PROP_NAME);

                std::string langFile = projectPath + child.GetString(AE_LOC_FILE_PROP_NAME);

                AEResult loadLangRet = LoadExtendedLanguageLiterals(language, langFile);
                if (loadLangRet != AEResult::Ok)
                {
                    AETODO("Add Literal");

                    std::string msg_error = fmt::format("{0}: Failed to read Language Literals: {1}, from file: {2}", __FUNCTION__, language, langFile);

                    m_CallByLocManager = true;
                    AELOGGER->AddNewLog(LogLevel::Error, msg_error);
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
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult LocalizationManager::LoadLanguageLiterals(const std::string& name, const std::string& file)
{
    if(file.empty() || name.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEXMLParser newFile;
    if (newFile.LoadFile(file) != AEResult::Ok)
    {
        /*Hard coded string Loc Manager not loaded*/
        std::string msg_error = fmt::format("{0}: Could not read file: {1}", __FUNCTION__, file);

        m_CallByLocManager = true;
        AELOGGER->AddNewLog(LogLevel::Error, msg_error);
        m_CallByLocManager = false;

        return AEResult::OpenFileFail;
    }

    AEXMLParser languageXML = newFile[AE_LOC_LANGUAGE_NODE_NAME];

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
            AEXMLParser child = languageXML(i);

            std::string l_Type = child.GetName();

            if (l_Type.compare(AE_LOC_LITERAL_NODE_NAME) == 0)
            {
                std::string literalName    = child.GetString(AE_LOC_LITERAL_NAME_PROP_NAME);
                std::string literalMsg     = child.GetString(AE_LOC_LITERAL_MSG_PROP_NAME);

                if(!name.empty())
                {
                    if(litMap.find(literalName) == litMap.end()) //If Literal Name is not present
                    {                        
                        litMap[literalName] = LocalizationLiteral(literalName, literalMsg);
                    }
                    else //If Literal Name is present
                    {
                        /*Hard coded string Loc Manager not loaded*/
                        std::string msg_error = fmt::format("{0}: Literal: {1}, already exists in the language collection", __FUNCTION__, literalName);

                        m_CallByLocManager = true;
                        AELOGGER->AddNewLog(LogLevel::Warning, msg_error);
                        m_CallByLocManager = false;
                    }
                }
                else
                {
                    /*Hard coded string Loc Manager not loaded*/
                    std::string msg_error = fmt::format("{0}: Msg: {1} does not have a name", __FUNCTION__, literalMsg);

                    m_CallByLocManager = true;
                    AELOGGER->AddNewLog(LogLevel::Warning, msg_error);
                    m_CallByLocManager = false;
                }
            }
        }

        return AEResult::Ok;
    }

    return AEResult::Fail;
}

AEResult LocalizationManager::LoadExtendedLanguageLiterals(const std::string& name, const std::string& file)
{
    if (file.empty() || name.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEXMLParser newFile;
    if (newFile.LoadFile(file) != AEResult::Ok)
    {
        AETODO("Add literal");


        std::string msg_error = fmt::format("{0}: Could not read file: {1}", __FUNCTION__, file);

        m_CallByLocManager = true;
        AELOGGER->AddNewLog(LogLevel::Error, msg_error);
        m_CallByLocManager = false;

        return AEResult::OpenFileFail;
    }

    AEXMLParser languageXML = newFile[AE_LOC_LANGUAGE_NODE_NAME];

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
            AEXMLParser child = languageXML(i);

            std::string l_Type = child.GetName();

            if (l_Type.compare(AE_LOC_LITERAL_NODE_NAME) == 0)
            {
                std::string literalName    = child.GetString(AE_LOC_LITERAL_NAME_PROP_NAME);
                std::string literalMsg     = child.GetString(AE_LOC_LITERAL_MSG_PROP_NAME);

                if (!name.empty())
                {
                    if (litMap.find(literalName) == litMap.end())
                    {
                        litSet.insert(literalName);

                        litMap[literalName] = LocalizationLiteral(literalName, literalMsg);
                    }
                    else
                    {
                        AETODO("Add literal");


                        std::string msg_error = fmt::format("{0}: Literal: {1}, already exists in the language collection", __FUNCTION__, literalName);

                        m_CallByLocManager = true;
                        AELOGGER->AddNewLog(LogLevel::Warning, msg_error);
                        m_CallByLocManager = false;
                    }
                }
                else
                {
                    AETODO("Add literal");


                    std::string msg_error = fmt::format("{0}: Msg: {1} does not have a name", __FUNCTION__, literalMsg);

                    m_CallByLocManager = true;
                    AELOGGER->AddNewLog(LogLevel::Warning, msg_error);
                    m_CallByLocManager = false;
                }
            }
        }

        return AEResult::Ok;
    }

    return AEResult::Fail;
}

const std::string& LocalizationManager::GetLiteral(const std::string& literalName)
{
    if(!m_IsReady)
    {
        return AE_LOC_LANG_DEFAULT_LITERAL;
    }

    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    auto it = m_CurrentLiteralMap->find(literalName);

    if(it != m_CurrentLiteralMap->end())
    {
        return it->second.m_Msg;
    }

    return AE_LOC_LANG_DEFAULT_LITERAL;
}

const std::string& LocalizationManager::GetDefaultLanguage() const
{
    if(!m_IsReady)
    {
        return AE_LOC_LANG_NO_LANGUAGE;
    }

    if(m_DefaultLanguage.empty())
    {
        return AE_LOC_LANG_NO_LANGUAGE;
    }

    return m_DefaultLanguage;
}

AEResult LocalizationManager::SetDefaultLanguage(const std::string& languageName)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    return SetDefaultLanguageWithoutLock(languageName);
}

AEResult LocalizationManager::SetDefaultLanguageWithoutLock(const std::string& languageName)
{
    auto it = m_LanguagesMap.find(languageName);

    if(it != m_LanguagesMap.end())
    {
        m_DefaultLanguage = languageName;
        m_CurrentLiteralMap = &it->second;

        return AEResult::Ok;
    }

    return AEResult::NotFound;
}

AEResult LocalizationManager::AddExtendedLanguage(const std::string& languageName)
{
    //////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    //////////////////////////////////
    //Lock
    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    //////////////////////////////////
    //Extended Language Does not exist
    if (m_ExtendedLiteralsMap.find(languageName) != m_ExtendedLiteralsMap.end())
    {
        return AEResult::ObjExists;
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

    return AEResult::Ok;
}

AEResult LocalizationManager::AddExtendedLiteral(const std::string& languageName, const std::string& literalName, const std::string& literalMessage)
{
    //////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    //////////////////////////////////
    //Lock
    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    //////////////////////////////////
    //Extended Language Exists
    if (m_ExtendedLiteralsMap.find(languageName) == m_ExtendedLiteralsMap.end())
    {
        return AEResult::NotFound;
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

        AETODO("Add log");

        AETODO("Better return type");
        return AEResult::Fail;
    }

    //////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult LocalizationManager::RemoveExtendedLanguage(const std::string& languageName)
{
    //////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    //////////////////////////////////
    //Lock
    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    //////////////////////////////////
    //Extended Language Exists
    if (m_ExtendedLiteralsMap.find(languageName) == m_ExtendedLiteralsMap.end())
    {
        return AEResult::NotFound;
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
    return AEResult::Ok;
}

AEResult LocalizationManager::RemoveExtendedLiteral(const std::string& languageName, const std::string& literalName)
{
    //////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    //////////////////////////////////
    //Lock
    std::lock_guard<std::mutex> lock(m_LocalizationMutex);

    //////////////////////////////////
    //Extended Language Exists
    if (m_ExtendedLiteralsMap.find(languageName) == m_ExtendedLiteralsMap.end())
    {
        return AEResult::NotFound;
    }

    //////////////////////////////////
    //Find in Literal Set and Remove
    LiteralsSet& literalSet = m_ExtendedLiteralsMap[languageName];

    if (literalSet.find(literalName) == literalSet.end())
    {
        return AEResult::NotFound;
    }

    literalSet.erase(literalName);

    //////////////////////////////////
    //Remove from Language Map
    LiteralMap& literalMap = m_LanguagesMap[languageName];

    literalMap.erase(literalName);

    //////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult LocalizationManager::SaveToXML(const std::string& filename, const std::string& projectDir) const
{
    //////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (filename.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEXMLWriter xmlWriter;
    AEResult ret = AEResult::Ok;

    ret = xmlWriter.CreateXML(filename, true);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.StartNode(AE_LOC_LOCALIZATION_NODE_NAME);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (m_ExtendedLiteralsMap.size() != 0)
    {
        ret = xmlWriter.StartNode(AE_LOC_LANGUAGE_NODE_NAME);
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }

        for (auto extendedLiteralIt : m_ExtendedLiteralsMap)
        {
            xmlWriter.WriteString(AE_LOC_LANG_NAME_PROP_NAME, extendedLiteralIt.first);

            std::string filename = AE_PROJ_LOCALIZATION_DIR_LOC;
            filename += "/" + extendedLiteralIt.first + ".xml";
            xmlWriter.WriteString(AE_LOC_FILE_PROP_NAME, filename);

            filename = projectDir + "/" + filename;
            ret = SaveToXMLExtentedLiteral(filename, extendedLiteralIt.second, extendedLiteralIt.first);
            if (ret != AEResult::Ok)
            {
                AETODO("Add error");

                AETODO("Better return code");
                return AEResult::Fail;
            }
        }

        ret = xmlWriter.EndNode();
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }
    }

    ret = xmlWriter.EndNode();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.FinalizeXML();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    //////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult LocalizationManager::SaveToXMLExtentedLiteral(const std::string& filename, const LiteralsSet& literalSet, const std::string& languageName) const
{
    if (filename.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEXMLWriter xmlWriter;
    AEResult ret = AEResult::Ok;

    ret = xmlWriter.CreateXML(filename, true);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.StartNode(AE_LOC_LANGUAGE_NODE_NAME);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    auto langMapIt = m_LanguagesMap.find(languageName);
    if (langMapIt != m_LanguagesMap.end())
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    const LiteralMap& literalMap = langMapIt->second;
    for (auto literalSetName : literalSet)
    {
        ret = xmlWriter.StartNode(AE_LOC_LITERAL_NODE_NAME);
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }

        xmlWriter.WriteString(AE_LOC_LITERAL_NAME_PROP_NAME, literalSetName);

        xmlWriter.WriteString(AE_LOC_LITERAL_MSG_PROP_NAME, literalMap.find(literalSetName)->second.m_Msg);

        ret = xmlWriter.EndNode();
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }
    }

    ret = xmlWriter.EndNode();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.FinalizeXML();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    //////////////////////////////////
    //Finish
    return AEResult::Ok;
}
