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

/*************************
*   3rd Party Includes   *
**************************/
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameObjectAudioListenerComponentWidget.h"
#include "GameObject\Components\AudioListenerGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectAudioListenerComponentWidget::GameObjectAudioListenerComponentWidget(GameObject* gameObject, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
{
    m_GameObjectAudioListenerComponentWidgetQtUI.setupUi(this);

    AEAssert(m_GameObject != nullptr);
}

GameObjectAudioListenerComponentWidget::~GameObjectAudioListenerComponentWidget()
{
}

void GameObjectAudioListenerComponentWidget::InitFields()
{
    AEAssert(m_GameObject != nullptr);
    if (m_GameObject == nullptr)
    {
        return;
    }

    AEAssert(m_GameObject->HasAudioListenerGOC());
    if (!m_GameObject->HasAudioListenerGOC())
    {
        return;
    }

    m_IsReady = true;
}
