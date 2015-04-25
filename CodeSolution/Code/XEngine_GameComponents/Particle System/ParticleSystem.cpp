/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "ParticleSystem.h"
#include "Utils\XETimeDefs.h"
#include "Particle System\Materials\ParticleSystemMaterial.h"
#include "GameApp\GameApp.h"
#include "Math\XEMathDefs.h"
#include "Vertex\IndexBuffer.h"
#include "Utils\XERandomHelpers.h"
#include "Camera\Camera.h"
#include "Textures\Texture2D.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ParticleSystem::ParticleSystem(GameApp* gameApp, const ParticleSystemSettings& settings, const std::string& cameraServiceName, const std::string& gameComponentName, uint32_t callOrder)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
	, m_ParticleMaterial(nullptr)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_FirstActiveParticle(0)
	, m_FirstNewParticle(0)
	, m_FirstFreeParticle(0)
	, m_FirstRetiredParticle(0)
	, m_Particles(nullptr)
	, m_Settings(settings)
	, m_CurrentTime(0.0f)
	, m_DrawCounter(0)
	, m_Camera(nullptr)
	, m_CameraSeviceName(cameraServiceName)
	, m_ParticeTexture(nullptr)
{
}

ParticleSystem::~ParticleSystem()
{
	DeleteMem(m_ParticleMaterial);
	DeleteMem(m_VertexBuffer);
	DeleteMem(m_IndexBuffer);
	DeleteMemArr(m_Particles);
	DeleteMem(m_ParticeTexture);

	//Erase Vertex Particle VD from Memory
	VertexParticle::DeleteVertexDeclaration();
}

void ParticleSystem::Initialize()
{
	//Cr5eate a new Particle Material Instance
	m_ParticleMaterial = new ParticleSystemMaterial();
	
	//Create Index Buffer Instance
	m_IndexBuffer = IndexBuffer::CreateIndexBufferInst(m_GraphicDevice);

	//Create Texture Instance
	m_ParticeTexture = Texture2D::CreateTexture2DInst();
	
	//Create the Vertex Buffer Instance, set usage as XE_USAGE_DYNAMIC to make the Vertex Buffer a Dynamic Buffer and 
	//lock to XE_LOCK_DISCARD to increase performance when writing to the VRAM
	m_VertexBuffer = VertexBuffer<VertexParticle>::CreateVertexBufferInst(m_GraphicDevice, XE_POOL_DEFAULT, XE_USAGE_WRITEONLY | XE_USAGE_DYNAMIC);

	// Allocate the particle array, and fill in the corner fields (which never change).
	m_Particles = new VertexParticle[m_Settings.m_MaxParticles * 4];
	memset(m_Particles, 0, VertexParticle::VertexSize() * m_Settings.m_MaxParticles * 4);

	//Init Corner Settings in the particles
	for (uint32_t i = 0; i < m_Settings.m_MaxParticles; ++i)
	{
		m_Particles[i * 4 + 0].m_Corner_X = -1;
		m_Particles[i * 4 + 0].m_Corner_Y = -1;
		
		m_Particles[i * 4 + 1].m_Corner_X = 1;
		m_Particles[i * 4 + 1].m_Corner_Y = -1;
		
		m_Particles[i * 4 + 2].m_Corner_X = 1;
		m_Particles[i * 4 + 2].m_Corner_Y = 1;
		
		m_Particles[i * 4 + 3].m_Corner_X = -1;
		m_Particles[i * 4 + 3].m_Corner_Y = 1;
	}

	m_Camera = static_cast<Camera*>(m_GameApp->GetGameService(m_CameraSeviceName));

	DrawableGameComponent::Initialize();
}

void ParticleSystem::InitParamsParticleEffect()
{
    // Set the values of parameters that do not change.
	m_ParticleMaterial->SetDuration(m_Settings.m_Duration);
	m_ParticleMaterial->SetDurationRandomness(m_Settings.m_DurationRandomness);
	m_ParticleMaterial->SetGravity(m_Settings.m_Gravity);
	m_ParticleMaterial->SetEndVelocity(m_Settings.m_EndVelocity);
    m_ParticleMaterial->SetMinColor(m_Settings.m_MinColor);
    m_ParticleMaterial->SetMaxColor(m_Settings.m_MaxColor);
	m_ParticleMaterial->SetRotateSpeed(XMFLOAT2(m_Settings.m_MinRotateSpeed, m_Settings.m_MaxRotateSpeed));
    m_ParticleMaterial->SetStartSize(XMFLOAT2(m_Settings.m_MinStartSize, m_Settings.m_MaxStartSize));
	m_ParticleMaterial->SetEndSize(XMFLOAT2(m_Settings.m_MinEndSize, m_Settings.m_MaxEndSize));
	m_ParticleMaterial->SetParticleTexture(m_ParticeTexture);
}

