/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <assert.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "angelscript.h"
#include "Logger\Logger.h"
#include "GameComponent.h"
#include "Time\XETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "DrawableGameComponent.h"
#include "GameComponentCollection.h"
#include "AngelScript\AngelScriptManager.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

XETODO("Add mutex");
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

XEResult GameComponentCollection::SetGCDrawCallOrder(uint32_t id, uint32_t drawCallOrder)
{
	if(m_Map.find(id) == m_Map.end())
	{
		return XEResult::NotFound;
	}

	m_Map[id]->SetCallOrder(drawCallOrder);

	return XEResult::Ok;
}

XEResult GameComponentCollection::SetGCVisible(uint32_t id, bool visible)
{
	if(m_Map.find(id) == m_Map.end())
	{
		return XEResult::NotFound;
	}

	if(!m_Map[id]->IsDrawableGameComponent())
	{
		return XEResult::Fail;
	}
	
	DrawableGameComponent* dgc = static_cast<DrawableGameComponent*>(m_Map[id]);

	dgc->SetVisible(visible);

	return XEResult::Ok;
}

XEResult GameComponentCollection::SetGCEnable(uint32_t id, bool enable)
{
	if(m_Map.find(id) == m_Map.end())
	{
		return XEResult::NotFound;
	}

	m_Map[id]->SetEnable(enable);

	return XEResult::Ok;
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

XEResult GameComponentCollection::Add(GameComponent* gc)
{
	XEAssert(gc != nullptr);
	if(gc == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(DoesGCExist(gc))
	{
		if(XELOGGER != nullptr && XELOCMAN != nullptr)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"GC_ID_DUPLICATE_ERR_MSG"), __FUNCTIONW__, gc->GetUniqueID());
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		return XEResult::ObjExists;
	}

	//Reference m_NeedSortChangeCallback to GC Class
	gc->m_NeedSortChangeCallback = std::bind(&GameComponentCollection::NeedSortChange, this);

	//Add to Container
	m_Container.push_back(gc);

	//Sort the container to include the new added GC in correct order
	SortContainer();

	//Add to map
	m_Map[gc->GetUniqueID()] = gc;

	return XEResult::Ok;
}

XEResult GameComponentCollection::Remove(GameComponent* gc)
{
	XEAssert(gc != nullptr);
	if(gc == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(!DoesGCExist(gc))
	{
		return XEResult::NotFound;
	}

	//Remove Callback for sorting
	gc->m_NeedSortChangeCallback = nullptr;

	//Dereference GC Class from Container
	m_Container.remove(gc);

	//Remove from Map
	m_Map.erase(gc->GetUniqueID());

	return XEResult::Ok;
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
