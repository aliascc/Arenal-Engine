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

#ifndef _TEXTURE_ARRAY_H
#define _TEXTURE_ARRAY_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Textures\TextureDefs.h"
#include "Shaders\Bindings\ShaderBinding.h"

/********************
*   Forward Decls   *
*********************/
class Texture;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class TextureArray abstract : public ShaderBinding
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Type of the Texture
        /// </summary>
        TextureType m_TextureType = TextureType::Texture2D;

#pragma endregion

    protected:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Protected Variables

        /// <summary>
        /// Width of the Texture
        /// </summary>
        uint32_t m_Width = 0;

        /// <summary>
        /// Height of the Texture
        /// </summary>
        uint32_t m_Height = 0;

        /// <summary>
        /// Size of the Array
        /// </summary>
        uint32_t m_ArraySize = 0;

        /// <summary>
        /// Number of Mip Levels of the textures
        /// </summary>
        uint32_t m_MipLevels = 0;

        /// <summary>
        /// Format of the Textures
        /// </summary>
        DXGI_FORMAT m_Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

        /// <summary>
        /// Determines if the class is ready to process data
        /// </summary>
        bool m_IsReady = false;

        /// <summary>
        /// Graphic Device associated with the Texture Array
        /// </summary>
        GraphicDevice& m_GraphicDevice;

        /// <summary>
        /// DirectX Shader Resource View use to bind the Texture Array to a Shader
        /// </summary>
        ID3D11ShaderResourceView* m_TextureArraySRV = nullptr;

        /// <summary>
        /// Texture Bind option in the Array
        /// </summary>
        TextureBindOption m_TextureBindOption = TextureBindOption::Default;

#pragma endregion

        /************************
        *   Protected Methods   *
        *************************/
#pragma region Protected Methods

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default TextureArray Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Texture</param>
        /// <param name="name">Name of the Texture Array</param>
        /// <param name="textureType">Type of the Texture of the Array</param>
        /// <param name="bindIndex">Index where to bind</param>
        TextureArray(GraphicDevice& graphicDevice, const std::string& name, TextureType textureType, uint32_t bindIndex);

        /// <summary>
        /// Default TextureArray Destructor
        /// </summary>
        virtual ~TextureArray();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// DirectX Shader Resource use to bind the Texture Array to a Shader
        /// </summary>
        /// <returns>Returns DirectX Shader Texture Resource</returns>
        inline ID3D11ShaderResourceView* GetDXTextureArraySRV() const
        {
            return m_TextureArraySRV;
        }

        /// <summary>
        /// Gets the Texture Type of the Array
        /// </summary>
        /// <returns>Get the Type of the Texture of the Array</returns>
        inline TextureType GetTextureType() const
        {
            return m_TextureType;
        }

        /// <summary>
        /// Gets if the Texture Array is ready
        /// </summary>
        /// <returns>True if it is ready</returns>
        inline bool IsReady() const
        {
            return m_IsReady;
        }

        /// <summary>
        /// Gets the size of the Array
        /// </summary>
        /// <returns>Size of the Array</returns>
        inline uint32_t GetSize() const
        {
            return m_ArraySize;
        }

        /// <summary>
        /// Gets Texture Bind option in the Array
        /// </summary>
        /// <returns>Texture Bind Option</returns>
        inline TextureBindOption GetTextureBindOption() const
        {
            return m_TextureBindOption;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Copies the Texture to the array at an specific index
        /// </summary>
        /// <param name="index">Index of the Array</param>
        /// <param name="texture">Texture to copy</param>
        /// <returns>Returns AEResult::Ok if successful</returns>
        virtual AEResult SetTexture(uint32_t index, Texture* texture) = 0;

        /// <summary>
        /// Deinitializies the Texture Array
        /// </summary>
        /// <returns>Returns AEResult::Ok if successful</returns>
        virtual AEResult Deinitialize() = 0;

#pragma endregion

};

#endif
