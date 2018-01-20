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

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObjectBaseInformationWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectBaseInformationWidget::GameObjectBaseInformationWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
    , m_EngineViewer(engineViewer)
{
    m_GameObjectBaseInformationWidgetQtUI.setupUi(this);

    AEAssert(m_GameObject != nullptr);

    AEAssert(m_EngineViewer != nullptr);

    InitFields();
}

GameObjectBaseInformationWidget::~GameObjectBaseInformationWidget()
{
}

void GameObjectBaseInformationWidget::InitFields()
{
    AEAssert(m_GameObject != nullptr);
    AEAssert(m_EngineViewer != nullptr);
    if (m_GameObject == nullptr || m_EngineViewer == nullptr)
    {
        return;
    }

    QString id = QString::number(m_GameObject->GetUniqueID());
    m_GameObjectBaseInformationWidgetQtUI.m_GameObjectIDTextBox->setText(id);

    m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->setText(QString::fromStdWString(m_GameObject->GetName()));
}

void GameObjectBaseInformationWidget::on_m_GameObjectNameTextBox_editingFinished()
{
    AEAssert(m_GameObject != nullptr);

    if(m_GameObject == nullptr)
    {
        return;
    }

    if(m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->text().isEmpty())
    {
        m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->setText(QString::fromStdWString(m_GameObject->GetName()));

        return;
    }

    QString newName = m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->text();

    std::wstring newNameStd = newName.toStdWString();
    
    m_GameObject->SetName(newNameStd);

    emit (NameChanged(m_GameObject->GetUniqueID()));
}