void ParticleSystem::LoadContent()
{
	//Initialize Material 
	m_ParticleMaterial->LoadMaterial(m_GraphicDevice);

	//Load Particle Texture
	m_ParticeTexture->CreateFromFile(m_GraphicDevice, "Particle System Texture", m_Settings.m_TextureName);

	//Initialize Particle Effect Parameters
	InitParamsParticleEffect();

	//Load Vertex Buffer
	m_VertexBuffer->CopyVertexBuffer(m_Particles, m_Settings.m_MaxParticles * 4);

	//Load Index Buffer
	uint16_t* indices = new uint16_t[m_Settings.m_MaxParticles * 6];
	memset(indices, 0, sizeof(uint16_t) * m_Settings.m_MaxParticles * 6);

    for (uint16_t i = 0; i < m_Settings.m_MaxParticles; ++i)
    {
        indices[i * 6 + 0] = (i * 4 + 0);
        indices[i * 6 + 1] = (i * 4 + 1);
        indices[i * 6 + 2] = (i * 4 + 2);

        indices[i * 6 + 3] = (i * 4 + 0);
        indices[i * 6 + 4] = (i * 4 + 2);
        indices[i * 6 + 5] = (i * 4 + 3);
    }

	m_IndexBuffer->CopyToIndexBuffer(indices, m_Settings.m_MaxParticles * 6);

	DeleteMemArr(indices);
	
	DrawableGameComponent::LoadContent();
}

void ParticleSystem::Update(const TimerParams& timerParams)
{
	m_CurrentTime += (float)timerParams.m_ElapsedTime;

	RetireActiveParticles();
	FreeRetiredParticles();

	// If we let our timer go on increasing for ever, it would eventually
	// run out of floating point precision, at which point the particles
	// would render incorrectly. An easy way to prevent this is to notice
	// that the time value doesn't matter when no particles are being drawn,
	// so we can reset it back to zero any time the active queue is empty.
	if (m_FirstActiveParticle == m_FirstFreeParticle)
	{
		m_CurrentTime = 0.0f;
	}

	if (m_FirstRetiredParticle == m_FirstActiveParticle)
	{
		m_DrawCounter = 0;
	}

	DrawableGameComponent::Update(timerParams);
}

