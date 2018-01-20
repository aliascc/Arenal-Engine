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

/**********************
*   System Includes   *
***********************/
#include <assert.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "angelscript.h"
#include "Logger\Logger.h"
#include "GameComponent.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "DrawableGameComponent.h"
#include "GameComponentCollection.h"
#include "AngelScript\AngelScriptManager.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

AETODO("Add mutex");
/********************
*   Function Defs   *
*********************/
GameComponentCollection::GameComponentCollection()
{
}

GameComponentCollection::~GameComponentCollection()
{
}

uint32_t GameComponentCollection::GetSize() const
{ 
    return (uint32_t)m_Container.size(); 
}

AEResult GameComponentCollection::SetGCDrawCallOrder(uint32_t id, uint32_t drawCallOrder)
{
    if(m_Map.find(id) == m_Map.end())
    {
        return AEResult::NotFound;
    }

    m_Map[id]->SetCallOrder(drawCallOrder);

    return AEResult::Ok;
}

AEResult GameComponentCollection::SetGCVisible(uint32_t id, bool visible)
{
    if(m_Map.find(id) == m_Map.end())
    {
        return AEResult::NotFound;
    }

    if(!m_Map[id]->IsDrawableGameComponent())
    {
        return AEResult::Fail;
    }
    
    DrawableGameComponent* dgc = static_cast<DrawableGameComponent*>(m_Map[id]);

    dgc->SetVisible(visible);

    return AEResult::Ok;
}

AEResult GameComponentCollection::SetGCEnable(uint32_t id, bool enable)
{
    if(m_Map.find(id) == m_Map.end())
    {
        return AEResult::NotFound;
    }

    m_Map[id]->SetEnable(enable);

    return AEResult::Ok;
}

GCBasicProps GameComponentCollection::GetGCBasicProps(uint32_t index)
{
    GCBasicProps basic;

    if(index >= m_Container.size())
    {
        return basic;
    }

    auto it = m_Container.begin();

    for(uint32_t i = 0; i < index; ++i, ++it);

    GameComponent* gc = (*it);
    
    basic.m_GCName = gc->GetName();
    basic.m_GCID = gc->GetUniqueID();
    basic.m_Enabled = gc->GetEnable();
    basic.m_GCCallOrder = gc->GetCallOrder();
    
    if(gc->IsDrawableGameComponent())
    {
        DrawableGameComponent* dgc = static_cast<DrawableGameComponent*>(gc);
        basic.m_Drawable = true;
        basic.m_Visibled = dgc->GetVisible();
    }

    return basic;
}

GCBasicProps GameComponentCollection::GetGCBasicPropsID(uint32_t id)
{
    GCBasicProps basic;
    
    if(m_Map.find(id) == m_Map.end())
    {
        return basic;
    }

    GameComponent* gc = (m_Map[id]);
    
    basic.m_GCName = gc->GetName();
    basic.m_GCID = gc->GetUniqueID();
    basic.m_Enabled = gc->GetEnable();
    basic.m_GCCallOrder = gc->GetCallOrder();
    
    if(gc->IsDrawableGameComponent())
    {
        DrawableGameComponent* dgc = static_cast<DrawableGameComponent*>(gc);
        basic.m_Drawable = true;
        basic.m_Visibled = dgc->GetVisible();
    }

    return basic;
}

bool GameComponentCollection::ValidIndex(uint32_t index) const
{
    if(index >= m_Container.size())
    {
        return false;
    }

    return true;
}

bool GameComponentCollection::SortGCCol::operator()(GameComponent* left, GameComponent* right) const
{
    return (left->GetCallOrder() < right->GetCallOrder());
}

void GameComponentCollection::SortContainer()
{
    m_Container.sort(m_SortGCContainer);
}

bool GameComponentCollection::DoesGCExist(GameComponent* gc) const
{
    if(gc == nullptr)
    {
        return false;
    }

    return DoesIDExist(gc->GetUniqueID());
}

bool GameComponentCollection::DoesIDExist(uint64_t id) const
{
    return (m_Map.find(id) != m_Map.end());
}

AEResult GameComponentCollection::Add(GameComponent* gc)
{
    AEAssert(gc != nullptr);
    if(gc == nullptr)
    {
        return AEResult::NullParameter;
    }

    if(DoesGCExist(gc))
    {
        if(AELOGGER != nullptr && AELOCMAN != nullptr)
        {
            std::wstring errMsg = fmt::format(AELOCMAN->GetLiteral(L"GC_ID_DUPLICATE_ERR_MSG"), __FUNCTIONW__, gc->GetUniqueID());
            AELOGGER->AddNewLog(LogLevel::Warning, errMsg);
        }

        return AEResult::ObjExists;
    }

    //Reference m_NeedSortChangeCallback to GC Class
    gc->m_NeedSortChangeCallback = std::bind(&GameComponentCollection::NeedSortChange, this);

    //Add to Container
    m_Container.push_back(gc);

    //Sort the container to include the new added GC in correct order
    SortContainer();

    //Add to map
    m_Map[gc->GetUniqueID()] = gc;

    return AEResult::Ok;
}

AEResult GameComponentCollection::Remove(GameComponent* gc)
{
    AEAssert(gc != nullptr);
    if(gc == nullptr)
    {
        return AEResult::NullParameter;
    }

    if(!DoesGCExist(gc))
    {
        return AEResult::NotFound;
    }

    //Remove Callback for sorting
    gc->m_NeedSortChangeCallback = nullptr;

    //Dereference GC Class from Container
    m_Container.remove(gc);

    //Remove from Map
    m_Map.erase(gc->GetUniqueID());

    return AEResult::Ok;
}

void GameComponentCollection::InitializeCollection()
{
    for(auto gc : m_Container)
    {        
        gc->Initialize();
    }
}

void GameComponentCollection::LoadContentCollection()
{
    for(auto gc : m_Container)
    {    
        gc->LoadContent();
    }
}

void GameComponentCollection::UnLoadContentCollection()
{
    for(auto gc : m_Container)
    {    
        gc->UnLoadContent();
    }
}

void GameComponentCollection::ConstantUpdateCollection(const TimerParams& timerParams)
{
    //First check if we need to sort the container
    if(m_NeedSort)
    {
        SortContainer();
    }

    for(auto gc : m_Container)
    {    
        if(gc->GetEnable())
        {
            gc->ConstantUpdate(timerParams);
        }
    }
}

void GameComponentCollection::UpdateCollection(const TimerParams& timerParams)
{
    //First check if we need to sort the container
    if(m_NeedSort)
    {
        SortContainer();
    }

    for(auto gc : m_Container)
    {    
        if(gc->GetEnable())
        {
            gc->Update(timerParams);
        }
    }
}

void GameComponentCollection::PostUpdateCollection(const TimerParams& timerParams)
{
    for(auto gc : m_Container)
    {    
        if(gc->GetEnable())
        {
            gc->PostUpdate(timerParams);
        }
    }
}

void GameComponentCollection::RenderCollection(const TimerParams& timerParams)
{
    for(auto gc : m_Container)
    {    
        if(gc->IsDrawableGameComponent())
        {
            DrawableGameComponent* dgc = static_cast<DrawableGameComponent*>(gc);

            if(dgc->GetVisible())
            {
                dgc->Render(timerParams);
            }
        }
    }
}
        
void GameComponentCollection::OnLostDeviceCollection()
{
    for(auto gc : m_Container)
    {    
        gc->OnLostDevice();
    }
}

void GameComponentCollection::OnResetDeviceCollection()
{
    for(auto gc : m_Container)
    {    
        gc->OnResetDevice();
    }
}
