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

#ifndef _TEXTURE_2D_ARRAY_H
#define _TEXTURE_2D_ARRAY_H

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
#include "TextureArray.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Texture;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class Texture2DArray sealed : public TextureArray
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX Texture
        /// </summary>
        ID3D11Texture2D* m_TextureArrayDX = nullptr;

        /// <summary>
        /// DirectX Render Target Views Vector
        /// </summary>
        std::vector<ID3D11RenderTargetView*> m_RenderTargetViewsVector;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Variables

        /// <summary>
        /// Clean up the memory use by the class
        /// </summary>
        void CleanUp();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default Texture2DArray Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Texture</param>
        /// <param name="name">Name of the Texture Array</param>
        /// <param name="bindIndex">Index where to bind</param>
        Texture2DArray(GraphicDevice* graphicDevice, const std::string& name, uint32_t bindIndex);

        /// <summary>
        /// Default Texture2DArray Destructor
        /// </summary>
        virtual ~Texture2DArray();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets the DirectX Render Target if Bind Option was set to Render Target
        /// </summary>
        /// <param name="index">Index of the Array</param>
        /// <returns>Returns DirectX Render Target if found, null otherwise</returns>
        ID3D11RenderTargetView* GetRenderTargetDX(uint32_t index);

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Initializes the Texture Array
        /// </summary>
        /// <param name="arraySize">Size of the Array</param>
        /// <param name="width">Width of the Textures in the Array</param>
        /// <param name="height">Height of the Textures in the Array</param>
        /// <param name="mipLevels">Mip Levels of the Textures in the Array</param>
        /// <param name="format">Format of the Textures in the Array</param>
        /// <param name="binfOption">Texture bind Option</param>
        /// <returns>Returns AEResult::Ok if successful</returns>
        AEResult Initialize(uint32_t arraySize, uint32_t width, uint32_t height, uint32_t mipLevels, DXGI_FORMAT format, TextureBindOption bindOption = TextureBindOption::Default);

        /// <summary>
        /// Copies the Texture to the array at an specific index
        /// </summary>
        /// <param name="index">Index of the Array</param>
        /// <param name="texture">Texture to copy</param>
        /// <returns>Returns AEResult::Ok if successful</returns>
        AEResult SetTexture(uint32_t index, Texture* texture) override;

        /// <summary>
        /// Deinitializies the Texture Array
        /// </summary>
        /// <returns>Returns AEResult::Ok if successful</returns>
        AEResult Deinitialize() override;

#pragma endregion

};

#endif
