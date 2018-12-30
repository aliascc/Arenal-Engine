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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "DebugStats.h"
#include "Camera\Camera.h"
#include "GameApp\GameApp.h"
#include "Vertex\VertexBuffer.h"
#include "Camera\CameraUpdater.h"
#include "Wrappers\SpriteFontAE.h"
#include "Wrappers\SpriteBatchAE.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Graphic Extensions\Materials\Basic\BasicColorMaterial.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DebugStats::DebugStats(GameApp& gameApp, GameResourceManager& gameResourceManager, GraphicDevice& graphicDevice, const DebugStatsConfig& debugStatsConfig, const std::string& gameComponentName, const std::string& cameraServiceName, uint32_t callOrder)
    : DrawableGameComponent(gameApp, gameResourceManager, graphicDevice, gameComponentName, callOrder)
    , m_DebugStatsConfig(debugStatsConfig)
{
    AETODO("Add ready flag to check this");
    m_CameraUpdater = m_GameApp.GetGameService<CameraUpdater>(cameraServiceName);
    AEAssert(m_CameraUpdater != nullptr);
}

DebugStats::~DebugStats()
{
    DeleteMem(m_SpriteFontAE);
    DeleteMem(m_SpriteBatchAE);
    DeleteMem(m_GridVertexBuffer);
    DeleteMem(m_AxisVertexBuffer);
    DeleteMem(m_BasicColorMaterial);
}

void DebugStats::Initialize()
{
    m_GridVertexBuffer = new VertexBuffer<VertexPositionColor>(m_GraphicDevice);

    m_AxisVertexBuffer = new VertexBuffer<VertexPositionColor>(m_GraphicDevice);

    AETODO("Set Define for name of material");
    m_BasicColorMaterial = new BasicColorMaterial(m_GraphicDevice, m_GameResourceManager, "AE_DebugStats_BasicColorMaterial");
    
    m_SpriteBatchAE = new SpriteBatchAE(m_GraphicDevice);

    m_SpriteFontAE = new SpriteFontAE(m_GraphicDevice, m_DebugStatsConfig.m_SpriteFontFile);

    DrawableGameComponent::Initialize();
}

void DebugStats::LoadContent()
{    
    InitializeAxisAndGrid();
    
    AETODO("Need to check ret for Load Content");
    m_SpriteBatchAE->Initialize();

    m_SpriteFontAE->Initialize();

    m_BasicColorMaterial->LoadContent();    
    AETODO("Check to see if we can implement easy access to variables in BasicColorMaterial class, if so remove all includes for CB, shader props");
    m_BasicColorMaterial->GetVSProps()->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME)->SetValueT<glm::mat4>(AE_CB_WORLD_VAR_NAME, AEMathHelpers::Mat4Identity);

    DrawableGameComponent::LoadContent();
}

void DebugStats::InitializeAxisAndGrid()
{
    //Init Axis
    VertexPositionColor axis[6];
    ZeroMemory(axis, VertexPositionColor::VertexSize() * 6);
    
    //X
    axis[0].m_Position.x    = 0.0f;
    axis[0].m_Position.y    = 0.0f;
    axis[0].m_Position.z    = 0.0f;
    axis[0].m_Color         = AEColors::Red;

    axis[1].m_Position.x    = m_DebugStatsConfig.m_AxisLength;
    axis[1].m_Position.y    = 0.0f;
    axis[1].m_Position.z    = 0.0f;
    axis[1].m_Color         = AEColors::Red;

    //Y
    axis[2].m_Position.x    = 0.0f;
    axis[2].m_Position.y    = 0.0f;
    axis[2].m_Position.z    = 0.0f;
    axis[2].m_Color         = AEColors::Green;

    axis[3].m_Position.x    = 0.0f;
    axis[3].m_Position.y    = m_DebugStatsConfig.m_AxisLength;
    axis[3].m_Position.z    = 0.0f;
    axis[3].m_Color         = AEColors::Green;

    //Z
    axis[4].m_Position.x    = 0.0f;
    axis[4].m_Position.y    = 0.0f;
    axis[4].m_Position.z    = 0.0f;
    axis[4].m_Color         = AEColors::Blue;

    axis[5].m_Position.x    = 0.0f;
    axis[5].m_Position.y    = 0.0f;
    axis[5].m_Position.z    = m_DebugStatsConfig.m_AxisLength;
    axis[5].m_Color         = AEColors::Blue;
    
    m_AxisVertexBuffer->CopyVertexBuffer(axis, 6);

    //Init Grid
    uint32_t linesPerSide = (uint32_t)(m_DebugStatsConfig.m_GridSize / m_DebugStatsConfig.m_GridUnits);
    uint32_t totalLines = linesPerSide * 2;
    uint32_t vbSize = (totalLines * 2) + 4;
    float startGrid = -(m_DebugStatsConfig.m_GridUnits * linesPerSide) / 2;

    VertexPositionColor* lines = new VertexPositionColor[vbSize];
    ZeroMemory(lines, VertexPositionColor::VertexSize() * (vbSize));

    uint32_t i = 0;
    for(uint32_t lineCount = 0; lineCount <= linesPerSide; i += 2, ++lineCount)
    {
        lines[i].m_Position.x = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
        lines[i].m_Position.y = 0.0f;
        lines[i].m_Position.z = (-m_DebugStatsConfig.m_GridSize / 2.0f);
        lines[i].m_Color = (lines[i].m_Position.x != 0) ? AEColors::Gray : AEColors::Black;

        lines[i + 1].m_Position.x = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
        lines[i + 1].m_Position.y = 0.0f;
        lines[i + 1].m_Position.z = (m_DebugStatsConfig.m_GridSize / 2.0f);
        lines[i + 1].m_Color = (lines[i + 1].m_Position.x != 0) ? AEColors::Gray : AEColors::Black;
    }

    for(uint32_t lineCount = 0; lineCount <= linesPerSide; i += 2, ++lineCount)
    {
        lines[i].m_Position.x = (-m_DebugStatsConfig.m_GridSize / 2.0f);
        lines[i].m_Position.y = 0.0f;
        lines[i].m_Position.z = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
        lines[i].m_Color = (lines[i].m_Position.z != 0) ? AEColors::Gray : AEColors::Black;

        lines[i + 1].m_Position.x = (m_DebugStatsConfig.m_GridSize / 2.0f);
        lines[i + 1].m_Position.y = 0.0f;
        lines[i + 1].m_Position.z = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
        lines[i + 1].m_Color = (lines[i + 1].m_Position.z != 0) ? AEColors::Gray : AEColors::Black;
    }

    m_GridVertexBuffer->CopyVertexBuffer(lines, vbSize);

    DeleteMemArr(lines);
}

