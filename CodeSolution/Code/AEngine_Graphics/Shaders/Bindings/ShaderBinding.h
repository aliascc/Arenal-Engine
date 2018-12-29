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

#ifndef _SHADER_BINDING_H
#define _SHADER_BINDING_H

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
class ShaderBinding abstract : public Named
{
    protected:

        /****************************
         *   Protected Variables    *
         ****************************/
#pragma region Protected Variables
                
        /// <summary>
        /// Index to where to bind
        /// </summary>
        uint32_t m_BindIndex = 0;

#pragma endregion

    public:
        /*****************************************
         *   Constructor & Destructor Methods   *
         *****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// ShaderBinding Constructor
        /// </summary>
        /// <param name="name">Name of the Binding</param>
        /// <param name="bindIndex">Index where to bind</param>
        ShaderBinding(const std::string& name, uint32_t bindIndex);

        /// <summary>
        /// Default ShaderBinding Destructor
        /// </summary>
        virtual ~ShaderBinding();

#pragma endregion

        /********************
         *   Get Methods    *
         ********************/
#pragma region Get Methods

        /// <summary>
        /// Gets Bind Index
        /// </summary>
        /// <returns>Bind Index</returns>
        inline uint32_t GetBindIndex() const 
        { 
            return m_BindIndex; 
        }

#pragma endregion

        /********************
         *   Set Methods    *
         ********************/
#pragma region Set Methods

        /// <summary>
        /// Sets the Bind Index to where to bind to the shader
        /// </summary>
        /// <param name="bindIndex">New Bind Index</param>
        inline void SetBindIndex(uint32_t bindIndex) 
        { 
            m_BindIndex = bindIndex; 
        }

#pragma endregion

};

#endif
