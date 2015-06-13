/** \file
* Contains definitions for handling Audio
*/

/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AUDIO_DEFS_H
#define _AUDIO_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

/************
*   Enums   *
*************/
enum class AudioType : uint32_t
{
	Sound = 0,
	Music
};

/**************
*   Typedef   *
***************/

/******************
*   Struct Decl   *
*******************/

struct AudioPlayerState sealed : public AEObject
{
	float m_Volume = 100.0f;

	float m_Attenuation = 1.0f;

	float m_MinDistance = 1.0f;

	float m_Pitch = 1.0f;

	glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

	bool m_Loop = false;

	bool m_Is3D = false;

	AudioPlayerState()
	{
	}
};

#endif