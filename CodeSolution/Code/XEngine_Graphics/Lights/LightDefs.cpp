/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/15/2013
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Light.h"
#include "LightDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/***********************
*   LightCascadeInfo   *
************************/
LightCascadeInfo::LightCascadeInfo()
{
	std::fill(m_CascadeViewMatrix.begin(), m_CascadeViewMatrix.end(), XEMathHelpers::Mat4Identity);
	std::fill(m_CascadeProjectionMatrix.begin(), m_CascadeProjectionMatrix.end(), XEMathHelpers::Mat4Identity);
	std::fill(m_CascadeDepths.begin(), m_CascadeDepths.end(), 0.0f);
	std::fill(m_CascadeShadowPosition.begin(), m_CascadeShadowPosition.end(), XEMathHelpers::Vec3fZero);
}

/*************************
*   LightCascadeInfoFX   *
**************************/
LightCascadeInfoFX::LightCascadeInfoFX()
{
	memset(m_CascadeViewProjectionMatrix, 0, sizeof(glm::mat4) * XE_LIGHT_NUM_CASCADE_MAPS);
	memset(m_CascadeDepths, 0, sizeof(float) * XE_LIGHT_NUM_CASCADE_MAPS);
}

/**************
*   LightFX   *
***************/
LightFX::LightFX()
{
}

bool LightFX::operator!=(const LightFX& other)
{
	return ( !(*this == other) );
}

bool LightFX::operator==(const LightFX& other)
{
	return	(
				this->m_LightType			== other.m_LightType			&&
				this->m_Color				== other.m_Color				&&
				this->m_NearAttenuation		== other.m_NearAttenuation		&&
				this->m_FarAttenuation		== other.m_FarAttenuation		&&
				this->m_Position			== other.m_Position				&&
				this->m_Direction			== other.m_Direction			&&
				this->m_Intensity			== other.m_Intensity			&&
				this->m_Angle				== other.m_Angle				&&
				this->m_FallOffAngle		== other.m_FallOffAngle			&&
				this->m_LightFXFlags		== other.m_LightFXFlags			&&
				this->m_ShadowTextureIndex	== other.m_ShadowTextureIndex
			);
}

/****************************
*   SpotLightShadowInfoFX   *
*****************************/
SpotLightShadowInfoFX::SpotLightShadowInfoFX()
{
}
