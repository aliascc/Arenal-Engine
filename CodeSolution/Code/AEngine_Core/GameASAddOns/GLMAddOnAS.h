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

#ifndef _GLM_ADD_ON_AS_H
#define _GLM_ADD_ON_AS_H

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

/*************
*   Define   *
**************/
#define AE_AS_MATH_HELPERS_NAMESPACE    "Math"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class GLMAddOnAS sealed : public AngelScriptAddOn
{
    private:

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Registers GLM by a Native Interface
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGLM_Native(asIScriptEngine* engine);

        /// <summary>
        /// Registers GLM Objects by a Native Interface
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGLM_Objects_Native(asIScriptEngine* engine);

        /// <summary>
        /// Registers GLM Vector 3 Functions by a Native Interface
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGLM_Vec3_Functions_Native(asIScriptEngine* engine);

        /// <summary>
        /// Registers GLM Vector 4 Functions by a Native Interface
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGLM_Vec4_Functions_Native(asIScriptEngine* engine);

        /// <summary>
        /// Registers GLM Matrix 4x4 Functions by a Native Interface
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGLM_Mat4_Functions_Native(asIScriptEngine* engine);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GLMAddOnAS Constructor
        /// </summary>
        GLMAddOnAS();

        /// <summary>
        /// Default GLMAddOnAS Destructor
        /// </summary>
        virtual ~GLMAddOnAS();

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
