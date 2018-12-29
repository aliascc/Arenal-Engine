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

#ifndef _AE_OBJECT_H
#define _AE_OBJECT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

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
/// Base class for most of all AEngine Classes or structs
/// </summary>
class AEObject abstract
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

protected:

    /**************************
    *   Protected Variables   *
    ***************************/
#pragma region Protected Variables

#pragma endregion


public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

    /// <summary>
    /// AEObject Constructor
    /// </summary>
    AEObject();
        
    /// <summary>
    /// Default AEObject Destructor
    /// </summary>
    virtual ~AEObject();

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

    /// <summary>
    /// Converts the Objects to a String. 
    /// </summary>
    /// <returns>Object in String format</returns>
    virtual std::string ToString() const;

#pragma endregion

};

#endif
