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
#include "precomp_imgui.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ImGuiMenu.h"
#include "ImGuiObject.h"
#include "ImGuiWindow.h"
#include "ImGuiManager.h"
#include "GraphicDevice.h"
#include "ImGuiMainMenu.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Framework Defs   *
**********************/

#ifdef AE_EDITOR_MODE

ImGuiManager::ImGuiManager(GraphicDevice& graphicDevice)
    : m_GraphicDevice(graphicDevice)
{
}

ImGuiManager::~ImGuiManager()
{
    CleanUp();
}

void ImGuiManager::CleanUp()
{
    m_ImGuiWindowMap.clear();
    m_ImGuiWindows.clear();

    DeleteMem(m_ImGuiMainMenu);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::SortWindows()
{
    AEAssert(m_IsReady);

    std::sort(m_ImGuiWindows.begin(), m_ImGuiWindows.end(),
              [](ImGuiWindow* left, ImGuiWindow* right)
                {
                    return (left->GetRenderPriority() < right->GetRenderPriority());
                });
}

AEResult ImGuiManager::AddImGuiWindow(ImGuiWindow* imGuiWindow)
{
    AEAssert(m_IsReady);

    if (imGuiWindow == nullptr)
    {
        return AEResult::NullParameter;
    }

    const uint64_t imGuiWindowID    = imGuiWindow->GetUniqueID();

    m_ImGuiWindowMap[imGuiWindowID]                 = imGuiWindow;
    m_ImGuiWindowNameMap[imGuiWindow->GetName()]    = imGuiWindowID;
    m_ImGuiWindows.push_back(imGuiWindow);

    return AEResult::Ok;
}

AEResult ImGuiManager::RemoveImGuiWindow(uint64_t imGuiWindowID)
{
    AEAssert(m_IsReady);

    //Reorganize the indexes of the windows
    m_ImGuiWindowMap.erase(imGuiWindowID);

    //Remove from the vector
    auto it = m_ImGuiWindows.begin();
    for (; (*it)->GetUniqueID() != imGuiWindowID && it != m_ImGuiWindows.end(); it++);
    AEAssert(it != m_ImGuiWindows.end());

    m_ImGuiWindowNameMap.erase((*it)->GetName());
    m_ImGuiWindows.erase(it);

    return AEResult::Ok;
}

ImGuiWindow* ImGuiManager::GetImGuiWindow(const std::string& windowName)
{
    auto it = m_ImGuiWindowNameMap.find(windowName);
    if (it == m_ImGuiWindowNameMap.end())
    {
        return nullptr;
    }

    return m_ImGuiWindowMap[it->second];
}

AEResult ImGuiManager::Initialize()
{
    if (m_IsReady)
    {
        return AEResult::Ok;
    }

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    HWND windowHandle                       = m_GraphicDevice.GetGraphicPP().m_DeviceWindow;
    ID3D11Device* d3dDevice                 = m_GraphicDevice.GetDeviceDX();
    ID3D11DeviceContext* d3dDeviceContext   = m_GraphicDevice.GetDeviceContextDX();

    if (!ImGui_ImplWin32_Init(windowHandle))
    {
        return AEResult::Fail;
    }

    if (!ImGui_ImplDX11_Init(d3dDevice, d3dDeviceContext))
    {
        return AEResult::Fail;
    }

    //////////////////////////////////
    // Init ImGui Docking Variables

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    m_DockingWindowFlags =  ImGuiWindowFlags_MenuBar                | ImGuiWindowFlags_NoDocking    |
                            ImGuiWindowFlags_NoTitleBar             | ImGuiWindowFlags_NoCollapse   |
                            ImGuiWindowFlags_NoResize               | ImGuiWindowFlags_NoMove       |
                            ImGuiWindowFlags_NoBringToFrontOnFocus  | ImGuiWindowFlags_NoNavFocus   |
                            ImGuiWindowFlags_NoBackground;

    // When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    m_DockingFlags = ImGuiDockNodeFlags_PassthruDockspace;

    //////////////////////////////////
    // Creatte Main Menu
    m_ImGuiMainMenu = new ImGuiMainMenu(*this);

    m_IsReady = true;

    return AEResult::Ok;
}

void ImGuiManager::UpdateDockingSpace()
{
    AEAssert(m_IsReady);

    ImGuiIO& io = ImGui::GetIO();
    AEAssert(io.ConfigFlags & ImGuiConfigFlags_DockingEnable);

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin(AE_DOCKING_SPACE_WINDOW_NAME, nullptr, m_DockingWindowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceID = ImGui::GetID(AE_DOCKING_SPACE_NAME);
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), m_DockingFlags);

    ImGui::End();
}

void ImGuiManager::Update(const TimerParams& timerParams)
{
    AEAssert(m_IsReady);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    UpdateDockingSpace();

    if (m_ImGuiMainMenu->IsVisible())
    {
        m_ImGuiMainMenu->Update(timerParams);
    }

    size_t size = m_ImGuiWindows.size();
    for (size_t i = 0; i < size; i++)
    {
        ImGuiWindow* window = m_ImGuiWindows[i];

        if (window->IsVisible())
        {
            window->Update(timerParams);
        }
    }

    ImGui::EndFrame();
}

void ImGuiManager::Render(const TimerParams& timerParams)
{
    AEAssert(m_IsReady);

    ///////////////////////////
    // Render the ImGui Data
    ImGui::Render();

    ///////////////////////////
    // Set Editor Render Target and clear
    m_GraphicDevice.SetEditorRenderTargetAndViewPort();
    m_GraphicDevice.Clear(true, 0, false, false, AEColors::EditorBackground);

    ///////////////////////////
    // Render to DirectX
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ///////////////////////////
    // Update and Render additional Platform Windows
    // Enclose in if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    // but we will always have view ports on
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    ///////////////////////////
    // Reset the Render Target to the Game
    m_GraphicDevice.ResetRenderTargetAndSetDepthStencil();
    m_GraphicDevice.ResetViewport();
}

void ImGuiManager::OnLostDevice()
{
    AEAssert(m_IsReady);

    ImGui_ImplDX11_InvalidateDeviceObjects();
}

void ImGuiManager::OnResetDevice()
{
    AEAssert(m_IsReady);

    ImGui_ImplDX11_CreateDeviceObjects();
}

#endif //AE_EDITOR_MODE