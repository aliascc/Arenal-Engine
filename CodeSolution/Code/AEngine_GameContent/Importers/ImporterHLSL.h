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

#ifndef _IMPORTER_HLSL_H
#define _IMPORTER_HLSL_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shaders\ShaderDefs.h"
#include "IncludeInterfaceHLSL.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct ShaderContent;

struct ID3D11ShaderReflection;

/*****************
*   Class Decl   *
******************/
class ImporterHLSL sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX HLSL Include interface to find files needed to be included by Shader
        /// </summary>
        IncludeInterfaceHLSL m_IncludeInterfaceHLSL;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        ShaderVariableClass GetShaderVariableClass(D3D_SHADER_VARIABLE_CLASS dxClass);

        ShaderVariableType GetShaderVariableType(D3D_SHADER_VARIABLE_TYPE  dxType);

        AEResult BuildShaderTarget(ShaderType shaderType, ShaderModel shaderModel, std::string& target);

        AEResult ExtractShaderProperties(ShaderContent* content, ID3DBlob* shaderBlob);

        AEResult ExtractShaderConstantBuffers(ShaderContent* content, ID3D11ShaderReflection* shaderReflector);

        AEResult ExtractBoundResources(ShaderContent* content, ID3D11ShaderReflection* shaderReflector);

        AEResult ExtractShaderSimpleBuffers(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

        AEResult ExtractShaderStructuredBuffers(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

        AEResult ExtractShaderTextureInputs(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

        AEResult ExtractShaderSamplers(ShaderContent* content, const D3D11_SHADER_INPUT_BIND_DESC& descBoundResources);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// ImporterHLSL Constructor
        /// </summary>
        ImporterHLSL();

        /// <summary>
        /// Default ImporterHLSL Destructor
        /// </summary>
        virtual ~ImporterHLSL();

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult ImportShader(const std::string& filename, ShaderType shaderType, ShaderModel shaderModel, ShaderContent** shaderContent);

#pragma endregion

};

#endif
