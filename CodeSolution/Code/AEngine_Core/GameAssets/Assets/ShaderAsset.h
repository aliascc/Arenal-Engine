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

#ifndef _SHADER_ASSET_H
#define _SHADER_ASSET_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameAssets\GameAsset.h"

/********************
*   Forward Decls   *
*********************/
class Shader;

/*****************
*   Class Decl   *
******************/
class ShaderAsset sealed : public GameAsset
{    
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        Shader* m_Shader = nullptr;

        ShaderType m_ShaderType = ShaderType::VertexShader;

        /// <summary>
        /// Graphic Device to create Asset Resources in video memory
        /// </summary>
        GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods
        
        /// <summary>
        /// Loads an Asset Resource to Memory
        /// </summary>
        /// <returns>AEResult::OK if successful</returns>
        AEResult LoadAssetResource() override;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// ShaderAsset Constructor
        /// </summary>
        /// <param name="filePath">File Path of Game Asset to Load</param>
        /// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
        /// <param name="shaderType">Type of the Shader that this Asset will hold</param>
        /// <param name="graphicDevice">Graphic Device to create Asset Resources in video memory</param>
        ShaderAsset(const std::string& filePath, GameResourceManager* gameResourceManager, ShaderType shaderType, GraphicDevice* graphicDevice);

        /// <summary>
        /// Default MaterialAsset Destructor
        /// </summary>
        virtual ~ShaderAsset();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline ShaderType GetShaderType() const
        {
            return m_ShaderType;
        }

        Shader* GetShaderReference();

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif
