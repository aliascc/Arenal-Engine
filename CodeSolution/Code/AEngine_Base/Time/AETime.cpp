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
#include "precomp_base.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AETime.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETime::AETime(bool sampleFPS)
    : m_IsSamplingFPS(sampleFPS)
{
}

AETime::~AETime()
{
}

void AETime::UpdateAllTimers()
{
    UpdateConstantUpdateTimer();
    UpdateUpdateTimer();
    UpdatePostUpdateTimer();
    UpdateRenderTimer();
    UpdateFrameTimer();
}

void AETime::UpdateConstantUpdateTimer()
{
    m_ConstantUpdateTimer.Update();
}

void AETime::PostUpdateConstantUpdateTimer()
{
    m_ConstantUpdateTimer.PostUpdate();
}

void AETime::UpdateUpdateTimer()
{
    m_UpdateTimer.Update();
}

void AETime::PostUpdateUpdateTimer()
{
    m_UpdateTimer.PostUpdate();
}

void AETime::UpdatePostUpdateTimer()
{
    m_PostUpdateTimer.Update();
}

void AETime::PostUpdatePostUpdateTimer()
{
    m_PostUpdateTimer.PostUpdate();
}

void AETime::UpdateRenderTimer()
{
    m_RenderTimer.Update();
}

void AETime::PostUpdateRenderTimer()
{
    m_RenderTimer.PostUpdate();
}

void AETime::UpdateFrameTimer()
{
    m_FrameTimer.Update();

    if(m_IsSamplingFPS)
    {
        SampleFPS();
    }
}

void AETime::SampleFPS()
{
    m_FrameCount++;

    double diff = m_FrameTimer.GetTimerParams().m_TotalElapsedTime - m_LastFPSTime;

    if( diff >= 1.0 )
    {
        m_FPS =  m_FrameCount/(float)diff;
        m_FrameCount = 0;
        m_LastFPSTime = m_FrameTimer.GetTimerParams().m_TotalElapsedTime;

        m_MilliPerFrame = 1000.0f / m_FPS;
    }
}
