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

#ifndef _VARIANCE_SHADOW_MATERIAL_H
#define _VARIANCE_SHADOW_MATERIAL_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Graphic Extensions\Materials\Material.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class GameResourceManager;

/*****************
*   Class Decl   *
******************/
class VarianceShadowMaterial sealed : public Material
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

        AEResult CreateVertexShader();

        AEResult CreateVertexShaderConstantBuffer();

        AEResult CreatePixelShader();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// VarianceShadowMaterial Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with</param>
        /// <param name="gameResourceManager">Game Resource Manager to get the manage resources</param>
        /// <param name="name">Name of the Effect</param>
        VarianceShadowMaterial(GraphicDevice& graphicDevice, GameResourceManager& gameResourceManager, const std::string& name = AE_VARIANCE_SHADOW_MAT_NAME);

        /// <summary>
        /// Default VarianceShadowMaterial Destructor
        /// </summary>
        virtual ~VarianceShadowMaterial();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

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

        AEResult LoadContent() override;

#pragma endregion

};

#endif
