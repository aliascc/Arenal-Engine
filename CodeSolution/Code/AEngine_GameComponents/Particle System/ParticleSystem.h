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

#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <DirectXMath.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"
#include "ParticleSystemDefs.h"
#include "Particle System\Vertex\VertexParticle.h"
#include "Vertex\VertexBuffer.h"

/************
*   Using   *
*************/
using namespace DirectX;

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameApp;
class ParticleSystemMaterial;
class IndexBuffer;
class Camera;
class Texture2D;

/*****************
*   Class Decl   *
******************/
class ParticleSystem : public DrawableGameComponent
{
	protected:
		
		/***************************
		*	Protected Variables	   *
		****************************/
#pragma region Protected Variables

		//Camera Service Instance to poll View & Projection Matrices as well as Aspect Ratio
		Camera*							m_Camera;

		//Camera Service Name to Aquire instance from Game Services
		const std::string				m_CameraSeviceName;
		
		//Texture use for Particles
		Texture2D*						m_ParticeTexture;

        // Custom effect for drawing particles. This computes the particle
        // animation entirely in the vertex shader: no per-particle CPU work required!
		ParticleSystemMaterial*			m_ParticleMaterial;

        // Settings class controls the appearance and animation of this particle system.
		ParticleSystemSettings			m_Settings;

        // An array of particles, treated as a circular queue.
        VertexParticle*					m_Particles;

        // A vertex buffer holding our particles. This contains the same data as
        // the particles array, but copied across to where the GPU can access it.
		VertexBuffer<VertexParticle>*	m_VertexBuffer;

        // Index buffer turns sets of four vertices into particle quads (pairs of triangles).
		IndexBuffer*					m_IndexBuffer;

        // The particles array and vertex buffer are treated as a circular queue.
        // Initially, the entire contents of the array are free, because no particles
        // are in use. When a new particle is created, this is allocated from the
        // beginning of the array. If more than one particle is created, these will
        // always be stored in a consecutive block of array elements. Because all
        // particles last for the same amount of time, old particles will always be
        // removed in order from the start of this active particle region, so the
        // active and free regions will never be intermingled. Because the queue is
        // circular, there can be times when the active particle region wraps from the
        // end of the array back to the start. The queue uses modulo arithmetic to
        // handle these cases. For instance with a four entry queue we could have:
        //
        //      0
        //      1 - first active particle
        //      2 
        //      3 - first free particle
        //
        // In this case, particles 1 and 2 are active, while 3 and 4 are free.
        // Using modulo arithmetic we could also have:
        //
        //      0
        //      1 - first free particle
        //      2 
        //      3 - first active particle
        //
        // Here, 3 and 0 are active, while 1 and 2 are free.
        //
        // But wait! The full story is even more complex.
        //
        // When we create a new particle, we add them to our managed particles array.
        // We also need to copy this new data into the GPU vertex buffer, but we don't
        // want to do that straight away, because setting new data into a vertex buffer
        // can be an expensive operation. If we are going to be adding several particles
        // in a single frame, it is faster to initially just store them in our managed
        // array, and then later upload them all to the GPU in one single call. So our
        // queue also needs a region for storing new particles that have been added to
        // the managed array but not yet uploaded to the vertex buffer.
        //
        // Another issue occurs when old particles are retired. The CPU and GPU run
        // asynchronously, so the GPU will often still be busy drawing the previous
        // frame while the CPU is working on the next frame. This can cause a
        // synchronization problem if an old particle is retired, and then immediately
        // overwritten by a new one, because the CPU might try to change the contents
        // of the vertex buffer while the GPU is still busy drawing the old data from
        // it. Normally the graphics driver will take care of this by waiting until
        // the GPU has finished drawing inside the VertexBuffer.SetData call, but we
        // don't want to waste time waiting around every time we try to add a new
        // particle! To avoid this delay, we can specify the SetDataOptions.NoOverwrite
        // flag when we write to the vertex buffer. This basically means "I promise I
        // will never try to overwrite any data that the GPU might still be using, so
        // you can just go ahead and update the buffer straight away". To keep this
        // promise, we must avoid reusing vertices immediately after they are drawn.
        //
        // So in total, our queue contains four different regions:
        //
        // Vertices between firstActiveParticle and firstNewParticle are actively
        // being drawn, and exist in both the managed particles array and the GPU
        // vertex buffer.
        //
        // Vertices between firstNewParticle and firstFreeParticle are newly created,
        // and exist only in the managed particles array. These need to be uploaded
        // to the GPU at the start of the next draw call.
        //
        // Vertices between firstFreeParticle and firstRetiredParticle are free and
        // waiting to be allocated.
        //
        // Vertices between firstRetiredParticle and firstActiveParticle are no longer
        // being drawn, but were drawn recently enough that the GPU could still be
        // using them. These need to be kept around for a few more frames before they
        // can be reallocated.
        uint32_t						m_FirstActiveParticle;
        uint32_t						m_FirstNewParticle;
        uint32_t						m_FirstFreeParticle;
        uint32_t						m_FirstRetiredParticle;

        // Store the current time, in seconds.
        float							m_CurrentTime;
		
        // Count how many times Draw has been called. This is used to know
        // when it is safe to retire old particles back into the free list.
        uint32_t						m_DrawCounter;

#pragma endregion

		/***********************************
		*	Protected Framework Methods	   *
		************************************/
#pragma region Protected Framework Methods

        /// <summary>
        /// Helper for checking when active particles have reached the end of
        /// their life. It moves old particles from the active area of the queue
        /// to the retired section.
        /// </summary>
        void							RetireActiveParticles			();

		/// <summary>
        /// Helper for checking when retired particles have been kept around long
        /// enough that we can be sure the GPU is no longer using them. It moves
        /// old particles from the retired area of the queue to the free section.
        /// </summary>
        void							FreeRetiredParticles			();

        /// <summary>
        /// Helper for uploading new particles from our managed
        /// array to the GPU vertex buffer.
        /// </summary>
        void							AddNewParticlesToVertexBuffer	();
		
        /// <summary>
        /// Helper for initializing the particle effect params.
        /// </summary>
        void							InitParamsParticleEffect		();

#pragma endregion

	public:
		//Contructor & Destructor
		ParticleSystem(GameApp* gameApp, const ParticleSystemSettings& settings, const std::string& cameraServiceName = "Camera", const std::string& gameComponentName = "Particle System", uint32_t callOrder = 100);
		virtual ~ParticleSystem();
		
		/***************************
		*	Framework Methods	   *
		****************************/
#pragma region Framework Methods
		
		/// <summary>
        /// Adds a new particle to the system.
        /// </summary>
		/// <param name='position'>Position of the new Particle added to the system</param>
		/// <param name='velocity'>Velocity of the new Particle added to the system</param>
		/// <returns>Return AEResult::Ok if particle was added</returns>
		AEResult						AddParticle						(const XMFLOAT3& position, const XMFLOAT3& velocity);

#pragma endregion
		
		
		/***************************************************
		*	Drawable Game Component Override Methods	   *
		****************************************************/
#pragma region Drawable Game Component Override Methods

		void							Initialize						();
		void							LoadContent						();
		void							Update							(const TimerParams& timerParams);
		void							Render							(const TimerParams& timerParams);
		void							OnLostDevice					();
		void							OnResetDevice					();

#pragma endregion

};

#endif