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

#pragma once

#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <DirectXMath.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"
#include "Base\Base.h"

/************
*   Using   *
*************/
using namespace DirectX;

/********************
*   Forward Decls   *
*********************/
class ParticleSystem;
struct TimerParams;

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Helper for objects that want to leave particles behind them as they
/// move around the world. This emitter implementation solves two related
/// problems:
/// 
/// If an object wants to create particles very slowly, less than once per
/// frame, it can be a pain to keep track of which updates ought to create
/// a new particle versus which should not.
/// 
/// If an object is moving quickly and is creating many particles per frame,
/// it will look ugly if these particles are all bunched up together. Much
/// better if they can be spread out along a line between where the object
/// is now and where it was on the previous frame. This is particularly
/// important for leaving trails behind fast moving objects such as rockets.
/// 
/// This emitter class keeps track of a moving object, remembering its
/// previous position so it can calculate the velocity of the object. It
/// works out the perfect locations for creating particles at any frequency
/// you specify, regardless of whether this is faster or slower than the
/// game update rate.
/// </summary>
class ParticleEmitter : public AEObject
{
	protected:
		ParticleSystem*				m_ParticleSystem;
		float						m_TimeBetweenParticles;
        XMFLOAT3					m_PreviousPosition;
        float						m_TimeLeftOver;

	public:
		//Constructor & Destructor
		ParticleEmitter(ParticleSystem* particleSystem, float particlesPerSecond, const XMFLOAT3& initialPosition);
		virtual ~ParticleEmitter();

		//Framework Methods
		void						Update						(const TimerParams& timerParams, const XMFLOAT3& newPosition);

};

#endif