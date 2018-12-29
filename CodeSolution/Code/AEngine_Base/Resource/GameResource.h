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

#ifndef _GAME_RESOURCE_H
#define _GAME_RESOURCE_H

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
#include "Base\Named.h"
#include "GameResourceDefs.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class GameResource abstract : public Named
{
    friend class GameResourceManager;

    protected:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Protected Variables

        /// <summary>
        /// Reference count to the Game Resource
        /// </summary>
        std::atomic<int64_t> m_RefCount = 0;

        /// <summary>
        /// If Game Resource is assigned to a 
        /// Resource manager this flags is true
        /// </summary>
        bool m_Managed = false;

        /// <summary>
        /// Game Resource Type
        /// </summary>
        GameResourceType m_ResourceType = GameResourceType::Unknown;

        /// <summary>
        /// Boost Mutex to Lock Thread to
        /// perform atomic operations
        /// </summary>
        std::mutex m_GameResourceMutex;

        /// <summary>
        /// Callback to notify Manager of 
        /// Game Resource Release
        /// </summary>
        GameResourceReleaseCallback m_ReleaseCallback = nullptr;

        /// <summary>
        /// FileName from where the Resource was read
        /// </summary>
        std::string m_FileName = "";

        /// <summary>
        /// String that Identifies the Resource in the Resource Manager
        /// </summary>
        std::string m_StringIdentifier = "";

#pragma endregion

        /************************
        *   Protected Methods   *
        *************************/
#pragma region Protected Methods

        /// <summary>
        /// Sets the name of the File that the
        /// the Resource was read from
        /// (if it was loaded from a file)
        /// </summary>
        /// <remarks>
        /// This function will not be thread safe and it is meant to be use when 
        /// a Thread safe function of a Game Resource needs to change the Filename
        /// </remarks>
        /// <param name="fileName">File from where the resource was read</param>
        void SetFileNameWithoutLock(const std::string& fileName);

        /// <summary>
        /// Sets the Name of the Resource
        /// </summary>
        /// <remarks>
        /// This function will not be thread safe and it is meant to be use when 
        /// a Thread safe function of a Game Resource needs to change the Name
        /// </remarks>
        /// <param name="name">Name of the Resource</param>
        void SetNameWithoutLock(const std::string name);

#pragma endregion

        /***********************************
        *   Protected Destructor Methods   *
        ************************************/
#pragma region Protected Destructor Methods

        /// <summary>
        /// Default GameResource Destructor
        /// Protected so only Childs can access it
        /// </summary>
        virtual ~GameResource();

#pragma endregion

    public:

        /**************************
        *   Constructor Methods   *
        ***************************/
#pragma region Constructor Methods

        /// <summary>
        /// GameResource Constructor
        /// </summary>
        /// <param name="resourceName">Resource Name</param>
        /// <param name="resourceType">Game Resource Type</param>
        GameResource(const std::string& resourceName, GameResourceType resourceType);

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Returns Reference Count of the 
        /// Game Resource
        /// </summary>
        /// <returns>Reference Count</returns>
        inline uint32_t GetRefCount() const
        {
            return m_RefCount;
        }

        /// <summary>
        /// Returns what type of Game Resource
        /// the object is
        /// </summary>
        /// <returns>Return Game Resource Type</returns>
        inline GameResourceType GetGameResourceType() const
        {
            return m_ResourceType;
        }

        /// <summary>
        /// Returns if Game Resource is managed
        /// by a Resource Manager
        /// </summary>
        /// <returns>True if resource is managed</returns>
        inline bool IsManaged() const
        {
            return m_Managed;
        }

        /// <summary>
        /// Returns name of the File that the
        /// the Resource was read from
        /// (if it was loaded from a file)
        /// </summary>
        /// <returns>File Name of the Resource</returns>
        inline const std::string& GetFileName() const
        {
            return m_FileName;
        }

        /// <summary>
        /// Returns the String Identifier use in the Game Resource Manager
        /// </summary>
        /// <returns>String Identifier use in Game Resource Manager, empty if none is set</returns>
        inline const std::string& GetStringIdentifier() const
        {
            return m_StringIdentifier;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        /// <summary>
        /// Sets the name of the File that the
        /// the Resource was read from
        /// (if it was loaded from a file)
        /// </summary>
        /// <param name="fileName">File from where the resource was read</param>
        void SetFileName(const std::string& fileName);
        
        /// <summary>
        /// Sets the Name of the Resource
        /// </summary>
        /// <param name="name">Name of the Resource</param>
        void SetName(const std::string name) override;

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Decreases the reference count to this resource, if it reaches 0 and Keep Alive is false, it will delete it self
        /// </summary>
        /// <returns>Number of references remaining</returns>
        int64_t Release();

        /// <summary>
        /// Increases the reference count and returns a pointer to itself
        /// </summary>
        /// <returns>An exact reference to this Game Resource</returns>
        virtual GameResource* AddRef();

        /// <summary>
        /// Load the Game Resource
        /// </summary>
        /// <returns>AEResult::Ok if it was successful</returns>
        virtual AEResult Load() = 0;

#pragma endregion

};

#endif
