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
#include "CoreCommands.h"
#include "ImGuiManager.h"
#include "GraphicDevice.h"
#include "GameApp/GameApp.h"
#include "GameUtils/GameComponentCollection.h"

//Always include last
#include "Memory\MemLeaks.h"

/**********************************
*   ResizeCommand Function Defs   *
***********************************/

ResizeCommand::ResizeCommand(GameApp& gameApp, const glm::ivec2& newSize)
    : m_GameApp(gameApp)
    , m_NewSize(newSize)
{
}

ResizeCommand::~ResizeCommand()
{
}

void ResizeCommand::Execute()
{
    m_GameApp.Resize(m_NewSize.x, m_NewSize.y);
}

/**********************************
*   GCSortCommand Function Defs   *
***********************************/

GCSortCommand::GCSortCommand(GameComponentCollection& gameComponentCollection)
    : m_GameComponentCollection(gameComponentCollection)
{
}

GCSortCommand::~GCSortCommand()
{
}

void GCSortCommand::Execute()
{
    m_GameComponentCollection.SortContainer();
}

#ifdef AE_EDITOR_MODE

/*******************************
*   Resize Editor Class Decl   *
********************************/

ResizeEditorCommand::ResizeEditorCommand(GameApp& gameApp, const glm::ivec2& newSize)
    : m_GameApp(gameApp)
    , m_NewSize(newSize)
{
}

ResizeEditorCommand::~ResizeEditorCommand()
{
}

void ResizeEditorCommand::Execute()
{
    m_GameApp.ResizeEditor(m_NewSize.x, m_NewSize.y);
}

#endif