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
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "angelscript.h"
#include "GameComponent.h"
#include "DrawableGameComponent.h"
#include "GameComponentCollection.h"
#include "Core Game Command/CoreCommands.h"
#include "AngelScript\AngelScriptManager.h"

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

void GameComponentCollection::FillGCBasicProps(const GameComponent& gc, GCBasicProps& basicProps) const
{
    basicProps.m_GCName          = gc.GetName();
    basicProps.m_GCID            = gc.GetUniqueID();
    basicProps.m_Enabled         = gc.GetEnable();
    basicProps.m_GCCallOrder     = gc.GetCallOrder();
    
    if(gc.IsDrawableGameComponent())
    {
        const DrawableGameComponent& dgc    = reinterpret_cast<const DrawableGameComponent&>(gc);
        basicProps.m_Drawable               = true;
        basicProps.m_Visibled               = dgc.GetVisible();
    }
}

AETODO("Change to return success and save info in reference");
AETODO("Change method to const");
GCBasicProps GameComponentCollection::GetGCBasicProps(uint32_t index)
{
    GCBasicProps basicProps;

    if(index >= m_Container.size())
    {
        return basicProps;
    }

    const GameComponent* gc   = m_Container[index];

    FillGCBasicProps(*gc, basicProps);

    return basicProps;
}

AETODO("Change to return success and save info in reference");
AETODO("Change to unsigned 64bit integer");
AETODO("Change method to const");
GCBasicProps GameComponentCollection::GetGCBasicPropsID(uint32_t id)
{
    GCBasicProps basicProps;

    if(m_Map.find(id) == m_Map.end())
    {
        return basicProps;
    }

    const GameComponent* gc = m_Map[id];

    FillGCBasicProps(*gc, basicProps);

    return basicProps;
}

bool GameComponentCollection::ValidIndex(uint32_t index) const
{
    if(index >= m_Container.size())
    {
        return false;
    }

    return true;
}

void GameComponentCollection::NeedSortChange()
{
    GCSortCommand* sortCommand = new GCSortCommand(*this);
    GameCommandManager::GetInstance().AddCommand(sortCommand);
}

void GameComponentCollection::SortContainer()
{
    std::sort(m_Container.begin(), m_Container.end(),
              [](GameComponent* left, GameComponent* right)
                {
                    return (left->GetCallOrder() < right->GetCallOrder());
                });
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
        std::string errMsg = fmt::format(AELOCMAN.GetLiteral("GC_ID_DUPLICATE_ERR_MSG"), __FUNCTION__, gc->GetUniqueID());
        AELOGGER.AddNewLog(LogLevel::Warning, errMsg);

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
    auto it = m_Container.begin();
    for (; (*it)->GetUniqueID() != gc->GetUniqueID() && it != m_Container.end(); it++);
    AEAssert(it != m_Container.end());
    m_Container.erase(it);

    //Remove from Map
    m_Map.erase(gc->GetUniqueID());

    return AEResult::Ok;
}

void GameComponentCollection::InitializeCollection()
{
    size_t size = m_Container.size();
    for(size_t i = 0; i < size; i++)
    {        
        m_Container[i]->Initialize();
    }
}

void GameComponentCollection::LoadContentCollection()
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {
        m_Container[i]->LoadContent();
    }
}

void GameComponentCollection::UnLoadContentCollection()
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {
        m_Container[i]->UnLoadContent();
    }
}

void GameComponentCollection::ConstantUpdateCollection(const TimerParams& timerParams)
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {    
        if(m_Container[i]->GetEnable())
        {
            m_Container[i]->ConstantUpdate(timerParams);
        }
    }
}

void GameComponentCollection::UpdateCollection(const TimerParams& timerParams)
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {    
        if(m_Container[i]->GetEnable())
        {
            m_Container[i]->Update(timerParams);
        }
    }
}

void GameComponentCollection::PostUpdateCollection(const TimerParams& timerParams)
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {    
        if(m_Container[i]->GetEnable())
        {
            m_Container[i]->PostUpdate(timerParams);
        }
    }
}

void GameComponentCollection::RenderCollection(const TimerParams& timerParams)
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {    
        if(m_Container[i]->IsDrawableGameComponent())
        {
            DrawableGameComponent* dgc = reinterpret_cast<DrawableGameComponent*>(m_Container[i]);

            if(dgc->GetVisible())
            {
                dgc->Render(timerParams);
            }
        }
    }
}
        
void GameComponentCollection::OnLostDeviceCollection()
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {    
        m_Container[i]->OnLostDevice();
    }
}

void GameComponentCollection::OnResetDeviceCollection()
{
    size_t size = m_Container.size();
    for (size_t i = 0; i < size; i++)
    {
        m_Container[i]->OnResetDevice();
    }
}
