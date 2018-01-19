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

#ifndef _DEBUG_STATS_H
#define _DEBUG_STATS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\Color.h"
#include "Math\AEMathDefs.h"
#include "Color\AEColorDefs.h"
#include "AEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"
#include "Vertex\Types\VertexPositionColor.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameApp;
struct TimerParams;
class SpriteFontAE;
class CameraUpdater;
class SpriteBatchAE;
class BasicColorMaterial;

template<class T>
class VertexBuffer;

/**************
*   Structs   *
***************/
struct DebugStatsConfig
{
    std::wstring m_SpriteFontFile = L"";
    Color m_TextColor = AEColors::White;
    bool m_FPSEnabled = true;
    bool m_AxisEnabled = true;
    float m_AxisLength = 1.0f;
    bool m_GridEnabled = true;
    Color m_GridColor = AEColors::Black;
    float m_GridSize = 10.0f;
    float m_GridUnits = 1.0f;

    DebugStatsConfig()
    {
    }
};

/*****************
*   Class Decl   *
******************/
class DebugStats : public DrawableGameComponent
{
    protected:
        //Variables
        DebugStatsConfig m_DebugStatsConfig; // No need to initialize
        SpriteBatchAE* m_SpriteBatchAE = nullptr;
        SpriteFontAE* m_SpriteFontAE = nullptr;
        VertexBuffer<VertexPositionColor>* m_GridVertexBuffer = nullptr;
        VertexBuffer<VertexPositionColor>* m_AxisVertexBuffer = nullptr;
        BasicColorMaterial* m_BasicColorMaterial = nullptr;
        CameraUpdater* m_CameraUpdater = nullptr;
        glm::mat4 m_WVP = AEMathHelpers::Mat4Identity;

        void InitializeAxisAndGrid();

    public:
        //Constructor Destructor.
        DebugStats(GameApp* gameApp, const DebugStatsConfig& debugStatsConfig, const std::wstring& gameComponentName = AE_DEBUG_STATS_DEF_COMPONENT_NAME, const std::wstring& cameraServiceName = AE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_DebugStats);
        virtual ~DebugStats();

        //Gets
        
        //Framework Methods
        inline bool GetGridEnabled() const
        {
            return m_DebugStatsConfig.m_GridEnabled;
        }
        
        inline void SetGridEnabled(bool enable)
        {
            m_DebugStatsConfig.m_GridEnabled = enable;
        }

        inline bool GetAxisEnabled()
        {
            return m_DebugStatsConfig.m_AxisEnabled;
        }
        
        inline void SetAxisEnabled(bool enable)
        {
            m_DebugStatsConfig.m_AxisEnabled = enable;
        }

        inline bool GetFPSEnabled()
        {
            return m_DebugStatsConfig.m_FPSEnabled;
        }
        
        inline void SetFPSEnabled(bool enable)
        {
            m_DebugStatsConfig.m_FPSEnabled = enable;
        }

        //Drawable Game Component Override methods
        void Initialize() override;
        void LoadContent() override;
        void Update(const TimerParams& timerParams) override;
        void Render(const TimerParams& timerParams) override;
        void OnLostDevice() override;
        void OnResetDevice() override;
};

#endif
