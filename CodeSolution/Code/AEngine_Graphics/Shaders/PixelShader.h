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

#ifndef _PIXEL_SHADER_H
#define _PIXEL_SHADER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shader.h"
#include "Base\Base.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IConstantBuffer;

/*****************
*   Class Decl   *
******************/

class PixelShader : public Shader
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX11 Pixel Shader
        /// </summary>
        ID3D11PixelShader* m_PSDX = nullptr;

#pragma endregion

    protected:

        /************************
        *   Protected Methods   *
        *************************/
#pragma region Private Methods

        AEResult LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length) override;

#pragma endregion

        /***********************************
        *   Protected Destructor Methods   *
        ************************************/
#pragma region Private Destructor Methods

        /// <summary>
        /// Default PixelShader Destructor
        /// </summary>
        virtual ~PixelShader();

#pragma endregion

    public:

        /**************************
        *   Constructor Methods   *
        ***************************/
#pragma region Constructor Methods

        /// <summary>
        /// Default PixelShader Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Shader</param>
        /// <param name="name">Name of Shader</param>
        PixelShader(GraphicDevice* graphicDevice, const std::wstring& name = L"");

#pragma endregion
        
        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Returns DirectX11 Pixel Shader
        /// </summary>
        /// <returns>DirectX11 Pixel Shader</returns>
        inline ID3D11PixelShader* GetPixelShaderDX() const
        { 
            return m_PSDX;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif
