/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/
#pragma once

#ifndef _LIGHT_DEFS_H
#define _LIGHT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <limits>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "Utils\Frustum.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

#define XE_MAX_LIGHTS								128

#define XE_LIGHT_SPOT_SHADOW_TEXTURE_HEIGHT			1024
#define XE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH			1024

#define XE_LIGHT_NUM_CASCADE_MAPS					3

#define XE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT			2048
#define XE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH			2048

#define XE_LIGHT_SHADOW_TEXTURE_FORMAT				DXGI_FORMAT_R32G32_FLOAT

#define XE_LIGHT_SHADOW_DISABLE_INDEX				std::numeric_limits<uint32_t>::max()

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class LightType : uint32_t
{
	Invalid = 0,
	Spot,
	Omni,
	Directional
};

enum LightFXFlags
{
	XE_LFXF_None			= 0x00,
	XE_LFXF_Enabled			= 0x01,
	XE_LFXF_ShadowEnabled	= 0x02
};

/********************
*   Forward Decls   *
*********************/

/***************
*   Typedefs   *
****************/
typedef std::array<Frustum, XE_LIGHT_NUM_CASCADE_MAPS> CascadeFrustums;
typedef std::array<glm::mat4, XE_LIGHT_NUM_CASCADE_MAPS> CascadeViewMatrix;
typedef std::array<glm::mat4, XE_LIGHT_NUM_CASCADE_MAPS> CascadeProjectionMatrix;
typedef std::array<float, XE_LIGHT_NUM_CASCADE_MAPS + 1> CascadeDepths;
typedef std::array<glm::vec3, XE_LIGHT_NUM_CASCADE_MAPS> CascadeShadowPosition;

/*************
*   Struct   *
**************/
struct LightCascadeInfo
{
	CascadeFrustums m_CascadeFrustums;

	CascadeViewMatrix m_CascadeViewMatrix;

	CascadeProjectionMatrix m_CascadeProjectionMatrix;

	CascadeDepths m_CascadeDepths;

	CascadeShadowPosition m_CascadeShadowPosition;

	LightCascadeInfo();
};

struct LightCascadeInfoFX
{
	glm::mat4 m_CascadeViewProjectionMatrix[XE_LIGHT_NUM_CASCADE_MAPS];
	float m_CascadeDepths[XE_LIGHT_NUM_CASCADE_MAPS + 1];

	LightCascadeInfoFX();
};

struct LightFX
{
	glm::vec4 m_Color = XEColors::Transparent;

	float m_NearAttenuation = 0.0f;

	float m_FarAttenuation = 0.0f;

	float m_FallOffAngle = 0.0f;

	float m_Angle = 0.0f;

	glm::vec3 m_Position = XEMathHelpers::Vec3fZero;

	LightType m_LightType = LightType::Invalid;

	glm::vec3 m_Direction = XEMathHelpers::Vec3fZero;

	float m_Intensity = 0.0f;

	uint32_t m_LightFXFlags = 0x00;

	uint32_t m_ShadowTextureIndex = 0;

	LightFX();

	/// <summary>
	/// Not Equal Operator Overload
	/// </summary>
	/// <returns>True if they are not the same</returns>
	bool operator!=(const LightFX& other);

	/// <summary>
	/// Equal Operator Overload
	/// </summary>
	/// <returns>True if they are the same</returns>
	bool operator==(const LightFX& other);

};

struct SpotLightShadowInfoFX
{
	glm::mat4 m_ViewProjectionMatrix = XEMathHelpers::Mat4Identity;

	SpotLightShadowInfoFX();
};

#endif