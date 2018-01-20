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
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObjectAddComponentDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectAddComponentDialog::GameObjectAddComponentDialog(QWidget *parent)
    : QDialog(parent)
{
    m_GameObjectAddComponentDialogQtUI.setupUi(this);

    AEQTHelpers::SetComboBoxGameObjectComponentTypeOption(m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox);
    m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox->setCurrentIndex(0);
}

GameObjectAddComponentDialog::~GameObjectAddComponentDialog()
{
}

GameObjectComponentTypeOption GameObjectAddComponentDialog::GetGameObjectComponentTypeOption() const
{
    int currentIndex = m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox->currentIndex();

    AEAssert(currentIndex != -1);
    if(currentIndex == -1)
    {
        return GameObjectComponentTypeOption::Mesh;
    }

    GameObjectComponentTypeOption gameObjectComponentTypeOption = static_cast<GameObjectComponentTypeOption>(m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox->itemData(currentIndex).toUInt());
    
    return gameObjectComponentTypeOption;
}