void ParticleSystem::Render(const TimerParams& timerParams)
{
	// If there are any particles waiting in the newly added queue,
	// we'd better upload them to the GPU ready for drawing.
	if (m_FirstNewParticle != m_FirstFreeParticle)
	{
		AddNewParticlesToVertexBuffer();
	}

	// If there are any active particles, draw them now!
	if (m_FirstActiveParticle != m_FirstFreeParticle)
	{
		//Set Blend State
		m_GraphicDevice->SetBlendState(m_Settings.m_BlendState);

		//Set ZBuffer Write to false, so we will only read from the depth buffer but not write to it
		m_GraphicDevice->SetDepthStencilState(GraphicDepthStencilState::DepthReadState);

		// Set an effect parameter describing the viewport size. This is
		// needed to convert particle sizes into screen space point sizes.
		XMFLOAT2 viewportScale(0.5f / m_Camera->GetAspectRatio(), -0.5);
		m_ParticleMaterial->SetViewportScale(viewportScale);

		// Set an effect parameter describing the current time. All the vertex
		// shader particle animation is keyed off this value.
		m_ParticleMaterial->SetCurrentTime(m_CurrentTime);

		// Set View & Projection Matrices
		m_ParticleMaterial->SetProjection(m_Camera->GetProjectionMatrix());
		m_ParticleMaterial->SetView(m_Camera->GetViewMatrix());

		// Set the particle vertex and index buffer.
		m_GraphicDevice->SetVertexBuffer(m_VertexBuffer);
		m_GraphicDevice->SetIndexBuffer(m_IndexBuffer);

		// Activate the particle effect.
		uint32_t passes = 0;
		m_ParticleMaterial->Begin(passes);

		for(uint32_t i = 0; i < passes; ++i)
		{
			//Begin Effect Pass
			m_ParticleMaterial->BeginPass(i);

			if (m_FirstActiveParticle < m_FirstFreeParticle)
			{
				// If the active particles are all in one consecutive range,
				// we can draw them all in a single call.
				m_GraphicDevice->DrawIndexedPrimitive(XE_PT_TRIANGLELIST, 0, 
														m_FirstActiveParticle * 4, (m_FirstFreeParticle - m_FirstActiveParticle) * 4,
														m_FirstActiveParticle * 6, (m_FirstFreeParticle - m_FirstActiveParticle) * 2);
			}
			else
			{
				// If the active particle range wraps past the end of the queue
				// back to the start, we must split them over two draw calls.
				m_GraphicDevice->DrawIndexedPrimitive(XE_PT_TRIANGLELIST, 0,
														m_FirstActiveParticle * 4, (m_Settings.m_MaxParticles - m_FirstActiveParticle) * 4,
														m_FirstActiveParticle * 6, (m_Settings.m_MaxParticles - m_FirstActiveParticle) * 2);

				if (m_FirstFreeParticle > 0)
				{
					m_GraphicDevice->DrawIndexedPrimitive(XE_PT_TRIANGLELIST, 0,
															0, m_FirstFreeParticle * 4,
															0, m_FirstFreeParticle * 2);
				}
			}

			//End Effect Pass
			m_ParticleMaterial->EndPass();
		}

		//End Effect
		m_ParticleMaterial->End();

		// Reset some of the renderstates that we changed,
		// so as not to mess up any other subsequent drawing.
		m_GraphicDevice->SetDepthStencilState(GraphicDepthStencilState::DefaultState);

		//Set BlendState Back to the default
		m_GraphicDevice->SetBlendState(GraphicBlendState::DefaultState);
	}

	m_DrawCounter++;

	DrawableGameComponent::Render(timerParams);
}

void ParticleSystem::OnLostDevice()
{
	m_ParticleMaterial->OnLostDevice();
	m_IndexBuffer->OnLostDevice();
	m_VertexBuffer->OnLostDevice();

	DrawableGameComponent::OnLostDevice();
}

void ParticleSystem::OnResetDevice()
{
	m_ParticleMaterial->OnResetDevice();
	m_IndexBuffer->OnResetDevice();
	m_VertexBuffer->OnResetDevice();

	DrawableGameComponent::OnResetDevice();
}

void ParticleSystem::RetireActiveParticles()
{
    float particleDuration = m_Settings.m_Duration;

    while (m_FirstActiveParticle != m_FirstNewParticle)
    {
        // Is this particle old enough to retire?
        // We multiply the active particle index by four, because each
        // particle consists of a quad that is made up of four vertices.
		float particleAge = m_CurrentTime - m_Particles[m_FirstActiveParticle * 4].m_Time;

        if (particleAge < particleDuration)
		{
            break;
		}

        // Remember the time at which we retired this particle.
        m_Particles[m_FirstActiveParticle * 4].m_Time = (float)m_DrawCounter;

        // Move the particle from the active to the retired queue.
        m_FirstActiveParticle++;

        if (m_FirstActiveParticle >= m_Settings.m_MaxParticles)
		{
            m_FirstActiveParticle = 0;
		}
    }
}

void ParticleSystem::FreeRetiredParticles()
{
    while (m_FirstRetiredParticle != m_FirstActiveParticle)
    {
        // Has this particle been unused long enough that
        // the GPU is sure to be finished with it?
        // We multiply the retired particle index by four, because each
        // particle consists of a quad that is made up of four vertices.
        int32_t age = m_DrawCounter - (int32_t)m_Particles[m_FirstRetiredParticle * 4].m_Time;

        // The GPU is never supposed to get more than 2 frames behind the CPU.
        // We add 1 to that, just to be safe in case of buggy drivers that
        // might bend the rules and let the GPU get further behind.
        if (age < 3)
		{
            break;
		}

        // Move the particle from the retired to the free queue.
        m_FirstRetiredParticle++;

        if(m_FirstRetiredParticle >= m_Settings.m_MaxParticles)
		{
            m_FirstRetiredParticle = 0;
		}
    }
}

