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
#include "ImGuiObject.h"
#include "ImGuiManager.h"
#include "GraphicDevice.h"

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
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

AEResult ImGuiManager::AddImGuiObject(ImGuiObject* imGuiObject)
{
    if (imGuiObject == nullptr)
    {
        return AEResult::NullParameter;
    }

    size_t imGuiObjectIndex = 0;
    const uint64_t imGuiObjectID = imGuiObject->GetUniqueID();

    if (GetImGuiObjectIndex(imGuiObjectID, imGuiObjectIndex) == AEResult::Ok)
    {
        return AEResult::ObjExists;
    }

    m_ImGuiObjectIndex[imGuiObjectID] = m_ImGuiObjects.size();
    m_ImGuiObjects.push_back(imGuiObject);

    return AEResult::Ok;
}

AEResult ImGuiManager::RemoveImGuiObject(const uint64_t imGuiObjectID, bool freeMemory)
{
    size_t imGuiObjectIndex = 0;

    if (GetImGuiObjectIndex(imGuiObjectID, imGuiObjectIndex) != AEResult::Ok)
    {
        return AEResult::NotFound;
    }

    //Reorganize the indexes of the windows
    m_ImGuiObjectIndex.erase(imGuiObjectID);
    for (auto& idx : m_ImGuiObjectIndex)
    {
        if (idx.second >= imGuiObjectIndex)
        {
            --idx.second;
        }
    }

    //Remove from the vector
    auto itWhere = m_ImGuiObjects.begin() + imGuiObjectIndex;
    m_ImGuiObjects.erase(itWhere);

    return AEResult::Ok;
}

AEResult ImGuiManager::Initialize()
{
    ImGui::CreateContext();

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

    return AEResult::Ok;
}

void ImGuiManager::Update(const TimerParams& timerParams)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    size_t size = m_ImGuiObjects.size();
    for (size_t i = 0; i < size; i++)
    {
        m_ImGuiObjects[i]->Update(timerParams);
    }

    ImGui::EndFrame();
}

void ImGuiManager::Render(const TimerParams& timerParams)
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

#endif //AE_EDITOR_MODE