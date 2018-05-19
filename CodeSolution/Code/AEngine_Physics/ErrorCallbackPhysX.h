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

#ifndef _ERROR_CALLBACK_PHYSX_H
#define _ERROR_CALLBACK_PHYSX_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class ErrorCallbackPhysX sealed : public physx::PxErrorCallback
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default ErrorCallbackPhysX Constructor
        /// </summary>
        ErrorCallbackPhysX();

        /// <summary>
        /// Default ErrorCallbackPhysX Destructor
        /// </summary>
        virtual ~ErrorCallbackPhysX();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

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

        void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);

#pragma endregion

};

#endif
