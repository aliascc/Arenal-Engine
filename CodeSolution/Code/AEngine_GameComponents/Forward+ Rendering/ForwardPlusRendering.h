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

#ifndef _FORWARD_PLUS_RENDERING_H
#define _FORWARD_PLUS_RENDERING_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AEGameComponentsDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class FPRPreZ;
class Sampler;
class GameApp;
class SimpleBuffer;
struct TimerParams;
class FPRObjectDraw;
class FPRLightCulling;
class StructuredBuffer;
class DepthStencilSurface;

/*****************
*   Class Decl   * 
******************/
class ForwardPlusRendering sealed : public GameComponent
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        LightManager& m_LightManager;

        glm::uvec2 m_NumTiles = AEMathHelpers::Vec2uZero;

        std::string m_ServiceName = "";

        FPRPreZ* m_FPRPreZ = nullptr;

        FPRLightCulling* m_FPRLightCulling = nullptr;

        FPRObjectDraw* m_FPRObjectDraw = nullptr;

        DepthStencilSurface* m_ForwardPlusDS = nullptr;

        StructuredBuffer* m_LightStructuredBuffer = nullptr;

        StructuredBuffer* m_ShadowSpotLightInfoStructuredBuffer = nullptr;

        StructuredBuffer* m_ShadowDirLightInfoStructuredBuffer = nullptr;

        SimpleBuffer* m_PerTileLightIndexBuffer = nullptr;

        Sampler* m_ShadowTextureSampler = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        AEResult InitForwardPlusDS();

        AEResult InitPerTileLightIndexBuffer();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// ForwardPlusRendering Constructor
        /// </summary>
        /// <param name="GameApp">Graphic App to be associated with this Game Component</param>
        /// <param name="gameComponentName">Game Component Name to be set</param>
        /// <param name="serviceName">Service Name to be Set for this Game Component</param>
        /// <param name="callOder">Call order of Game Component</param>
        ForwardPlusRendering(GameApp& gameApp, const std::string& gameComponentName = AE_FORWARD_PLUS_MAIN_DEF_COMPONENT_NAME, const std::string& serviceName = AE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_ForwardPlusMain);

        /// <summary>
        /// Default ForwardPlusRendering Destructor
        /// </summary>
        virtual ~ForwardPlusRendering();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline Sampler* GetShadowTextureSampler() const
        {
            return m_ShadowTextureSampler;
        }

        inline DepthStencilSurface* GetForwardPlusDS() const
        {
            return m_ForwardPlusDS;
        }

        inline StructuredBuffer* GetLightStructuredBuffer() const
        {
            return m_LightStructuredBuffer;
        }

        inline StructuredBuffer* GetShadowSpotLightInfoStructuredBuffer() const
        {
            return m_ShadowSpotLightInfoStructuredBuffer;
        }

        inline StructuredBuffer* GetShadowDirLightInfoStructuredBuffer() const
        {
            return m_ShadowDirLightInfoStructuredBuffer;
        }

        inline SimpleBuffer* GetPerTileLightIndexBuffer() const
        {
            return m_PerTileLightIndexBuffer;
        }

        inline const glm::uvec2& GetNumTiles() const
        {
            return m_NumTiles;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        void Initialize() override;

        void LoadContent() override;

        void Update(const TimerParams& timerParams) override;

        void OnResetDevice() override;

#pragma endregion

};

#endif