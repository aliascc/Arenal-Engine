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

#ifndef _RENDER_TARGET_H
#define _RENDER_TARGET_H

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
#include "GraphicsDefs.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class RenderTarget sealed : public ITexture2D
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX Render Target
        /// </summary>
        ID3D11RenderTargetView* m_RenderTargetDX = nullptr;

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
        /// Default Render Target Destructor
        /// </summary>
        virtual ~RenderTarget();

#pragma endregion

    public:

        /**************************
        *   Constructor Methods   *
        ***************************/
#pragma region Constructor Methods

        /// <summary>
        /// Default Render Target Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Render Target</param>
        /// <param name="renderTargetName">Name of the Render Target</param>
        RenderTarget(GraphicDevice* graphicDevice, const std::string& renderTargetName);

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// DirectX 11 Render Target View use to bind the Render Target to the Graphic Device
        /// </summary>
        /// <returns>Returns DirectX 11 Render Target View</returns>
        inline ID3D11RenderTargetView* GetDXRenderTargetView() const
        {
            return m_RenderTargetDX;
        }

#pragma endregion

        /***********************************
         *   Override Framework Methods    *
         ***********************************/
#pragma region Override Framework Methods

        /// <summary>
        /// Invalid Option for Render Target
        /// </summary>
        /// <returns>AEResult::Fail as method is not use</returns>
        AEResult Load() override;

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Reinitialize Render Target
        /// </summary>
        /// <param name="width">Width in pixels of the Render Target</param>
        /// <param name="height">Height in pixels of the Render Target</param>
        /// <param name="formatRenderTarget">Pixel format of the Render Target</param>
        /// <param name="graphicBufferUsage">Graphic Buffer Usage</param>
        /// <param name="graphicBufferAccess">Graphic Buffer Access</param>
        /// <returns>AEResult::Ok if Render Target creation succeeded</returns>
        AEResult InitializeRenderTarget(uint32_t width, uint32_t height, DXGI_FORMAT format, GraphicBufferUsage graphicBufferUsage = GraphicBufferUsage::Default, GraphicBufferAccess graphicBufferAccess = GraphicBufferAccess::None);

#pragma endregion

};

#endif
