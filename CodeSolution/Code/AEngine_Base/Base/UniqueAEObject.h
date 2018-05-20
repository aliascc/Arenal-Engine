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

#ifndef _UNIQUE_AE_OBJECT_H
#define _UNIQUE_AE_OBJECT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Class that contains a Unique ID
/// </summary>
class UniqueAEObject abstract : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Unique ID of the Object
        /// </summary>
        uint64_t m_UniqueID = 0;

#pragma endregion
        
    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default UniqueAEObject Constructor
        /// </summary>
        UniqueAEObject();

        /// <summary>
        /// Default UniqueAEObject Destructor
        /// </summary>
        virtual ~UniqueAEObject();

#pragma endregion

        /********************
        *     Get Methods   *
        *********************/
#pragma region Get Methods

        /// <summary>
        /// Gets Unique ID
        /// </summary>
        /// <returns>Gets the Unique ID of the Object</returns>
        inline uint64_t GetUniqueID() const
        {
            return m_UniqueID;
        }

#pragma endregion
        
        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif