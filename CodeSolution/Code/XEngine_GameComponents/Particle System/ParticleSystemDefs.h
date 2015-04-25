/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _PARTICLE_SYSTEM_DEFS_H
#define _PARTICLE_SYSTEM_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
#include "GraphicStates.h"

/********************************
*		Using					*
*********************************/
using namespace DirectX;
using namespace PackedVector;

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/

/// <summary>
/// Settings struct describes all the tweakable options used
/// to control the appearance of a particle system.
/// </summary>
struct ParticleSystemSettings sealed : public XEObject
{
	// Name of the texture used by this particle system.
	std::string			m_TextureName;

	// Maximum number of particles that can be displayed at one time.
	uint32_t			m_MaxParticles;

	// How long these particles will last in seconds.
	float				m_Duration;;

	// If greater than zero, some particles will last a shorter time than others.
	float				m_DurationRandomness;

	// Controls how much particles are influenced by the velocity of the object
	// which created them. You can see this in action with the explosion effect,
	// where the flames continue to move in the same direction as the source
	// projectile. The projectile trail particles, on the other hand, set this
	// value very low so they are less affected by the velocity of the projectile.
	float				m_EmitterVelocitySensitivity;

	// Range of values controlling how much X and Z axis velocity to give each
	// particle. Values for individual particles are randomly chosen from somewhere
	// between these limits.
	float				m_MinHorizontalVelocity;
	float				m_MaxHorizontalVelocity;

	// Range of values controlling how much Y axis velocity to give each particle.
	// Values for individual particles are randomly chosen from somewhere between
	// these limits.
	float				m_MinVerticalVelocity;
	float				m_MaxVerticalVelocity;

	// Direction and strength of the gravity effect. Note that this can point in any
	// direction, not just down! The fire effect points it upward to make the flames
	// rise, and the smoke plume points it sideways to simulate wind.
	XMFLOAT3			m_Gravity;

	// Controls how the particle velocity will change over their lifetime. If set
	// to 1, particles will keep going at the same speed as when they were created.
	// If set to 0, particles will come to a complete stop right before they die.
	// Values greater than 1 make the particles speed up over time.
	float				m_EndVelocity;

	// Range of values controlling the particle color and alpha. Values for
	// individual particles are randomly chosen from somewhere between these limits.
	XMCOLOR				m_MinColor;
	XMCOLOR				m_MaxColor;

	// Range of values controlling how fast the particles rotate. Values for
	// individual particles are randomly chosen from somewhere between these
	// limits. If both these values are set to 0, the particle system will
	// automatically switch to an alternative shader technique that does not
	// support rotation, and thus requires significantly less GPU power. This
	// means if you don't need the rotation effect, you may get a performance
	// boost from leaving these values at 0.
	float				m_MinRotateSpeed;
	float				m_MaxRotateSpeed;

	// Range of values controlling how big the particles are when first created.
	// Values for individual particles are randomly chosen from somewhere between
	// these limits.
	float				m_MinStartSize;
	float				m_MaxStartSize;

	// Range of values controlling how big particles become at the end of their
	// life. Values for individual particles are randomly chosen from somewhere
	// between these limits.
	float				m_MinEndSize;
	float				m_MaxEndSize;

	//Blend State for this Particle System
	GraphicBlendState	m_BlendState;

	//Constructor
	ParticleSystemSettings();
	ParticleSystemSettings(const std::string textureName, uint32_t maxParticles, float duration, float durationRandomness, float emitterVelocitySensitivity,
		float minHorizontalVelocity, float maxHorizontalVelocity, float minVerticalVelocity, float maxVerticalVelocity,
		const XMFLOAT3& gravity, float endVelocity, const XMCOLOR& minColor, const XMCOLOR& maxColor, float minRotateSpeed,
		float maxRotateSpeed, float minStartSize, float maxStartSize, float minEndSize, float maxEndSize, const GraphicBlendState& blendState);
};

#endif