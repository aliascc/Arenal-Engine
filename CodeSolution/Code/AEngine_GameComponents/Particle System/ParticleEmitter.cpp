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
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Utils\AETimeDefs.h"
#include "Math\AEMathDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ParticleEmitter::ParticleEmitter(ParticleSystem* particleSystem, float particlesPerSecond, const XMFLOAT3& initialPosition)
	: m_ParticleSystem(particleSystem)
	, m_TimeBetweenParticles(1.0f / particlesPerSecond)
	, m_PreviousPosition(initialPosition)
	, m_TimeLeftOver(0.0f)
{
	assert(particleSystem);
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Update(const TimerParams& timerParams, const XMFLOAT3& newPosition)
{
	// Work out how much time has passed since the previous update.
	float elapsedTime = timerParams.m_ElapsedTime;

	if (elapsedTime > 0 && m_ParticleSystem != nullptr)
	{
        // Work out how fast we are moving. (NewPos - PrevPos) / elapsedTime
		XMFLOAT3 velocity = XMFLOAT3ZERO;
		XEMathHelperVector::SubtrackVector3(newPosition, m_PreviousPosition, velocity);
		XEMathHelperVector::DivideVector3(velocity, elapsedTime, velocity);

        // If we had any time left over that we didn't use during the
        // previous update, add that to the current elapsed time.
		float timeToSpend = m_TimeLeftOver + elapsedTime;
                
        // Counter for looping over the time interval.
		float currentTime = -m_TimeLeftOver;

        // Create particles as long as we have a big enough time interval.
		while (timeToSpend > m_TimeBetweenParticles)
        {
            currentTime += m_TimeBetweenParticles;
            timeToSpend -= m_TimeBetweenParticles;

            // Work out the optimal position for this particle. This will produce
            // evenly spaced particles regardless of the object speed, particle
            // creation frequency, or game update rate.
            float mu = currentTime / elapsedTime;

            XMFLOAT3 position = XMFLOAT3ZERO;
			XEMathHelperVector::LerpVector3(m_PreviousPosition, newPosition, mu, position);

            // Create the particle.
			m_ParticleSystem->AddParticle(position, velocity);
        }

        // Store any time we didn't use, so it can be part of the next update.
		m_TimeLeftOver = timeToSpend;
    }

    m_PreviousPosition = newPosition;
}