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

#ifndef _NAMED_H
#define _NAMED_H

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
/// Class that Defines a name for the Object with its Gets and Sets
/// </summary>
class Named abstract : public AEObject
{
    protected:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Protected Variables

        /// <summary>
        /// Name of AEObject
        /// </summary>
        std::string m_Name = "";

#pragma endregion
        
    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Named Constructor
        /// </summary>
        /// <param name="name">Name of the AEObject</param>
        Named(const std::string& name);
        
        /// <summary>
        /// Default Named Destructor
        /// </summary>
        virtual ~Named();

#pragma endregion
        
        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods
        
        /// <summary>
        /// Gets the Name of AEObject
        /// </summary>
        /// <returns>Name of AEObject</returns>
        inline const std::string& GetName() const
        {
            return m_Name;
        }

#pragma endregion
        
        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods
        
        /// <summary>
        /// Sets the Name of the AEObject
        /// </summary>
        /// <param name="name">Name of the AEObject"</param>
        virtual void SetName(const std::string name);

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods
        
        /// <summary>
        /// Converts the Objects to a String. In case of Named it is the name
        /// </summary>
        /// <returns>Name of AEObject</returns>
        virtual std::string ToString() const override;

#pragma endregion

};

#endif
