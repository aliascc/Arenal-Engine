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

#ifndef _HULLSHADER_H
#define _HULLSHADER_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shader.h"

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
class HullShader sealed : public Shader
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables
                
        ///<summary>
        ///DirectX11 Hull Shader
        ///</summary>
        ID3D11HullShader* m_HSDX = nullptr;

#pragma endregion

        /************************
         *   Private Methods    *
         ************************/
#pragma region Private Methods

        AEResult LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length) override;

#pragma endregion

        /**********************************
         *   Private Destructor Methods   *
         **********************************/
#pragma region Private Destructor Methods

        ///<summary>
        ///Default HullShader Destructor
        ///</summary>
        virtual ~HullShader();

#pragma endregion

    public:

        /***************************
         *   Constructor Methods   *
         ***************************/
#pragma region Constructor Methods

        ///<summary>
        ///Default HullShader Constructor
        ///</summary>
        ///<param name="graphicDevice">Graphic Device to be associated with this Shader</param>
        ///<param name="name">Name of Shader</param>
        HullShader(GraphicDevice& graphicDevice, const std::string& name = "");

#pragma endregion
        
        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        ///<summary>
        ///Returns DirectX11 Hull Shader
        ///</summary>
        ///<returns>DirectX11 Hull Shader</returns>
        inline ID3D11HullShader* GetHullShaderDX() const
        { 
            return m_HSDX;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif
