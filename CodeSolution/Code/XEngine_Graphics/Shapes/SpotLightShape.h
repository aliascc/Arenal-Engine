/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SPOT_LIGHT_SHAPE_H
#define _SPOT_LIGHT_SHAPE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "LightShape.h"
#include "Color\Color.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionColor.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class SpotLightShape sealed : public LightShape
{
	private:

		uint32_t m_NumEdges = 0;

		float m_NearAttenuation = 0.0f;
		float m_FarAttenuation = 0.0f;
		float m_Angle = 0.0f;
		float m_FallOffAngle = 0.0f;

		uint32_t m_SizeOfInnerCone = 0;
		uint32_t m_SizeOfRings = 0;

		Color m_NearAttColor = XE_LIGHT_SHAPE_DEBUG_DEFAULT_NEAR_COLOR;
		Color m_FarAttColor = XE_LIGHT_SHAPE_DEBUG_DEFAULT_FAR_COLOR;
		Color m_InnerCircleColor = XE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_INNER_COLOR;
		Color m_LineInnerColor = XE_LIGHT_SHAPE_DEBUG_DEFAULT_INNER_CIRCLE_COLOR;
		Color m_LineNearColor = XE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_NEAR_COLOR;
		Color m_LineFarColor = XE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_FAR_COLOR;

		VertexBuffer<VertexPositionColor>* m_InnerConeAndLines = nullptr;
		VertexPositionColor* m_InnerConeAndLinesVtxs = nullptr;

		VertexBuffer<VertexPositionColor>* m_NearFallOfAngleRing = nullptr;
		VertexPositionColor* m_NearFallOfAngleRingVtxs = nullptr;

		VertexBuffer<VertexPositionColor>* m_NearAttenuationRing = nullptr;
		VertexPositionColor* m_NearAttenuationRingVtxs = nullptr;

		VertexBuffer<VertexPositionColor>* m_FarFallOfAngleRing = nullptr;
		VertexPositionColor* m_FarFallOfAngleRingVtxs = nullptr;

		VertexBuffer<VertexPositionColor>* m_FarAttenuationRing = nullptr;
		VertexPositionColor* m_FarAttenuationRingVtxs = nullptr;

		void UpdateInnerConeAndLines();

		void UpdateLines(uint32_t startIndex, glm::vec3 startPoints[4], glm::vec3 endPoints[4], Color color);

		void UpdateNearAttenuationRing();

		void UpdateNearFallOffAttenuationRing();

		void UpdateFarAttenuationRing();

		void UpdateFarFallOffAttenuationRing();

		void UpdateRingInfo(VertexPositionColor vtxVector[], float size, float distance, Color color);

	public:
		//Constructor Destructor.
		SpotLightShape(GraphicDevice* graphicDevice, uint32_t numEdges = XE_LIGHT_SHAPE_DEFAULT_NUM_EDGES);
		virtual ~SpotLightShape();

		XEResult UpdateLightInfo(float nearAttenuation, float farAttenuation, float angle, float fallOfAngle);

		XEResult BuildLightShape() override;
};

#endif

