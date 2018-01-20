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

#ifndef _RAW_GAME_ASSET_H
#define _RAW_GAME_ASSET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Time\AETimeDefs.h"
#include "GameContentDefs.h"
#include "Base\UniqueAEObjectNamed.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class RawGameAsset sealed : public UniqueAEObjectNamed
{
    friend class GameAssetManager;
    
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Content Type of the Raw File
        /// <summary>
        GameContentType m_ContentType = GameContentType::Unknown;

        /// <summary>
        /// Content Sub Type of the Raw File, specifying even more how is it to be imported
        /// <summary>
        GameContentSubtype m_ContentSubtype = GameContentSubtype::Unknown;

        /// <summary>
        /// File Extension Type
        /// <summary>
        GameContextFileExt m_GameContextFileExt = GameContextFileExt::Unknown;

        /// <summary>
        /// Last Modified Time when the Raw File was modified. This is use to know that we need to import again the file
        /// <summary>
        TimeStamp m_LastModifiedTimeStamp;

        /// <summary>
        /// File Path of the Raw Asset to import
        /// <summary>
        std::wstring m_FilePath = L"";

        /// <summary>
        /// Project Directory for the Raw Asset File
        /// <summary>
        std::wstring m_ProjectDirectory = L"";

        /// <summary>
        /// File Name to use when outputting the Raw Asset File
        /// <summary>
        std::wstring m_OutputFileName = L"";

        /// <summary>
        /// Lets know if the Raw File needs to be imported again
        /// <summary>
        bool m_ReloadNeeded = false;

        /// <summary>
        /// Lets know if the Raw File needs to be imported again, because the content subtype has changed
        /// <summary>
        bool m_ContentSubtypeChanged = false;

        /// <summary>
        /// Associated Asset ID
        /// <summary>
        /// <remarks>
        /// This is the Asset it produces when the Raw Asset is imported
        /// </remarks>
        uint64_t m_UniqueAssociatedAssetID = 0;

        /// <summary>
        /// Custom name of the Game Asset
        /// </summary>
        std::wstring m_CustomName = L"";

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Sets the Asset ID that this Raw Asset Imported
        /// </summary>
        /// <remakrs>
        /// This methods is private so only the friend class Game Asset Manager can call it
        /// </remarks>
        void SetUniqueAssociatedAssetID(uint64_t uniqueAssociatedAssetID)
        {
            m_UniqueAssociatedAssetID = uniqueAssociatedAssetID;
        }

        /// <summary>
        /// Tells Raw Game Asset it has been successfully imported
        /// </summary>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult ImportDone();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// RawGameAsset Constructor
        /// </summary>
        /// <param name="filePath">File Path of the Raw Asset to import</param>
        /// <param name="projectDirectory">Project Directory</param>
        /// <param name="name">Name of the Raw Asset</param>
        RawGameAsset(const std::wstring& filePath, const std::wstring& projectDirectory, const std::wstring& name = L"");

        /// <summary>
        /// Default RawGameAsset Destructor
        /// </summary>
        virtual ~RawGameAsset();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets the Content Type of the Raw File
        /// </summary>
        /// <returns>Content Type of the Raw File</returns>
        inline GameContentType GetContentType() const
        {
            return m_ContentType;
        }

        /// <summary>
        /// Gets Content Sub Type of the Raw File
        /// </summary>
        /// <returns>Content Sub Type of the Raw File</returns>
        inline GameContentSubtype GetContentSubtype() const
        {
            return m_ContentSubtype;
        }

        /// <summary>
        /// Gets File Extension Type of the Raw File
        /// </summary>
        /// <returns>File Extension Type of the Raw File</returns>
        inline GameContextFileExt GetGameContextFileExt() const
        {
            return m_GameContextFileExt;
        }

        /// <summary>
        /// Gets the Last Modified Time when the Raw File was modified.
        /// </summary>
        /// <returns>Last Modified Time of Raw Asset File</returns>
        inline const TimeStamp& GetLastModifiedTimeStamp() const
        {
            return m_LastModifiedTimeStamp;
        }

        /// <summary>
        /// File Path of the Raw Asset to import
        /// </summary>
        /// <returns>File path of Raw Asset</returns>
        inline const std::wstring& GetFilePath() const
        {
            return m_FilePath;
        }

        /// <summary>
        /// File Name to Use when outputting the Raw Asset File
        /// </summary>
        /// <returns>Output File Name</returns>
        inline const std::wstring& GetOutputFileName() const
        {
            return m_OutputFileName;
        }

        /// <summary>
        /// Lets know if the Raw File needs to be imported again
        /// </summary>
        /// <returns>True if reload of the file is needed</returns>
        bool IsReloadNeeded() const;

        /// <summary>
        /// Gets the Associated Asset ID that was created when this Raw Asset was imported
        /// </summary>
        /// <returns>Associated Asset ID</returns>
        uint64_t GetUniqueAssociatedAssetID() const
        {
            return m_UniqueAssociatedAssetID;
        }

        /// <summary>
        /// Custom Name of the Game Asset
        /// </summary>
        /// <returns>Custom Name of Game Asset</returns>
        inline const std::wstring& GetCustomName() const
        {
            return m_CustomName;
        }

        bool GetContentSubtypeChanged() const
        {
            return m_ContentSubtypeChanged;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        /// <summary>
        /// Sets the Content type for the Raw Asset File, this is set to be a more specific when importing
        /// </summary>
        /// <param name="contentType">Content Type of raw asset</param>
        void SetContentType(GameContentType contentType);

        /// <summary>
        /// Sets the Content Subtype for the Raw Asset File, this is set to be a more specific when importing
        /// </summary>
        /// <param name="contentSubtype">Content SubType of raw asset</param>
        void SetContentSubtype(GameContentSubtype contentSubtype);

        void SetGameContextFileExt(GameContextFileExt gameContextFileExt);

        void SetLastModifiedTimeStamp(TimeStamp lastModifiedTimeStamp);

        void SetOutputFileName(std::wstring outputFileName);

        /// <summary>
        /// Sets the Custom Name of the Game Asset
        /// </summary>
        inline void SetCustomName(const std::wstring& customName)
        {
            m_CustomName = customName;
        }

        inline void SetReloadNeeded(bool reloadNeeded)
        {
            m_ReloadNeeded = reloadNeeded;
        }

        inline void SetContentSubtypeChanged(bool contentSubtypeChanged)
        {
            m_ContentSubtypeChanged = contentSubtypeChanged;
        }

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Parses the Raw Game Asset File to determine Content Type and Sub Content Type
        /// </summary>
        /// <remarks>
        /// If Sub Content Type if left to Unknown, user has to input it
        /// </remarks>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult ParseRawGameAssetFile();

        /// <summary>
        /// Checks if the Raw Asset File needs to be reloaded
        /// </summary>
        /// <remarks>
        /// If reload is needed GetReloadNeeded will return true
        /// </remarks>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult CheckIfLatest();
        
#pragma endregion

};

#endif
