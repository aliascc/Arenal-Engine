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

#ifndef _VIEWPORT_H
#define _VIEWPORT_H

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
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class Viewport sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX Viewport
        /// </summary>
        D3D11_VIEWPORT m_ViewportDX;

        /// <summary>
        /// X Position of the Top of the Viewport
        /// </summary>
        float m_TopLeftX = 0.0f;

        /// <summary>
        /// Y Position of the Top of the Viewport
        /// </summary>
        float m_TopLeftY = 0.0f;

        /// <summary>
        /// Width of the Viewport
        /// </summary>
        float m_Width = 0.0f;

        /// <summary>
        /// Height of the Viewport
        /// </summary>
        float m_Height = 0.0f;

        /// <summary>
        /// Min depth of the viewport. Ranges between 0 and 1
        /// </summary>
        float m_MinDepth = 0.0f;

        /// <summary>
        /// Max depth of the viewport. Ranges between 0 and 1
        /// </summary>
        float m_MaxDepth = 0.0f;

#pragma endregion
        
    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Viewport Constructor
        /// </summary>
        Viewport();

        /// <summary>
        /// Default Viewport Destructor
        /// </summary>
        virtual ~Viewport();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets DX Viewport
        /// </summary>
        /// <returns>Corners of the Frustum</returns>
        inline const D3D11_VIEWPORT& GetViewportDX() const
        {
            return m_ViewportDX;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion

        /*************************
         *   Framework Methods   *
         *************************/
#pragma region Framework Methods

        /// <summary>
        /// Initialize the Viewport
        /// </summary>
        /// <param name="topLeftX">X Position of the Top of the Viewport</param>
        /// <param name="topLeftY">Y Position of the Top of the Viewport</param>
        /// <param name="width">Width of the Viewport</param>
        /// <param name="height">Height of the Viewport</param>
        /// <param name="minDepth">Min depth of the viewport</param>
        /// <param name="maxDepth">Max depth of the viewport</param>
        /// <returns>Returns AEResult::Ok if successful</returns>
        AEResult Initialize(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth);

#pragma endregion

};

#endif
