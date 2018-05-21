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

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioPlayer.h"
#include "AudioAddOnAS.h"
#include "GameASAddOnsDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/*******************
*   AudioAddOnAS   *
********************/
AudioAddOnAS::AudioAddOnAS()
    : AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Audio)
{
}

AudioAddOnAS::~AudioAddOnAS()
{
}

AEResult AudioAddOnAS::Register(asIScriptEngine* engine)
{
    AEAssert(engine != nullptr);
    if (engine == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (RegisterAudioPlayerObject(engine) != AEResult::Ok)
    {
        AETODO("Add Log and return");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_BASIC_TIME_ERR_MSG"), __FUNCTION__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult AudioAddOnAS::RegisterAudioPlayerObject(asIScriptEngine* engine)
{
    int ret = 0;

    //////////////////////////
    //Register Object
    ret = engine->RegisterObjectType("AudioPlayer", 0, asOBJ_REF | asOBJ_NOCOUNT);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    //////////////////////////
    //Register Methods
    ret = engine->RegisterObjectMethod("AudioPlayer", "AEResult Play()", asMETHOD(AudioPlayer, Play), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("AudioPlayer", "AEResult Pause()", asMETHOD(AudioPlayer, Pause), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("AudioPlayer", "AEResult Stop()", asMETHOD(AudioPlayer, Stop), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    //////////////////////////
    //Finish
    return AEResult::Ok;
}
