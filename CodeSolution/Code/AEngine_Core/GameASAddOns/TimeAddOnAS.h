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

#ifndef _TIME_ADD_ON_AS_H
#define _TIME_ADD_ON_AS_H

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
#include "AngelScript\Add-ons\AngelScriptAddOn.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class TimeAddOnAS sealed : public AngelScriptAddOn
{
    private:

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Registers Time Stamp Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterTimeStampObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers Timer Params Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterTimerParamsObject(asIScriptEngine* engine);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// TimeAddOnAS Constructor
        /// </summary>
        TimeAddOnAS();

        /// <summary>
        /// Default TimeAddOnAS Destructor
        /// </summary>
        virtual ~TimeAddOnAS();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Registers the Add-on with the AS Engine.
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        AEResult Register(asIScriptEngine* engine) override;

#pragma endregion

};

#endif
