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

#ifndef _DEPTH_STENCIL_SURFACE_H
#define _DEPTH_STENCIL_SURFACE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ITexture2D.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class DepthStencilSurface sealed : public ITexture2D
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX Depth Stencil
        /// </summary>
        ID3D11DepthStencilView* m_DepthStencilDX = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Cleans up memory use by the class
        /// </summary>
        void CleanUp();

#pragma endregion

        /*********************************
        *   Private Destructor Methods   *
        **********************************/
#pragma region Private Destructor Methods

        /// <summary>
        /// Default Depth Stencil Destructor
        /// </summary>
        virtual ~DepthStencilSurface();

#pragma endregion

    public:

        /**************************
        *   Constructor Methods   *
        ***************************/
#pragma region Constructor Methods

        /// <summary>
        /// Default Render Target Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Depth Stencil</param>
        /// <param name="depthStencilName">Name of Depth Stencil</param>
        DepthStencilSurface(GraphicDevice* graphicDevice, const std::string& depthStencilName);

#pragma endregion

        /*******************
         *   Get Methods   *
         *******************/
#pragma region Get Methods

        /// <summary>
        /// DirectX 11 Depth Stencil View use to bind the Depth Stencil to the Graphic Device
        /// </summary>
        /// <returns>Returns DirectX 11 Depth Stencil View</returns>
        inline ID3D11DepthStencilView* GetDXDepthStencilView() const
        {
            return m_DepthStencilDX;
        }

#pragma endregion

        /***********************************
         *   Override Framework Methods    *
         ***********************************/
#pragma region Override Framework Methods

        /// <summary>
        /// Invalid Option for Depth Stencil Surface
        /// </summary>
        /// <returns>AEResult::Fail as method is not use</returns>
        AEResult Load() override;

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Reinitializes the Depth Stencil
        /// </summary>
        /// <param name="width">Width in pixels of the Depth Stencil</param>
        /// <param name="height">Height in pixels of the Depth Stencil</param>
        /// <param name="formatDST">Pixel format of the Depth Stencil Texture</param>
        /// <param name="formatDSV">Pixel format of the Depth Stencil View</param>
        /// <param name="formatDSSRV">Pixel format of the Depth Stencil Shader Resource View</param>
        /// <returns>AEResult::Ok if Depth Stencil creation succeeded</returns>
        AEResult InitializeDepthStencilSurface(uint32_t width, uint32_t height, DXGI_FORMAT formatDST, DXGI_FORMAT formatDSV, DXGI_FORMAT formatDSSRV);

#pragma endregion

};

#endif
