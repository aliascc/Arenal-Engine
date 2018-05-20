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
AETime::AETime(bool sampleFPS, uint32_t numSamples)
    : m_IsSamplingFPS(sampleFPS)
    , m_NumOfSamples(numSamples)
{
    m_FrameSamples.resize(m_NumOfSamples);
    for (uint32_t i = 0; i < m_NumOfSamples; i++)
    {
        m_FrameSamples[i] = 0.0;
    }
}

AETime::~AETime()
{
}

void AETime::Update()
{
    m_FrameTimer.Update();

    if(m_IsSamplingFPS)
    {
        SampleFPS();
    }
}

void AETime::SampleFPS()
{
    m_SampleFramePos++;

    if (m_SampleFramePos >= m_NumOfSamples)
    {
        m_SampleFramePos = 0;
    }

    double diff = m_FrameTimer.GetTimerParams().m_ElapsedTimePression;
    m_FrameSamples[m_SampleFramePos] = diff;

    double totalTime = 0.;
    for (uint32_t i = 0; i < m_NumOfSamples; i++)
    {
        totalTime += m_FrameSamples[i];
    }

    m_MilliPerFrame = totalTime / m_NumOfSamples;
    m_FPS = 1. / m_MilliPerFrame;
}

bool AETime::NeedToRunConstantUpdate()
{
    const TimerParams& frameTimer = m_FrameTimer.GetTimerParams();

    double diff = frameTimer.m_TotalElapsedTimePression - m_ConstantUpdateTimerParams.m_TotalElapsedTimePression;

    if (diff >= m_ConstantUpdateStep)
    {
        m_ConstantUpdateTimerParams.m_ElapsedTimePression       = m_ConstantUpdateStep;
        m_ConstantUpdateTimerParams.m_ElapsedTime               = (float)m_ConstantUpdateTimerParams.m_ElapsedTimePression;

        m_ConstantUpdateTimerParams.m_TotalElapsedTimePression  += m_ConstantUpdateTimerParams.m_ElapsedTime;
        m_ConstantUpdateTimerParams.m_TotalElapsedTime          = (float)m_ConstantUpdateTimerParams.m_TotalElapsedTimePression;

        return true;
    }

    return false;
}