XEResult ParticleSystem::AddParticle(const XMFLOAT3& position, const XMFLOAT3& velocity)
{
	// Figure out where in the circular queue to allocate the new particle.
	uint32_t nextFreeParticle = m_FirstFreeParticle + 1;

	if (nextFreeParticle >= m_Settings.m_MaxParticles)
	{
		nextFreeParticle = 0;
	}

	// If there are no free particles, we just have to give up.
	if (nextFreeParticle == m_FirstRetiredParticle)
	{
		return XEResult::Fail;
	}

	// Adjust the input velocity based on how much
	// this particle system wants to be affected by it.
	XMFLOAT3 newVelocity = XMFLOAT3ZERO;
	XEMathHelperVector::MultiplyVector3(velocity, m_Settings.m_EmitterVelocitySensitivity, newVelocity);

	// Add in some random amount of horizontal velocity.
	float horizontalVelocity = XE_Base::Lerp<float>(m_Settings.m_MinHorizontalVelocity, m_Settings.m_MaxHorizontalVelocity, XERandomHelpers::GetNextFloat());

	float horizontalAngle = XERandomHelpers::GetNextFloat() * XM_2PI;

	newVelocity.x += horizontalVelocity * XMScalarCos(horizontalAngle);
	newVelocity.z += horizontalVelocity * XMScalarSin(horizontalAngle);

	// Add in some random amount of vertical velocity.
	newVelocity.y += XE_Base::Lerp<float>(m_Settings.m_MinVerticalVelocity, m_Settings.m_MaxVerticalVelocity, XERandomHelpers::GetNextFloat());

	// Choose four random control values. These will be used by the vertex
	// shader to give each particle a different size, rotation, and color.
	XMCOLOR randomValues(
				XERandomHelpers::GetNextFloat(),
				XERandomHelpers::GetNextFloat(),
				XERandomHelpers::GetNextFloat(),
				XERandomHelpers::GetNextFloat()
				);

	// Fill in the particle vertex structure.
	for (uint32_t i = 0; i < 4; ++i)
	{
		//Set position
		m_Particles[m_FirstFreeParticle * 4 + i].m_x = position.x;
		m_Particles[m_FirstFreeParticle * 4 + i].m_y = position.y;
		m_Particles[m_FirstFreeParticle * 4 + i].m_z = position.z;

		//Set Velocity
		m_Particles[m_FirstFreeParticle * 4 + i].m_v_x = newVelocity.x;
		m_Particles[m_FirstFreeParticle * 4 + i].m_v_y = newVelocity.y;
		m_Particles[m_FirstFreeParticle * 4 + i].m_v_z = newVelocity.z;

		//Set Random
		m_Particles[m_FirstFreeParticle * 4 + i].m_RandomColorRGBA = randomValues;

		//Set Time
		m_Particles[m_FirstFreeParticle * 4 + i].m_Time = m_CurrentTime;
	}

	m_FirstFreeParticle = nextFreeParticle;

	return XEResult::Ok;
}

void ParticleSystem::AddNewParticlesToVertexBuffer()
{
	if (m_FirstNewParticle < m_FirstFreeParticle)
	{
		// If the new particles are all in one consecutive range,
		// we can upload them all in a single call.
		m_VertexBuffer->SetVertexData(m_FirstNewParticle * 4,
										m_Particles,
										m_FirstNewParticle * 4,
										(m_FirstFreeParticle - m_FirstNewParticle) * 4,
										XE_LOCK_NOOVERWRITE);
	}
	else
	{
		// If the new particle range wraps past the end of the queue
		// back to the start, we must split them over two upload calls.
		m_VertexBuffer->SetVertexData(m_FirstNewParticle * 4,
										m_Particles,
										m_FirstNewParticle * 4,
										(m_Settings.m_MaxParticles - m_FirstNewParticle) * 4,
										XE_LOCK_NOOVERWRITE);

		if (m_FirstFreeParticle > 0)
		{
			m_VertexBuffer->SetVertexData(0,
											m_Particles,
											0,
											m_FirstNewParticle * 4,
											XE_LOCK_NOOVERWRITE);
		}
	}

	// Move the particles we just uploaded from the new to the active queue.
	m_FirstNewParticle = m_FirstFreeParticle;
}