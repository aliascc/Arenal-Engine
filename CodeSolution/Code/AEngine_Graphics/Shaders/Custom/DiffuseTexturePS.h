/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _DIFFUSE_TEXTURE_PS_H
#define _DIFFUSE_TEXTURE_PS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shaders\PixelShader.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IConstantBuffer;

/**************
*   Defines   *
***************/

/// <summary>
/// Pixel Shader PNT Name
/// </summary>
#define AE_DIFFUSE_TEXTURE_PS_NAME            "Diffuse Texture Pixel Shader"

/*****************
*   Class Decl   *
******************/
class DiffuseTexturePS sealed : public PixelShader
{

    private:

        /************************
        *   Private Variables   *
        *************************/
    #pragma region Private Variables

    #pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
    #pragma region Private Methods

    #pragma endregion

        /*********************************
        *   Private Destructor Methods   *
        **********************************/
    #pragma region Private Destructor Methods

        /// <summary>
        /// Default DiffuseTexturePS Destructor
        /// </summary>
        virtual ~DiffuseTexturePS();

    #pragma endregion

    public:

        /**************************
        *   Constructor Methods   *
        ***************************/
    #pragma region Constructor Methods

        /// <summary>
        /// Default DiffuseTexturePS Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Shader</param>
        /// <param name="name">Name of Shader</param>
        DiffuseTexturePS(GraphicDevice& graphicDevice, const std::string& resourceName);

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

        AEResult Load() override;

    #pragma endregion

};

#endif