void DebugStats::Update(const TimerParams& timerParams)
{
    AETODO("move all this to post update or beginning of render");

    AETODO("Do check for this and var to know if everything is init");
    Camera* currentCamera = m_CameraUpdater->GetMainCamera();
    AEAssert(currentCamera != nullptr);

    AETODO("Check to see if we can implement easy access to variables in BasicLineMaterial & BasicColorMaterial class, if so remove all includes for CB, shader props");
    m_BasicColorMaterial->GetVSProps()->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME)->SetValueT<glm::mat4>(AE_CB_VIEW_VAR_NAME, currentCamera->GetViewMatrix());
    m_BasicColorMaterial->GetVSProps()->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME)->SetValueT<glm::mat4>(AE_CB_PROJECTION_VAR_NAME, currentCamera->GetProjectionMatrix());

    DrawableGameComponent::Update(timerParams);
}

void DebugStats::Render(const TimerParams& timerParams)
{
    glm::vec2 stride(0.0f, 0.0f);
    glm::vec2 pos(10.0f, 10.0f);
    std::string msg = "";

    m_GraphicDevice.BeginEvent("Debug Stats");

    if(m_DebugStatsConfig.m_GridEnabled || m_DebugStatsConfig.m_AxisEnabled)
    {
        m_GraphicDevice.BeginEvent("Debug Grid Axis");

        //Set Topology to LineList for Both Axis and Grid
        m_GraphicDevice.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

        //Draw Grid
        if(m_DebugStatsConfig.m_GridEnabled)
        {
            m_GraphicDevice.SetVertexBuffer(m_GridVertexBuffer);

            m_BasicColorMaterial->Apply();

            m_GraphicDevice.Draw(m_GridVertexBuffer->GetSize(), 0);
        }

        //Draw Axis
        if(m_DebugStatsConfig.m_AxisEnabled)
        {
            m_GraphicDevice.SetVertexBuffer(m_AxisVertexBuffer);

            m_BasicColorMaterial->Apply();

            m_GraphicDevice.Draw(m_AxisVertexBuffer->GetSize(), 0);
        }

        m_GraphicDevice.EndEvent();
    }

    if(m_DebugStatsConfig.m_FPSEnabled)
    {
        m_GraphicDevice.BeginEvent("Debug Text");

        m_SpriteBatchAE->Begin();

        AETODO("Change to Localization Literals");
        msg = fmt::format("FPS: {0}", m_GameApp.GetTimer().GetFPS());
        m_SpriteFontAE->DrawString(*m_SpriteBatchAE, msg, pos, m_DebugStatsConfig.m_TextColor);
        stride = m_SpriteFontAE->MeasureString(msg);
        pos.y += stride.y;

        msg = fmt::format("Milliseconds/Frame: {0}", m_GameApp.GetTimer().GetMilliPerFrame());
        m_SpriteFontAE->DrawString(*m_SpriteBatchAE, msg, pos, m_DebugStatsConfig.m_TextColor);
        stride = m_SpriteFontAE->MeasureString(msg);
        pos.y += stride.y;

        m_SpriteBatchAE->End();
    
        m_GraphicDevice.EndEvent();
    }
    
    m_GraphicDevice.EndEvent();
    
    DrawableGameComponent::Render(timerParams);
}

void DebugStats::OnLostDevice()
{    
    DrawableGameComponent::OnLostDevice();
}

void DebugStats::OnResetDevice()
{
    DrawableGameComponent::OnResetDevice();
}
