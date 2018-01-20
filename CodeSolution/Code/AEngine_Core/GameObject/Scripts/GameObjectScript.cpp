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
#include "GameContentDefs.h"
#include "GameObjectScript.h"
#include "Base\BaseFunctions.h"
#include "AngelScript\AngelScriptDefs.h"
#include "AngelScript\AngelScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectScript::GameObjectScript(const std::wstring& resourceName, AngelScriptManager* angelScriptManager)
    : GameResource(resourceName, GameResourceType::Animation)
    , m_AngelScriptManager(angelScriptManager)
{
    AEAssert(angelScriptManager);
}

GameObjectScript::~GameObjectScript()
{
    CleanUp();
}

void GameObjectScript::CleanUp()
{
    if (m_GameObjectScriptFunctions != nullptr)
    {
        DeleteMem(m_GameObjectScriptFunctions);

        m_AngelScriptManager->RemoveModule(m_Name);
    }
}

AEResult GameObjectScript::Load()
{
    /////////////////////////////////////////
    //Pre-checks
    if (m_AngelScriptManager == nullptr)
    {
        AETODO("Add error for AS is null");
        return AEResult::NullObj;
    }

    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    AEAssert(!m_FileName.empty());
    if (m_FileName.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEAssert(!m_Name.empty());
    if (m_Name.empty())
    {
        return AEResult::EmptyName;
    }

    /////////////////////////////////////////
    //Remove Memory from old script
    CleanUp();

    /////////////////////////////////////////
    //Variables
    int retInt = 0;
    AEResult ret = AEResult::Ok;
    asIScriptModule* asModule = nullptr;

    /////////////////////////////////////////
    //Load scripts
    ret = m_AngelScriptManager->LoadScript(m_FileName, m_Name, &asModule);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    if (asModule->GetObjectTypeCount() == 0)
    {
        m_AngelScriptManager->RemoveModule(m_Name);

        AETODO("set correct error code");
        return AEResult::Fail;
    }

    /////////////////////////////////////////
    //Verify that Game Object Script Interface appears
    asITypeInfo* objType = asModule->GetObjectTypeByIndex(0);

    bool interfaceFound = false;
    uint32_t interfaceCount = objType->GetInterfaceCount();
    for (uint32_t i = 0; i < interfaceCount; i++)
    {
        if (strcmp(objType->GetInterface(i)->GetName(), AE_GOS_INTERFACE_NAME) == 0)
        {
            interfaceFound = true;
            i = interfaceCount;
        }
    }

    if (!interfaceFound)
    {
        m_AngelScriptManager->RemoveModule(m_Name);

        AETODO("set correct error code");
        AETODO("log error");
        return AEResult::Fail;
    }

    /////////////////////////////////////////
    //Get Interface Functions
    m_GameObjectScriptFunctions = new GameObjectScriptFunctions();

    m_GameObjectScriptFunctions->m_Constructor       = objType->GetFactoryByIndex(0);
    m_GameObjectScriptFunctions->m_Initialize        = objType->GetMethodByDecl(AE_GOS_INIT_FUNC);
    m_GameObjectScriptFunctions->m_ConstantUpdate    = objType->GetMethodByDecl(AE_GOS_CONSTANT_UPDATE_FUNC);
    m_GameObjectScriptFunctions->m_Update            = objType->GetMethodByDecl(AE_GOS_UPDATE_FUNC);
    m_GameObjectScriptFunctions->m_PostUpdate        = objType->GetMethodByDecl(AE_GOS_POST_UPDATE_FUNC);

    m_GameObjectScriptFunctions->m_Constructor->AddRef();
    m_GameObjectScriptFunctions->m_Initialize->AddRef();
    m_GameObjectScriptFunctions->m_ConstantUpdate->AddRef();
    m_GameObjectScriptFunctions->m_Update->AddRef();
    m_GameObjectScriptFunctions->m_PostUpdate->AddRef();

    return AEResult::Ok;
}
